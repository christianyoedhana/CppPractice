#include "Ch2_1.h"

template <class Container, class Predicate> 
void insertionSort(Container& toSort, Predicate&& compare) {
	auto N = toSort.size();
	//Outer loop is the unsorted sequence. I use int because the loop invariant indexer starts at 0, and the first terminating condition is now i>-1 instead of i>0
	//If we use the book's algorithm verbatim, the first element will be left out of the sorting
	for (decltype(N) j = 1; j < N; ++j) {
		auto key = toSort[j];
		int i = static_cast<int>(j - 1); //Of course this blow up when j >= 2^31 -1
		//Moving backward is more convenient that traversing from 0 because the key position can be bubbled in by the element prior to the key
		//Moving from 0 makes a more natural loop, until you need to figure out how to shift the rest of the sorted elements in the loop invariant once you
		//find a suitable insertion position
		for (; (i > -1) && !compare(toSort[i], key); --i) {
			toSort[i + 1] = toSort[i];
		}
		toSort[i+1] = key;
	}
}