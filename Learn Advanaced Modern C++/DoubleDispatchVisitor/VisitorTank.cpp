#include "VisitorProjectile.h"
#include "VisitorTank.h"
#include <iostream>
using namespace std;

void Tank::takeDamage(Projectile& round) {
	cout << __FUNCSIG__ << endl;
	//Option 1: takeDamage function only exist here. Subclass implements all of the virtual function calls that Tank::takeDamage will call
	//Call other virtual function to calculate the amount of damage. As long as projectile calls its own virtual functions, C++ code can work just fine without double dispatch
}


void Sherman::takeDamage(Projectile& round) {
	cout << __FUNCSIG__ << endl;
	round.damage(*this);
}

void Sherman::takeDamage(Round88MM& round)
{
	cout << __FUNCSIG__ << endl;
	//This tank is now a zippo lighter
	round.damage(*this);
}

void Tiger::takeDamage(Projectile& round){
	cout << __FUNCSIG__ << endl;
	//Projectile simply bounce off
	round.damage(*this);	//Visitor pattern. Now Projectile needs to know how to damage a Tiger tank.
}