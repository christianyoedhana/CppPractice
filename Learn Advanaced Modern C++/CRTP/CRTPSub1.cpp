#include "CRTPSub1.h"
#include "CRTPBase.cpp"	//Only time I am allowed to include a cpp is to specialize a template across multiple cpp definion file while hiding the template implementation away from the header file
#include <iostream>
using namespace std;
//so template specialization? I have never done this.
template class CRTPBase<typename CRTPSub1>;

void CRTPSub1::act1() const
{
	cout << __FUNCSIG__ << endl;
}

void CRTPSub1::act2() const
{
	cout << __FUNCSIG__ << endl;
}

