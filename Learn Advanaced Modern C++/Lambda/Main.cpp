/*
I am very comfortable using lambda expressions by now. However, using a static variable inside a lambda expression challenges my understanding of what is 
a lambda to the compiler, actually.
Remember Scott Meyer's lecture. Lamba expression, to the compiler, is a class. Compiler generates a local class and overloads the operator() to contain the 
lambda's parameter and body defition. So if the operator() function contains, and modifies, a static, and the expression containing the lambda expression gets
called repeatedly, the lambda's static variable changes accumulates. This is the nature of a static variable. The same thing will happen if you use a 
static variable in a regular c++ function, annd you call that function repeatedly. The static variable changes will accumulate.
To initialize a variable exactly once at every lambda call, while retaining the ability to reuse the variable value changes at each lambda invocation,
create and initialize the variable in the lambda capture, and declare the lambda as mutable. I think this is the reason why mutable exist for lambda expression.
Example: [aVar = valueFromOutside]() mutable { aVar = modify(aVar); }
This allows moving valueFromOutside to aVar.
*/
#include <iostream>
#include <string>
using namespace std;

//This is essentially what happen to your lambda function inside an stl algorithm
void lambdaWithStatic(int i) {
	auto lambda = [=]() {
		static auto j = i;
		cout << "Lamba j = " << ++j << endl;
	};
	lambda();
}

void lambdaWithMoveCapture(int i) {
	auto lambda = [j = i]() mutable {
		cout << "Lamba j = " << ++j << endl;
	};
	lambda();
}

int main(int, const char*) {
	cout << "Demonstrating a lambda with static var" << endl;
	for (uint32_t i = 0; i < 10; i+=2) {
		lambdaWithStatic(i);
		lambdaWithMoveCapture(i);
	}

	class Cust {
		string m_name;
	public:
		Cust(const string& name) : m_name(name) {}
	};

	Cust sam("sam");
	//Cust joe = "joe"; //this code never compiles in C++. Needs 2 conversion. const char*[4] -> std::string->Cust
	return 0;
}