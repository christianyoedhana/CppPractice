#include "RandomPermutationInPlace.h"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

void randomPermutationTest()
{
	using namespace std;
	using IndexType = typename vector<int>::size_type;
	using ValueType = typename vector<int>::value_type;

	constexpr IndexType N = 20;
	vector<int> testData(N);
	mt19937 engine;	//Not seeded for debugging, so you can have a consistent test data.
	constexpr int range = 100;
	uniform_int_distribution<> generator(0, range);
	generate(testData.begin(), testData.end(), [&]() { return generator(engine); });
	cout << "Generating random permutation for sequence : " << endl;
	for_each(testData.cbegin(), testData.cend(), [](auto item) { cout << item << " "; });
	cout << endl;
	cout << "Result :" << endl;
	for (IndexType i = 0; i < N; ++i) {
		randomPermutationInPlace(testData);
		for_each(testData.cbegin(), testData.cend(), [](auto item) { cout << item << " "; });
		cout << endl;
	}
}

int main() {
	randomPermutationTest();
}