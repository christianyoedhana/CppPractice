#include "HeapSort.h"
#include <utility>
#include <iostream>

//Heap node calculations are more convenient if we set the heap root at index = 1. if root starts at index 0, parent() has to figure out whether it needs to
//calculate i/2 for odd index, or i/2 - 1 for even index
template <class T>
constexpr T parentNode(T i) { return (i-1) / 2; }
//constexpr T parentNode(T i) { return i / 2; }	//Cormen is using 1-based array

template <class T>
constexpr T leftNode(T i) { return 2*i + 1; }
//constexpr T leftNode(T i) { return 2 * i; }	//Cormen is using 1-based array

template <class T>
constexpr T rightNode(T i) { return 2*i + 2; }
//constexpr T rightNode(T i) { return 2*i + 2); }	//Cormen is using 1-based array

//General implementation of heapify, taking a comparator
//@todo modify heapify so that, instead of taking the heapSize parameter, which is redundant to heap.size(), take the last leaf index to heapify
//lastLeaf < heap.size()
template <class T, class Comparator>
void heapify(T& heap, typename T::size_type i, typename T::size_type lastLeaf, Comparator&& compare) {
	auto largest = i;
	auto left = leftNode(i);

	if (!(left > lastLeaf)) {
		if (compare(heap.at(left), heap.at(largest))) {
			largest = left;
		}
	}
	auto right = rightNode(i);
	if (!(right > lastLeaf)) {
		if (compare(heap.at(right), heap.at(largest))) {
			largest = right;
		}
	}

	if (largest != i) {
		std::swap(heap.at(i), heap.at(largest));
		heapify(heap, largest, lastLeaf, compare);
	}
}

template <class T>
void heapifyMax(T& heap, typename T::size_type from, typename T::size_type to) {
	heapify(heap, from, to, [](auto const& left, auto const& right) { return left > right; });
}

template <class T>
void heapifyMin(T& heap, typename T::size_type from, typename T::size_type to) {
	heapify(heap, from, to, [](auto const& left, auto const& right) { return left < right; });
}

template <class T, class Heapify>
void buildHeap(T& heap, typename T::size_type lastLeaf, Heapify& heapify)
{
	using Index = typename T::size_type;
	/*for (Index i = parentNode(lastLeaf); i > 0; --i) {
		heapify(heap, i, lastLeaf);
	}
	//This is the problem with backward loop heapify for zero-based index
	heapify(heap, 0, lastLeaf);*/

	//Cormen's book counts backward from the parentNode of the lastLeaf index downto 0
	//There is no guarantee if the index uses signed integer or not.
	//However, we can all agree that the parentIndex is >=0, so using subtraction of parentIndex with i, where parentIndex >= i is a guarantee
	const auto parentIndex = parentNode(lastLeaf);
	for (Index i = 0; i <= parentIndex; ++i) {
		heapify(heap, parentIndex - i, lastLeaf);
	}
}

template <class T, class Heapify>
void heapSort(T& heap, Heapify& heapify)
{
	for (auto iter = 1; iter < heap.size(); ++iter) {
		auto lastLeaf = heap.size() - iter;
		buildHeap(heap, lastLeaf, heapify);
		std::swap(heap[lastLeaf], heap[0]);
		for_each(heap.begin(), heap.end(), [](auto item) {std::cout << item << " "; });
		std::cout << std::endl;
	}
}

template <class T> void heapSortAscending(T& heap) {
	std::cout << "Heapsort ascending progress" << std::endl;
	heapSort(heap, heapifyMax<T>);
}

template <class T> void heapSortDescending(T& heap) {
	std::cout << "Heapsort descending progress" << std::endl;
	heapSort(heap, heapifyMin<T>);
}

template <class T>
void heapSort(T& heap) {
	heapSortAscending(heap);
	heapSortDescending(heap);
}