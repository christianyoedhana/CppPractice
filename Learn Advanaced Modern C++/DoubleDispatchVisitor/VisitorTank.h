#pragma once
class Projectile;
class Round88MM;

class Tank
{
public:
	virtual void takeDamage(Projectile& round);
};

class Sherman : public Tank
{
public:
	virtual void takeDamage(Projectile& round) override;
	//Then there is a special overload to take damage from an 88mm. instant death. I wonder if Herb Sutter or Scott Meyer have cautioned against doing this
	//in favor of a visitor implementation
	//Wait this has been known to mask the superclass's function. 
	virtual void takeDamage(Round88MM& round);
};

//Eh I will use this example to demonstrate visitor
class Tiger : public Tank
{
public:
	virtual void takeDamage(Projectile& round) override;
};