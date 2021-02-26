#include "Ch2_3.h"
#include <vector>
#include <algorithm>

using namespace std;
//T is definitely an stl container. Can be array, vector, or anything that supports either .at() or operator[]

template <class T>
class LessThan {
public:
	bool operator()(T left, T right) {
		return left < right;
	}
};

//check for merge correctness first, then add the predicate type
template <class T, class Pred = LessThan<typename T::value_type>>
void merge(T& container, typename T::size_type p, typename T::size_type q, typename T::size_type r) {
	//p,q,and r are not constexpr, so the internal helper container has to be vector
	//p<=q<r
	vector<typename T::value_type> left( q - p + 1);	//q > p. q==p if p==0
	//NOTE: I got into trouble using a static variable in a lambda expression. That's why I use the polymorphic capture
	generate(left.begin(), left.end(), [index = p,&container]() mutable ->typename T::value_type {
			return container.at(index++);
		});
	//Be careful with the subtle difference begween ()  and {} initializer.
	//() follows the constructor parameter rule. {} means put initial values into the container
	vector<typename T::value_type> right( r - q);		//r > q
	generate(right.begin(), right.end(), [index = q+1,&container]() mutable ->typename T::value_type {
			return container.at(index++); 
		});

	auto leftIter = left.cbegin();
	auto rightIter = right.cbegin();
	auto dest = p;
	Pred pred{};
	while (!(leftIter == left.cend() || rightIter == right.cend()))
	{
		if (pred(*leftIter, *rightIter)) {
			container.at(dest++) = *leftIter++;
		}
		else {
			container.at(dest++) = *rightIter++;
		}
	}
	//Can't use stl algorithm, not using iterators
	//Only one of this while loop will fire, so just define both and safe 2 if calls.
	while (leftIter != left.cend()) {
		container.at(dest++) = *leftIter++;
	}

	while(rightIter != right.cend()) {
		container.at(dest++) = *rightIter++;
	}
}

//left, right are zero-based element indexes
template<class T>
void mergeSort(T& container, typename T::size_type left, typename T::size_type right) {
	if (right <= left) return;	//nothing to merge if there is only 1 item left to consider for merging
	auto middle = (left + right) / 2;
	mergeSort(container, left, middle);
	mergeSort(container, middle + 1, right);
	merge(container, left, middle, right);
}

template <class T>
void mergeSortCall(T& container) {
	mergeSort(container, 0, container.size()-1);
}

//mergesort
/*
* void mergesort(container, l,r)
*	if (l==r) return;
*	middle = (l+r)/2;
*	mergesort(container, l,m);
*	mergesort(container, m+1, r);
*	merge(container, l,m,r);
*/