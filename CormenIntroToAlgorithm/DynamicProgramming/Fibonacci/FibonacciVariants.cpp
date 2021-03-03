#include "FibonacciVariants.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <array>

namespace Fibonacci {
	using namespace std;
	//I prefer vector::at to operator[] because of the range check in vector::at
	uint32_t topDownHelper(uint32_t N, vector<uint32_t>& memoize)
	{
		if (N < 2) return N; //This should be as quick as referring to the memoize table
		if (memoize.at(N) == numeric_limits<decltype(N)>::max()) {
			memoize.at(N) = topDownHelper(N - 1, memoize) + topDownHelper(N - 2, memoize);
		}
		return memoize.at(N);
	}

	//This is a demo mainly for comparing brute force latency vs dynamic-programming optimized latency.
	//Main disadvantage is the size of the memoize table.
	uint32_t topDown(uint32_t N)
	{
		using Table = vector<decltype(N)>;
		//Can't use array. N is not compile-time
		Table memoize(static_cast<Table::size_type>(N)+1);
		fill(memoize.begin(), memoize.end(), numeric_limits<decltype(N)>::max());
		return topDownHelper(N, memoize);
	}

	//This is the least familiar way of computing a Fibonacci sequence.
	//Takes advantage of 2 properties:
	//1. Any tail recursion always have its iterative equivalent. This is a proven algo theorem
	//2. Fibonacci calculation cares only about the 2 previous results
	uint32_t bottomUp(uint32_t N)
	{
		uint32_t  memoize[2] = { 0,1 };
		for (uint32_t n = 2; n < N + 1; ++n)
		{
			memoize[n % 2] = memoize[0] + memoize[1];
		}
		return memoize[N % 2];
	}
}