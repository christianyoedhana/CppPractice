#include <iostream>
#include "RawPointer.h"
using namespace std;

Base* Base::clone() {
	cout << __FUNCSIG__ << endl;
	return this;
}

//Did not know I can do this. Fancy, but I think {} is shorter
Base::~Base() = default;

Derived* Derived::clone() {
	cout << __FUNCSIG__ << endl;
	return this;
}

PaperCup* PaperCup::clone() {
	cout << __FUNCSIG__ << endl;
	return this;
}

Battleship* Battleship::clone() {
	cout << __FUNCSIG__ << endl;
	return this;
}

void covariantRawPointerDemo()
{
	cout << __FUNCSIG__ << endl;
	Derived derived;
	Base* base = &derived;

	//Then at some point we need to create a copyh of this derived. I expect Derived::clone to be called here
	Derived* dClode = derived.clone();
	//So what gets called? I guess Derived::clone because VS has not complained about the override keyword
	//And yes I am correct because covariant rule just kicked in!
	Base* fromBase = base->clone();

	//Of course you are already asking for trouble if you do this
	//Does my battleship fits in my papercup? Yes it does
	Battleship ship;
	PaperCup cup = ship;
	PaperCup& smallShip = ship;
	ship.clone();
	cup.clone();
	smallShip.clone();
}

//Next: covariant simulation using unique_ptr