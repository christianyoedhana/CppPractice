#pragma once
#include <cstdint>

namespace Fibonacci {
	//constexpr requires that the compiler can see the function on every translatrion unit. Plus it also implies inline.
	//This way compiler can try to evaluate the function at compile time. If it cannot, function gets called at runtime.
	constexpr std::uint32_t standardRecursive(std::uint32_t N) {
		if (N < 2) return N;
		return standardRecursive(N - 1) + standardRecursive(N - 2);
	}
	
	//Top down optimization. o(N) storage.
	std::uint32_t topDown(std::uint32_t N);
	//Bottom up optimization
	//Takes advantage of 2 properties:
	//1. Any tail recursion always have its iterative equivalent. This is a proven algo theorem
	//2. Fibonacci calculation cares only about the 2 previous results
	constexpr std::uint32_t bottomUp(std::uint32_t N) {
		uint32_t  memoize[2] = { 0,1 };
		for (uint32_t n = 2; n < N + 1; ++n)
		{
			memoize[n % 2] = memoize[0] + memoize[1];
		}
		return memoize[N % 2];
	}
}
