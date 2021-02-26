#include <iostream>
#include <array>
#include <string>
#include <algorithm>
#include <utility>
#include "Ch2_1.cpp"
#include "Ch2_3.cpp"
#include "LambdaDemo.h"
#include "Ch2_4.cpp"

using namespace std;

template <class Container>
void showArray(const Container& container) {
	for (auto elem : container) {
		cout << elem << " ";
	}
}

//exercise 2_1-1,2
void ex2_1_1and_2()
{
	cout << __FUNCTION__ << endl;
	array<uint32_t, 6> toSort{ 31,41,59,26,42,58 };
	cout << "Pre_sorted : ";
	showArray(toSort);
	cout << endl;
	insertionSort(toSort, [](decltype(toSort)::value_type const& left, decltype(toSort)::value_type const& right)->bool { return left < right; });
	cout << "Post-sorted : ";
	showArray(toSort);
	cout << endl;
	cout << "Worst case scenario for insertion sort: resort a sorted array in the opposite direction" << endl;
	insertionSort(toSort, [](decltype(toSort)::value_type const& left, decltype(toSort)::value_type const& right)->bool { return !(left < right); });
	cout << "Resorted : ";
	showArray(toSort);
	cout << endl;

}

void ex2_1_4() {
	cout << __FUNCTION__ << endl;
	constexpr size_t N = 8;//8 bit
	array<char, N> lhs{ 1,1,1,1,1,1,1,1 };
	array<char, N> rhs{ 0,0,0,0,0,0,0,1 };
	array<char, N + 1> result{ 0 };
	char carry{ 0 };
	//I know this defeats the purpose of the exercise. however, knowing how transform works shows understanding of the solutionn
	transform(lhs.crbegin(), lhs.crend(), rhs.crbegin(), result.rbegin(), [&carry](char l, char r)->char {
		auto result = (l + r + carry);
		carry = result / 2;
		return result %2;
		});
	result[0] = carry;

	cout << "Result ";
	for (auto i : result) {
		cout << static_cast<int>(i);
	}
	cout << endl;
}

template <class Container> void mergeSortExerciseHelper(Container& container) {
	cout << "Before sorting ";
	showArray(container);
	mergeSortCall(container);
	cout << "\nAfter sorting ";
	showArray(container);
	cout << "\n";
}

void mergeSortExercise() {
	cout << "Merge Sort Divide-And-Conquer\n";
	uint32_t data[] = { 23u, 12u, 13u, 0u, 100u};
	constexpr std::size_t numElem = sizeof(data) / sizeof(data[0]);
	array<uint32_t, numElem> toMerge{ 0 };
	for (size_t i = 0; i < toMerge.size(); ++i)
	{
		toMerge.at(i) = data[i];
	}

	mergeSortExerciseHelper(toMerge);
	vector<uint32_t> toMergeVector{ 23u, 12u, 13u, 0u, 100u };
	cout << "\nMerge sort using vector instead of array\n";
	mergeSortExerciseHelper(toMergeVector);
	cout << endl;
}

void maximumSubarrayExercise() {
	vector<int> prices{ 100, 113, 110, 85, 105, 102, 86, 63, 81, 101, 94, 106, 101, 79, 94, 90, 97 };
	cout << "Figure 2.4.1 stock price of Volatile Chemical Co" << endl;
	cout << "Prices from day 1 to day " << prices.size() << endl;
	for (auto price : prices)
	{
		cout << price << " ";
	}
	cout << endl;
	pair<size_t, size_t> maxPrices{ maximumSubarrayBruteForce(prices) };
	cout << "Maximum profit when buying at day " << maxPrices.first << " at " << prices[maxPrices.first];
	cout << " And sell at day " << maxPrices.second << " at " << prices[maxPrices.second] << " realizing the profit of " << prices[maxPrices.second] - prices[maxPrices.first] << endl;
	
	cout << "Using transformation-based algorithm:\n";
	auto refined = maximumSubarrayUsingTransformation(prices);
	auto const buyDate = std::get<0>(refined)-1;
	auto const sellDate = std::get<1>(refined) - 1;
	auto const gain = std::get<2>(refined) - 1;
	cout << "Maximum profit when buying at day " << buyDate << " at " << prices[buyDate];
	cout << " And sell at day " << sellDate << " at " << prices[sellDate] << " realizing the profit of " << gain << endl;
}

int main()
{
	ex2_1_1and_2();
	ex2_1_4();
	mergeSortExercise();
	lambdaDemo();
	maximumSubarrayExercise();
	return 0;
}