#include "BinarySearch.h"

template<class T>
bool binarySearch(const T& sortedContainer, typename T::value_type searchTerm, typename T::size_type& foundAt) {
	if (sortedContainer.empty()) return false;
	bool found = false;
	using Index = typename T::size_type;
	Index left = 0;
	Index right = sortedContainer.size() - 1;
	while (!found) {
		Index middle = (left + right) / 2;
		auto midItem = sortedContainer.at(middle);
		if (midItem == searchTerm) {
			found = true;
			foundAt = middle;
			continue;
		}

		if (left == right) {	//done checking, item not found
			return found;
		}

		if (searchTerm < midItem) {
			right = middle - 1;
		}
		else {
			left = middle + 1;
		}
	}
	return found;
}