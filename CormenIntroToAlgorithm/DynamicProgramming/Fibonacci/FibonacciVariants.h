#pragma once
#include <cstdint>

namespace Fibonacci {
	//constexpr requires that the compiler can see the function on every translatrion unit. Plus it also implies inline.
	//This way compiler can try to evaluate the function at compile time. If it cannot, function gets called at runtime.
	constexpr std::uint32_t standardRecursive(std::uint32_t N) {
		if (N < 2) return N;
		return standardRecursive(N - 1) + standardRecursive(N - 2);
	}
	
	//Top down optimization
	std::uint32_t topDown(std::uint32_t N);
	//Bottom up optimization
	std::uint32_t bottomUp(std::uint32_t N);
}
