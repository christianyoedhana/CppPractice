#include <iostream>
using namespace std;

int& badReturn() {
	int i = 0;
	cout << "i" << endl;
	return ++i;
}

int* potentialDangling() {
	return new int;
}

void takingARawPointer(int* ptr)
{
	cout << "Accessing the 47th element" << endl;
	cout << ptr[47] << endl;
}

void codaAnalysisTest() {
	int i;
	int* iPtr = new int;
	int iArray[10];
	for (int j = 0; j < 20; ++j)
	{
		cout << iArray[j] << " ";
	}
	cout << endl;
	int* jPtr = new int;

	{
		cout << *jPtr << endl;
		delete jPtr;
	}

	cout << *jPtr << endl;
	int* iArrayPtr = new int[10];
	for (int j = 0; j < 20; ++j)
	{
		cout << iArrayPtr[j] << " ";
	}

	delete iArrayPtr;

	int& danger = badReturn();

	*potentialDangling() = 0;
}
