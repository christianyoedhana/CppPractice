#include "MovePractice.h"
#include <iostream>
#include <utility>
using namespace std;

class Base {
public:
	Base() = default;
	Base(const Base&) {
		cout << __FUNCSIG__ << endl;
	}
	Base& operator=(const Base&){
		cout << __FUNCSIG__ << endl;
		return *this;
	}

	Base(Base&&) noexcept {
		cout<< __FUNCSIG__ << endl;
	}

	Base& operator=(Base&&) noexcept {
		cout << __FUNCSIG__ << endl;
		return *this;
	}
};

class NoMove {
public:
	NoMove() = default;
	NoMove& operator=(const NoMove&) {
		cout << __FUNCSIG__ << endl;
		return *this;
	}
	NoMove(const NoMove&) {
		cout << __FUNCSIG__ << endl;
	}
	//These lines will cause compile errors. std::swap's internal call to move tries to call this move operator
	//The difference between not allowing the compiler to generate a constructor/operator, and marking these constructor/operator as deleted is
	//the existence of these functions. When the compiler cannot generate these functions, they don't exist. Compiler cannot find these functions
	//When you mark the functions as 'delete', they exist. Compiler will find them, and dispatches them when called for. 
	//When the user sees the function as deleted, it will complain that it cannot access the function
	//NoMove& operator=(NoMove&&) noexcept = delete;
	//NoMove(NoMove&&) noexcept = delete;
};

struct BaseComposition {
	BaseComposition() = default;
	BaseComposition(BaseComposition&) = default;
	//If BaseComposition members are initialized correctly, the swap operation will ensure that rhs is safe to destruct
	BaseComposition(BaseComposition&& rhs) noexcept {
		cout << __FUNCSIG__ << endl;
		std::swap(m_aMember, rhs.m_aMember);
		//std::swap(m_noMove, rhs.m_noMove);
	}
	//See the repetition? That's why you need a custom swap function that both operator will call
	BaseComposition& operator=(BaseComposition&& rhs) noexcept {
		cout << __FUNCSIG__ << endl;
		//std::swap(*this, source);	//This will cause a stack overflow, guaranteed. Guess what function std::swap will call?
		//The strong exception safety is enforced here, because swap will create a temporary copy of source
		std::swap(m_aMember, rhs.m_aMember);
		//std::swap(m_noMove, rhs.m_noMove);
		return *this;
	}
	Base m_aMember;
	//NoMove m_noMove;
	Base* m_basePtr = nullptr;
};

void swapPractice(){
	using std::swap;
	using std::move;
	cout << __FUNCSIG__ << endl;
	Base left, right;
	swap(left, right);
	BaseComposition before, after;
	//std::swap only takes l-value parameters
	cout << "Swapping structs with swapppable and non-swappable members" << endl;
	swap(before, after);
};

void moveOrCopy(){
	cout << __FUNCSIG__ << endl;
	NoMove left, right;
	using std::swap;
	swap(left, right);
}