#pragma once
#include <random>

//Generate a random permutation in place, given a sequence of items
template <class T>
void randomPermutationInPlace(T& container) {
	using namespace std;
	random_device rand;
	mt19937 engine(rand());
	using IndexType = typename T::size_type;
	const IndexType N = container.size()-1;
	for (IndexType i = 0; i < N; ++i) {
		uniform_int_distribution<IndexType> generator(i, N);
		std::swap(container.at(i), container.at(generator(engine)));
	}
}