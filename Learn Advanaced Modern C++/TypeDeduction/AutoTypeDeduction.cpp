#include <iostream>
#include <string>
#include <vector>
#include "AutoTypeDeduction.h"
using namespace std;

//General template function undefined. Has to be specialized. The goal is to fail compilation if I get the auto type deduction wrong
template <class T> void funct();

//Remember: all iostream objects are global objects. That's why the cout return works
template <> void funct<int>() {
	 cout << __FUNCSIG__ << endl;
}

template <> void funct<const int>() {
	cout << __FUNCSIG__ << endl;
}

template <> void funct<const int*>() {
	cout << __FUNCSIG__ << endl;
}

template <> void funct<int&>() {
	cout << __FUNCSIG__ << endl;
}

template <> void funct<const int*&>() {
	cout << __FUNCSIG__ << endl;
}

//This overload shows the param type and the T type, which is not always the same
template <typename T> void byValue(T)
{
	cout << __FUNCSIG__ << endl;
}

template <typename T> void byPointer(T*) {
	cout << __FUNCSIG__ << endl;
}

template <typename T> void byReference(T&) {
	cout << __FUNCSIG__ << endl;
}

template <typename T> void byConstRef(const T&) {
	cout << __FUNCSIG__ << endl;
}

//Specialization. If I get the template deduction wrong, the code won't compile
void autoTypeDeductionExercise()
{
	int anInt = 1;
	const auto anotherInt = anInt;	//auto is an int.
	cout << "int anInt = 1;\nconst auto anotherInt = anInt;" << endl;
	byValue(anotherInt);	//cv-qualifiers got stripped away when deducing by value.
	byPointer(&anotherInt);
	byReference(anotherInt);
	//And here is the lamba wrinkle with capture by value. cv qualifier is preserved, everything else follows template type deduction by value
	auto constPreserved = [anotherInt]() {funct<decltype(anotherInt)>(); };
	cout << "auto constPreserved = [anotherInt]() {funct<decltype(anotherInt)>(); };" << endl;
	constPreserved();
	//C++14 init capture uses auto type resolution
	auto initCapture = [brandNew = anotherInt]() {funct<decltype(brandNew)>(); };
	cout << "auto initCapture = [brandNew = anotherInt]() {funct<decltype(brandNew)>(); };" << endl;
	initCapture();

	const int* intPtr = &anInt;
	auto anotherPtr = intPtr;	//auto is a const int*.	You cannot 'const cast' a const int* by simply aliasing the pointer using an auto
	cout << "const int* intPtr = &anInt;\nauto anotherPtr = intPtr; " << endl;
	byValue(anotherPtr);
	byPointer(anotherPtr);
	byReference(anotherPtr);

	const int& intRef = anInt;
	cout << "const int& intRef = anInt;" << endl;
	byValue(intRef);
	byReference(intRef);
	auto whatAmI = intRef;
	cout<<"auto whatAmI = intRef"<<endl;	//auto does not take reference, and throws away the top const qualifier, just like template type deduction by value
	funct<decltype(whatAmI)>();

	const int* const constPtr = &anInt;
	auto autoConstPtr = constPtr;
	cout << "const int* const constPtr = &anInt;\nauto autoConstPtr = constPtr; " << endl;
	byValue(autoConstPtr);
	byPointer(autoConstPtr);
	byReference(autoConstPtr);

	auto x{ 1 };	//auto type deduced to an int
	cout << "auto x{ 1 };" << endl;
	byValue(x);
	auto y = { 1 };	//auto type deduced to a std::initializer_list<int>
	cout << "auto y = { 1 };" << endl;
	byValue(y);
	//auto z{1,2,3}; //fails compilation because {} has no type, unless in the first 2 usages

	//array types will be deduced to its pointer reduction by value. By ref the array type is preserved.
	int intArray[2];
	cout << "int intArray[2];" << endl;
	byValue(intArray);
	byPointer(intArray);
	byReference(intArray);
	
	int (&arrayRef)[2] = intArray;	//14 years and this is VERY VERY first time I have to write a reference to an array. And I figure it out by looking at the output of __FUNCSIG__
	cout << "int(&arrayRef)[2] = intArray;" << endl;
	byValue(arrayRef);
	byReference(arrayRef);

	//Now to function auto return type deduction. auto as a return type uses template type deduction. strips out references
	//But decltype(auto) uses decltype for expression type deduction. Will add & reference when appropriate for expressions.

	//Scott Meyer's rule of thumb:
	//1. use auto if a reference return type is never correct. Return by value is the only correct behavior
	//2. use decltype(auto) if a reference type can be the correct return type.
	
	class AutoOtto {
	public:
		auto getAnInt() {
			return 0;
		}

		decltype(auto) getAnIntRef() {
			return 0;
		}
	};

	/*AutoOtto autoOtto;
	auto justInt = autoOtto.getAnInt();
	auto intWithRef = autoOtto.getAnIntRef();
	funct<decltype(justInt)>();
	*/
}

class Something {};

vector<Something> factoryOfSomething()
{
	return vector<Something>{10};
}

template <> void funct<Something const*>() {
	cout << __FUNCSIG__ << endl;
}

template <> void funct<Something const* const&>() {
	cout << __FUNCSIG__ << endl;
}

template <typename T> void byConstReference(const T& p);
/*{
	funct<T>();
	funct<decltype(p)>();
}*/
//Template type deduction and auto type deduction is very similar. Auto just does not deduce to a &
void confusingTemplateTypeDeduction(){
	const auto someContainer = factoryOfSomething();
	//Is const T* == T const*? I dont think so.
	byConstRef(&someContainer[0]);
	int x = 0;
	//Watch out for this major differece. x by itself is a variable name. decltype(name) returns the actual type of the name
	cout << "decltype(x) deduced type is : "; funct<decltype(x)>();
	//On the other hand, (x) is an l-value expression. By rule, decltype(l-value expression) gets an extra&, because all l-value expressions are implicitly references.
	cout << "decltype((x)) deduced type is : "; funct<decltype((x))>();
}