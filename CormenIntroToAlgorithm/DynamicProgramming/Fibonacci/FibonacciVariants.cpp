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
	
}