#pragma once
//I declare this in a header to remind myself of the syntax differences between class declaration and definition. Like the use of virtual and override keywords
class Base {
public:
	virtual Base* clone();
	virtual ~Base() = 0;
};

class Derived : public Base {
public:
	Derived* clone() override;
};

class PaperCup : public Derived {
public:
	PaperCup* clone() override;
};

class Battleship : public PaperCup {
public:
	Battleship* clone() override;
};

void covariantRawPointerDemo();