/*
* Constexpr expression (talk about redundancy) can be applied to an type and a function (including constructors, overloaded operators, etc).
* Scott Meyer's explanation of constexpr and what it does is simple. To paraphrase:
* 1.constexpr applied to a type enforces its constness where const statement can fail. Here is an example
* int i=10;
* int const j = i; //const is east, per Dan Sak's recommendation, 
* In this example, compiler silently convert j's initialization to a run-time initialization. j still cannot be mutated at runtime.
* This is fine, until we want to do this
* int k[j] = {0}; //Compiler error, j is not a constant expression
* If we declare j as constexpr int j = i, compiler will fail and balk about not using a constexpr to initialize j.
* 2. constexpr applied to a function gives that function a chance to be evaluated at compile time if all of the parameters can be deduced at compile-time.
* so this statement: int k[2 * numIntsForOurPurpose(10) -1] = {0}; will compile if:
* a. We declare numIntsForOurPurpose as constexpr size_t numIntsForOurPurpose(size_t numNodes);
* b. When used, numNodes takes a constexpr.
* c. No I/O in the function.
* If numNodes take a runtime deduced value, then numIntsForOurPurpose will be just another runtime function.
* Now we can safely initialize global variables with consexpr functions.
* Neat, eh?
* I just have not seen enough real-world examples of where a constexpr class TypeDefinition {...} is useful
*/
#include <iostream>
#include <array>
#include <algorithm>

void constexprStrictnessDemo()
{
	int i = 10;
	const int j = i;	//This will compile
	//constexpr int k = i;	//This will fail compilation
	constexpr int k =10;	//This is fine
	std::cout << __FUNCSIG__ <<" "<<i<<" "<<j<<" "<<k<< std::endl;
}

constexpr uint32_t factorial(uint32_t N) {
	//factorial(0) is 1, factorial(1) is also 1
	uint32_t result = 1;
	for (uint32_t i = 1; i < N+1; ++i)
	{
		result *= i;
	}
	return result;
}

int main() {
	std::array<int, factorial(4)> i{ 0 };
	fill(i.begin(), i.end(), factorial(5));
	int k = 0;
	int j = 1;
	std::cout << "Just to prevent compiler optimization, i[4] is factorial(5) =  " << i[4]<<std::endl;
	std::cout <<"factorial("<<j<<") "<< factorial(j) <<" factorial("<<k<<") "<< factorial(k) << std::endl;
	return 0;
}