#include "PureVirtualDemo.h"
#include <iostream>
#include <typeinfo>
using namespace std;

namespace PureVirtualDemo {
	//TODO HOW DOES THIS WORK?
	Projectile::~Projectile() = default;

	void Sherman::takeDamage(Projectile& round) {
		cout << __FUNCSIG__ << endl;
		round.damage(*this);
	}
	
	void Tiger::takeDamage(Projectile& round) {
		cout << __FUNCSIG__ << endl;
		//Projectile simply bounce off
		round.damage(*this);	//Visitor pattern. Now Projectile needs to know how to damage a Tiger tank.
	}

	void Round76MM::damage(Tank& target) {
		cout << __FUNCSIG__ << endl;
	};

	void Round88MM::damage(Tank& target) {
		cout << __FUNCSIG__ << endl;
	};

	void pureVirtualDemo()
	{
		Sherman americanTank;
		Round88MM germanRound;

		Tank& tank = americanTank;
		Projectile& round = germanRound;

		//Expect Sherman::takeDamage(Round88MM)
		cout << "americanTank.takeDamage(germanRound); RESULT: ";
		americanTank.takeDamage(germanRound);
		cout << "\n";

		//Expect Sherman::takeDamage(Projectile)
		cout << "americanTank.takeDamage(round); RESULT: ";
		americanTank.takeDamage(round);
		cout << "\n";

		//Expect Sherman::takeDamage(Round88MM)
		cout << "tank.takeDamage(germanRound); RESULT: ";
		tank.takeDamage(germanRound);
		cout << "\n";

		//Expect Round88MM::damage(Sherman)
		cout << "round.damage(americanTank); RESULT: ";
		round.damage(americanTank);
		cout << "\n";

		//Expect Round88MM::damage(Tank)
		cout << "round.damage(tank); RESULT: ";
		round.damage(tank);
		cout << "This can be a problem if the intention is to specifically call Round88MM::damage(Sherman)" << endl;
		cout << "\n";

		Round76MM americanRound;
		Tiger germanTank;
		Tank& beast = germanTank;
		Projectile& puny = americanRound;

		//Expected result: 
		//Tiger::takeDamage(Projectile&)
		//Then Projectile::damage(Tiger&)
		//Tiger tank takes 'damage' from Round76MM, if it has not already bounce off
		beast.takeDamage(puny);
		//Expected result :
		//Sherman::takeDamage(Projectile&)
		//Projectile::damage(Tiger&)
		tank.takeDamage(round);
	}
}
