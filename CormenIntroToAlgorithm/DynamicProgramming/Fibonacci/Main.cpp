#include "FibonacciVariants.h"
#include <iostream>
#include <chrono>
#include "AmortizedRunTimer/RunTimer.h"

//You have to see the assembly code to prove that the fib assignment simply moves an integer, not a function call jump followed by a stack pop
void constexprDemo() {
	using namespace std;
	constexpr uint32_t N = 20;
	//Oooh the compiler refuse to evaluate Fibonacci::standardRecursive at compile time for a large N. Make sense since this solution is O(2^N)
	constexpr auto fib = Fibonacci::standardRecursive(N);
	cout << "Fibonacci(" << N << ") = " << fib << endl;
}

template<typename TimeUnit, typename Func>
std::string timeFibonacciImpl(Func func, uint32_t N)
{
	//At this point I already lost the function name
	using namespace std;
	MyPatterns::RunTimer runTimer;
	constexpr uint32_t NUM_LOOP = 1;
	auto fibM = runTimer(NUM_LOOP, func, N);
	string result =  "Fibonacci(" + std::to_string(N) + ") = " + std::to_string(fibM) + "\n";
	result += runTimer.latency<TimeUnit>();
	//yeah std::chrono::milliseconds can be template parameterized also.
	return result;
}

int main() {
	using namespace std;
	//Force a runtime evaluation of Fibonacci::standardRecursive
	cout << "Enter Fibonacci sequence to evaluate : ";
	uint32_t N = 0;
	cin >> N;
	cout <<"Fibonacci("<<N<<") bottom-up\n"<< timeFibonacciImpl < std::chrono::microseconds>(Fibonacci::bottomUp, N) << " us" << endl;
	cout<< "Fibonacci(" << N << ") top-down\n" << timeFibonacciImpl < std::chrono::microseconds> (Fibonacci::topDown, N)<<" us"<<endl;
	cout<< "Fibonacci(" << N << ") brute-force\n" << timeFibonacciImpl<std::chrono::milliseconds> (Fibonacci::standardRecursive, N)<< " ms"<<endl;
	return 0;
}