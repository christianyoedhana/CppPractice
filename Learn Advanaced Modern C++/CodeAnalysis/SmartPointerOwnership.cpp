#include <memory>
#include <iostream>
#include <gsl/gsl>

using namespace std;

unique_ptr<int> g_i;
shared_ptr<int> g_shared;

//So the proper code style, owing to the undetermined order of initialization of global variables, is to initialize a global variable inside a function.
//It becomes the programmer's responsibility to cal such init function before using the global
void uniquePtrAnalysis() {
	g_i = make_unique<int>(42);
	const gsl::not_null<int*> p_i = g_i.get();
	g_i = make_unique<int>(47);
	//Why am i not seeing a warning about this?
	*p_i = 0;
	cout << *p_i << endl;
}

void sharedPtrAnalysis() {
	g_shared = make_shared<int>(42);
	//I can silence the null check requirement for P-i
	const gsl::not_null<int*> p_i = g_shared.get();
	*p_i = 1;
	g_shared.reset();
	//But here,
	//This is an invalid assignment. Why is my compiler not issuing a warning about this?
	*p_i = 0;
	cout << *p_i << endl;
}

unique_ptr<int> makeAnInt(int number)
{
	return make_unique<int>(number);
}

void wrongUseOfConstRefToExtendTempLifetime() {
	//Good, the static analyzer catches this dangling pointer problem
	const int& doesNotExtend = *makeAnInt(42);
	cout << doesNotExtend << endl;
}

void unsafeRawPtrUse()
{
	auto ptrToVector = make_shared<vector<int>>();
	ptrToVector->push_back(47);
	shared_ptr<vector<int>>* sp = &ptrToVector;
	vector<int>* ptrVector = &(*ptrToVector);
	int* ptrInt = &ptrToVector->at(0);
	ptrVector->clear();
	*ptrInt = 1;
	cout << *ptrInt << endl;
}