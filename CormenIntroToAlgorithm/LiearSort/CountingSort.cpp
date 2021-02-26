#include <iostream>
#include <array>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

template <class T>
std::vector<typename T::size_type> countingSort(T const& input, T& output, typename T::size_type NumRange) {
	using namespace std;
	vector<typename T::size_type> counter(NumRange+1, 0);
	//count distinct elem in input
	for_each(input.cbegin(), input.cend(), [&counter](auto item) { counter[item] += 1; });
	//compute adjacent sum
	for (typename T::size_type index = 1; index < counter.size(); ++index) {
		counter.at(index) += counter.at(index - 1);
	}
	//create a copy for the return value;
	auto retVal(counter);
	//now use counter to create the output
	for_each(input.crbegin(), input.crend(), [&counter, &output](auto item) { counter.at(item) -= 1; output.at(counter.at(item)) = item;  });
	return retVal;
}

void countingSortTest() {
	using namespace std;
	constexpr int NumRange = 6; //Number range from 0-NumRange
	random_device seed;
	mt19937 engine(seed());
	uniform_int_distribution generator{ 0, NumRange };
	constexpr int N = 30;	//input and output size > NumRange. I want to see repetition
	array<int, N> input;
	generate_n(input.begin(), N, [&]() { return generator(engine); });
	array<int, N> output{ 0 };
	
	//counting sort is not an in-place sorting algorithm
	cout << "Input: " << endl;
	for_each(input.cbegin(), input.cend(), [](auto& elem) {cout << elem << " "; });
	auto counter = countingSort(input, output, NumRange);
	cout << "\nSorted: " << endl;
	for_each(output.cbegin(), output.cend(), [](auto& elem) {cout << elem << " "; });
	cout << "\nCounter Vector: " << endl;
	for_each(counter.cbegin(), counter.cend(), [](auto& elem) {cout << elem << " "; });

	//proof that the algorithm preserve its order,use this indexed string class and show that the order of the text is preserved
	class IndexedString {
		size_t m_index;
		string m_string;
	public:
		IndexedString(size_t index = 0, const string& str = string{}) : m_index(index), m_string(str) {}
		size_t index() const { return m_index; }
		string content() const { return m_string; }
		size_t& index() { return m_index; }
		string& content() { return m_string; }
		//This is crucial. Allows implicit conversion to size_t so IndexedString can be used as an array index
		operator size_t&() { return m_index; }
		operator size_t() const { return m_index; }
	};

	constexpr auto NumIndexedString = 10u;
	array<IndexedString, NumIndexedString> punchCards;
	for (auto index = 0; index < punchCards.size(); ++index) {
		punchCards[index].index() = generator(engine);
		punchCards[index].content() = to_string(index);
	}
	cout << "Unsorted punch card input order :" << endl;
	for_each(punchCards.cbegin(), punchCards.cend(), [](auto const &card) { cout << "Card :" << card.index() << " content :" << card.content() << endl; });
	array<IndexedString, NumIndexedString> sortedPunchCards;
	auto punchCardCounter = countingSort(punchCards, sortedPunchCards, NumRange);
	cout << "Sorted punch card output order :" << endl;
	for_each(sortedPunchCards.cbegin(), sortedPunchCards.cend(), [](auto const& card) { cout << "Card :" << card.index() << " content :" << card.content() << endl; });
}