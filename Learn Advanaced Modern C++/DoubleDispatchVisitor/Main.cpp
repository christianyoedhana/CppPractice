///This project demonstrates visitor pattern to simulate double dispatch, as shown in https://en.wikipedia.org/wiki/Double_dispatch
//I am trying this just to familiarize myself with a Visitor implementation. I am still convinced, however, that a proper single dispatch runtime polymorphism
//can leverage the static time function overload, negating the necessity of double dispatch.
//If the functions in the parent class calls virtual functions overloaded in the child classes, we should get the correctly runtime polymorphic behaviour without Visitor.

#include "VisitorDemo.h"
#include "PureVirtualDemo.h"

#include <iostream>
using namespace std;

int main()
{
	visitorDemo();
	cout << endl;
	PureVirtualDemo::pureVirtualDemo();
	return 0;
}