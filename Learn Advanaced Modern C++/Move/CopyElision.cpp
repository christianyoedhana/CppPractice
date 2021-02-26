#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Foo{
	string m_words;
public:
	static uint32_t m_count;
	Foo() = default;
	explicit Foo(int x) {
		cout << __FUNCSIG__ << " " << x << endl;
	}

	Foo(const Foo& foo) {
		cout << __FUNCSIG__ << endl;
		m_words = foo.m_words;
		m_count += 1;
	}

	//Does this defeat copy elision? Uh oh do I have to define my own move operator?
	Foo& operator=(const Foo& foo) {
		cout << __FUNCSIG__ << endl;
		Foo temp(foo);
		swap(*this, temp);
		return *this;
	}

	Foo(Foo&& rvalue) noexcept {
		m_words = rvalue.m_words;
		rvalue.m_words.clear();
	}

	void setWords(const string& words) {
		m_words = words;
	}
	inline friend ostream& operator<<(ostream& out, const Foo& foo) {
		out << foo.m_words;
		return out;
	}
};

uint32_t Foo::m_count;

Foo Bar()
{
	cout << "I worship 47!\n";
	return Foo{ 47 };
}

//This one involves 1 copy construction. I suspect move gets involved here
Foo MoreBar()
{
	//This is supposed to be named value optimization. But move constructor completely bypassed any copy construction.
	Foo justWords;
	justWords.setWords("Talk is cheap");
	return justWords;
}

void copyElision() {
	cout << __FUNCSIG__ << endl;
	//Without copy elision, the copy constructor gets called twice by Bar()
	Foo fortySeven = Bar();
	cout << "How many Foo(const Foo&) get called? " << Foo::m_count << endl;
	//Now let see if copy elision also works here.Nope, but move minimized the copy.
	Foo talkIsCheap = MoreBar();
	cout << "How many Foo(const Foo&) get called? " << Foo::m_count << endl;
	cout << talkIsCheap << endl;
}

void constLifetimeExtension()
{
	cout << __FUNCSIG__ << endl;
	const Foo& longLive = MoreBar();
	cout << longLive << endl;
}