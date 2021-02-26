#include "Malformed.h"
#include "CRTPBase.cpp"
#include <iostream>

using namespace std;

template class CRTPBase<Malformed>;

void Malformed::act1() {
	cout << __FUNCSIG__ << endl;
}

void Malformed::act2() const{
	cout << __FUNCSIG__ << endl;
}
