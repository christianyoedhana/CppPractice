#include <memory>
#include <iostream>
#include <vector>

using namespace std;

/*Circular reference problem happens when an owning shared pointer pointing to an object is copied by another non-owning shared pointer pointing to the same object
* This can happen in the same class definition when the class contains a shared pointer member of the same type, and an object of the class type initializes the 
* shared pointer member to itseld (which really should be flagged either by a decent static analyzer, or during code review), or
* two different objects of the same type assigning the shared pointer member to each other ( which, again, really should get flagged).
* The design question that needs to be ask is about the shared pointer member ownership. Is the intention object ownership sharing, or simply to maintain a non-dangling
* reference to another object?
* Raw pointer is a poor candidate to break this cycle because of it can dangle at some point in its lifetime.
* Implemeting an observer pattern is a solution, if implemented correctly
* weak_ptr is ideal because it temporarily extends the lifetime of the shared object (solves dangling pointer), and mandates the conversion to a shared pointer before using
* the weak_ptr instance (make a stronger case of the intention to actually use the shared_ptr object, not just accidentally assigning the shared_ptr to something.
* Here is the demo of the problem, and the solution with weak_ptr
* Q: is the solution thread-safe? There is some built-in safety in weak_ptr's ability to extend the lifetime of a shared_ptr object.
*/

//Definitely cannot be stored in any stl container. 
template<typename Butterfly>
class LiveOrDie {
	weak_ptr<Butterfly> m_butterfly;
	string m_name;
	string m_status;
public:
	LiveOrDie() = default;
	LiveOrDie(const shared_ptr<Butterfly>& toTrack) noexcept : m_butterfly{ toTrack }, m_name{ toTrack->name() } {}
	LiveOrDie(LiveOrDie&&) noexcept = default;
	LiveOrDie(const LiveOrDie&) = default;
	LiveOrDie& operator=(LiveOrDie&&) noexcept = default;
	LiveOrDie& operator=(const LiveOrDie&) = default;
	~LiveOrDie() {
		m_status = m_butterfly.expired() ? "Destroyed" : "Alive";
		cout <<__FUNCTION__<<" "<< m_name << " is " << m_status << endl;
	}
	//temporary until I figure out what's wrong with my use of weak_ptr
	void set(const shared_ptr<Butterfly>& toTrack) {
		m_butterfly = toTrack;
		m_name = toTrack->name();
	}
};

class SocialButterfly {
	shared_ptr<SocialButterfly> m_bff;
	const string m_name;
public:
	SocialButterfly(const string& name) :m_name{ name } {}
	//Nice warnings. Also btw I dont have to declare this friend function inline anymore
	friend void beMyFriend(shared_ptr<SocialButterfly>& lbff, shared_ptr<SocialButterfly>& rbff) {
		lbff->m_bff = rbff;
		rbff->m_bff = lbff;
	}
	//Nice warnings. So using operator<< in destructors is risky
	~SocialButterfly() {
		cout << m_name << " " << __FUNCSIG__ << endl;
	}

	const string& name() const { return m_name; }
};

//uses weak_ptr to be friends
class SafeSocialButterfly {
	weak_ptr<SafeSocialButterfly> m_bff;
	const string m_name;
public:
	SafeSocialButterfly(const string& name) : m_name{ name } {}

	//Btw I dont have to declare this friend function inline anymore
	friend void beMyFriend(shared_ptr<SafeSocialButterfly>& lbff, shared_ptr<SafeSocialButterfly>& rbff) {
		lbff->m_bff = rbff;
		rbff->m_bff = lbff;
	}

	const string& name() const {
		return m_name;
	}

	//weak_ptr must be converted to a shared_ptr before use. It has no operator->
	const string& myFriend() const {
		return m_bff.lock()->name();
	}
	//Nice warnings here
	~SafeSocialButterfly() {
		cout << m_name << " " << __FUNCSIG__ << endl;
	}
};

int main() {
	
	
	{
		{
			vector<LiveOrDie<SocialButterfly>> socialButterflies;
			socialButterflies.reserve(2);
			shared_ptr<SocialButterfly> agnes(make_shared<SocialButterfly>("agnes"));
			shared_ptr<SocialButterfly> gunman(make_shared<SocialButterfly>("gunman"));
			socialButterflies.emplace_back(agnes);
			socialButterflies.emplace_back(gunman);
			cout << "Agnes and gunman are befriending each other" << endl;
			beMyFriend(agnes, gunman);
			cout << "Do Agnes and gunman ever die?" << endl;
		}
	}
	
	{
		vector<LiveOrDie<SafeSocialButterfly>> safeButterflies;
		safeButterflies.reserve(2);
		shared_ptr<SafeSocialButterfly> clara(make_shared<SafeSocialButterfly>("clarabelle"));
		shared_ptr<SafeSocialButterfly> goofy(make_shared<SafeSocialButterfly>("goofy"));
		safeButterflies.emplace_back(clara);
		safeButterflies.emplace_back(goofy);
		cout << clara->name() << " befriends " << goofy->name() << endl;
		beMyFriend(clara, goofy);
		cout << clara->name() << " is now a friend of " << clara->myFriend() << endl;
		cout << goofy->name() << " is now a friend of " << goofy->myFriend() << endl;
	}
	return 0;
}

/*
As you can see from the output, agnes and gunman's SocialButterfly destructor is never called. Each of them maintain's the other's pointer reference count,
despite the lack of intention of ownership. Yes friendship is important and to be cherished, but owning your friend...even that doesn't end well in the human world.
SafeBufferfly objects clara and goofy maintains the same friendship relation without being overly possessive of their relationship with each other. 
They accomplish this using the weak_pointer object. The only catch is the need to check the weak_pointer before using it, which what you would do using a raw pointer
and real life too, right? You can't invite your friend to dinner if he/she is dead, or otherwise indisposed. So call expired() before lock() to a shared_ptr.
"Till death do us part" has more serious resource dereferencing repercussions in the C++ world than in the human world.
*/