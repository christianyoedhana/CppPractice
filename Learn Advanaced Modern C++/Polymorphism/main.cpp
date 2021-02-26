#include <iostream>
using namespace std;

class BaseClass
{
public:
	virtual void foo() const 
	{
		cout << __FUNCSIG__ << endl;
	}
};

class FirstLevelDerivative : public BaseClass
{
public:
	void foo() const {
		cout << __FUNCSIG__ << endl;
	}
};

class SecondLevelDerivative : public FirstLevelDerivative
{
public:
	void foo() const {
		cout << __FUNCSIG__ << endl;
	}
};

void staticBindingFoo(BaseClass base)
{
	base.foo();		//This should generate a compile error
}

void dynamicBindingFoo(const BaseClass& base)
{
	base.foo();	//This should compile fine
}

void dynamicBindingFoo(BaseClass& base)
{
	base.foo();	//This should compile fine
}

int main()
{
	//FirstLevelDerivative lifetime extended
	const BaseClass& firstLevelRef = FirstLevelDerivative{};
	FirstLevelDerivative firstLevel;
	SecondLevelDerivative secondLevel;
	BaseClass& secondLevelRef = secondLevel;
	firstLevel.foo();
	secondLevel.foo();
	staticBindingFoo(firstLevel);
	staticBindingFoo(secondLevel);
	dynamicBindingFoo(firstLevel);
	dynamicBindingFoo(secondLevel);
	return 0;
}
//Waiting for response from FSecure. This innocent code triggers their malware detection