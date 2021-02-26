#include "CRTPSub2.h"
#include "CRTPBase.cpp"
#include <iostream>

using namespace std;
template class CRTPBase<CRTPSub2>;

void CRTPSub2::act1()
{
	cout << __FUNCSIG__ << " I will be changing my state" << endl;
}

void CRTPSub2::act2() const
{
	cout << __FUNCSIG__ << " Romeo, romeo, where art thou?" << endl;
}
