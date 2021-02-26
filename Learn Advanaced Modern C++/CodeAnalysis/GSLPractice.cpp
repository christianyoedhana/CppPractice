#include <gsl\gsl>
#include <memory>
#include <iostream>

using std::cout;
using std::endl;
using std::unique_ptr;

//So at this point the runtime just crash. Per
void fnTakingPointer(gsl::not_null<int*> ptr) {
	cout << *ptr << endl;
}

void fnNakedPtr(const int* ptr)
{
	cout << *ptr << endl;
}

void pointerUse()
{
	cout << __FUNCSIG__ << endl;
	unique_ptr<int> intStorage = std::make_unique<int>();
	fnTakingPointer(intStorage.get());
	int* intPtr = nullptr;
	const gsl::not_null<int*> notNull(intPtr);
	//This one silently crash
	//fnNakedPtr(intPtr);
	//This one will terminate the program with an assertion.
	fnTakingPointer(intPtr);
	//This is now caught by the compiler because not_null static assert when constructed with a nullptr
	//fnTakingPointer(nullptr);
}