//The premise for using CRTP is knowing the type to use at compile time and wanting to do something about the interface
#include "CRTPSub1.h"
#include "CRTPSub2.h"
#include "Malformed.h"
#include <iostream>

using namespace std;

template <typename T> void executeTask(CRTPBase<T>& actors)
{
	cout << __FUNCSIG__ << endl;
	actors.task1();
	actors.task2();
}

int main(int, const char*)
{
	CRTPSub1 sub1;
	CRTPSub2 sub2;
	Malformed oops;
	executeTask(sub1);
	executeTask(sub2);
	executeTask(oops);
	return 0;
}