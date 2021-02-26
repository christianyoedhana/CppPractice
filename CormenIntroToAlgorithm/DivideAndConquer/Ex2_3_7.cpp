/*Let the sum = S
To get the required O(n lgn) performance:
1. Sort the sequence in ascending order. This should be an O(n lg n) operation
2. Find the first integer > than the sum. Exclude everything from here to the right. call this N
3. Starting from the first element to N, subtract S from the current element. Then from the next element to N, binary search the sublist for the subtracted value
4. If found, add the indices to a vector of pairs.*/
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>

using namespace std;

template <class T>
vector < pair<typename T::value_type, typename T::value_type>> findPairs(const T& toSearch, typename T::value_type sum) {
	T searchSet(toSearch);
	sort(searchSet.begin(), searchSet.end());
	using Index = typename T::size_type;
	using Value = typename T::value_type;
	vector<pair<Value, Value>> retVal;
	for (auto iter = searchSet.cbegin(); iter < searchSet.cend() - 1; ++iter) {
		auto diff = sum - *iter;
		auto candidate = lower_bound(iter + 1, searchSet.cend(), diff);
		if (candidate == searchSet.cend()) continue;
		if (*candidate == diff) {
			retVal.push_back(typename decltype(retVal)::value_type{ *iter, *candidate });
		}
	}
	return retVal;
}


void Ex2_3_7()
{
	mt19937 engine;
	constexpr int edge = 100;
	uniform_int_distribution<> generator{ -edge, edge };
	constexpr size_t N = 12;
	vector<int> intSet(N);
	generate(intSet.begin(), intSet.end(), [&]() {return generator(engine); });
	cout << "Search from :";
	for_each(intSet.cbegin(), intSet.cend(), [](auto item) {cout << item << " "; });
	int sum = 109;
	cout << "\n for 2 values that sums exactly " << sum << endl;
	auto intPairs = findPairs(intSet, sum);
	cout << "Result :" << endl;
	if (!intPairs.empty()) for_each(intPairs.cbegin(), intPairs.cend(), [](const auto& item) { cout << "(" << item.first << "," << item.second << ")\n"; });
	sum = 108;
	cout << "\n for 2 values that sums exactly " << sum << endl;
	auto noPairs = findPairs(intSet, sum);
	cout << "Result :" << endl;
	if (!noPairs.empty()) for_each(noPairs.cbegin(), noPairs.cend(), [](const auto& item) { cout << "(" << item.first << "," << item.second << ")\n"; });
}