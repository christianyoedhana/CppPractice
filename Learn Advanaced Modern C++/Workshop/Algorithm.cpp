#include <vector>
#include <iostream>
#include <iterator>
#include <random>
#include <chrono>
#include <numeric>
#include <sstream>
#include <fstream>
#include <map>

#include "Algorithm.h"

using namespace std;
//Singleton, should be obvious
template <typename T> T& seededRandomNumber()
{
	static T generator(std::chrono::system_clock::now().time_since_epoch().count());
	return generator;
}

template <typename DataType>
std::vector<DataType> generateRandomNumbers(size_t n, DataType low, DataType high)
{
	//Every pseudorandom number generator needs a seed. Traditionally, since I learn C in 1996, time(0) has always been the preferred seed
	//now this is the seed in C++
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	//@todo use the numeric sizeof length as a policy to generate the random number generator code. 4 generates 32-bit random number generator, otherwise use 64 by default
#ifdef _M_X64
	std::mt19937_64 randGenerator(seed);
#else
	std::mt19937 randGenerator(seed);
#endif
	std::vector<DataType> retVal(n);
	std::generate(retVal.begin(), retVal.end(), [=, &randGenerator]() { return static_cast<DataType>((randGenerator() % (high + 1)) + low); });
	return retVal;
}

template < typename T > stringstream showData(const T& container)
{
	stringstream sstream;
	all_of(container.cbegin(), container.cend(), [&sstream](typename T::value_type num) { sstream << num << ' '; return true; });
	return sstream;
}

//exercise 1&2 is the constructor
AlgorithmWorkshop::AlgorithmWorkshop(): m_randomNumbers(generateRandomNumbers<uint32_t>(10, 0, 1000))
{
	cout<<showData(m_randomNumbers).str()<<endl;
}

void AlgorithmWorkshop::exercise3_4()
{
	auto iterator = max_element(m_randomNumbers.cbegin(), m_randomNumbers.cend());
	size_t index = iterator - m_randomNumbers.cbegin();
	m_max = m_randomNumbers[index];
	cout << "Largest number is " << m_max << " at index " << index << endl;
}

void AlgorithmWorkshop::exercise5()
{
	uint32_t sum = accumulate(m_randomNumbers.cbegin(), m_randomNumbers.cend(), 0);
	cout << "SUM = " << sum << endl;
}

void AlgorithmWorkshop::exercise6()
{
	vector<double> normalized;
	transform(m_randomNumbers.cbegin(), m_randomNumbers.cend(), back_inserter(normalized), [=](uint32_t num)->double { return (num * 1.0) / m_max; });
	cout << showData(normalized).str() << endl;
}

void AlgorithmWorkshop::exercise7()
{
	//counting odd numbers in original vector. Return type is a signed integral, length unknown
	auto howManyOdds = count_if(m_randomNumbers.cbegin(), m_randomNumbers.cend(), [](auto num) { return num % 2 == 1; });
	cout << "Number of odd random numbers = " << howManyOdds << endl;
}

void AlgorithmWorkshop::exercise8()
{
	//now sort randomNumbers
	copy(m_randomNumbers.cbegin(), m_randomNumbers.cend(), back_inserter(m_sortedAscending));
	sort(m_sortedAscending.begin(), m_sortedAscending.end());
	const uint32_t searchNum = 456;
	auto location = lower_bound(m_sortedAscending.cbegin(), m_sortedAscending.cend(), searchNum);
	vector<uint32_t> searchResult;
	copy(location, m_sortedAscending.cend(), back_inserter(searchResult));
	cout << showData(searchResult).str() << endl;

	if (location != m_sortedAscending.cend())
	{
		cout << "There are " << m_sortedAscending.size() - (location - m_sortedAscending.cbegin()) << " numbers greater than " << searchNum - 1 << endl;
	}
	else
	{
		cout << "Every numbers are " << searchNum - 1 << " or less" << endl;
	}
}

void AlgorithmWorkshop::exercise9()
{
	vector<double> oddNumbers;
	copy_if(m_sortedAscending.cbegin(), m_sortedAscending.cend(), back_inserter(oddNumbers), [](uint32_t num) { return num % 2 == 1; });
	cout<<showData(oddNumbers).str()<<endl;
	cout << endl;
}

void AlgorithmWorkshop::exercise10()
{
	//This is quicksort worst case scenario
	m_sortedDescending = m_randomNumbers;
	sort(m_sortedDescending.begin(), m_sortedDescending.end(), [](double lhs, double rhs) { return lhs >= rhs; });
	cout << showData(m_sortedDescending).str() << endl;
}

void AlgorithmWorkshop::exercise11()
{
	m_shuffled = m_sortedDescending;
	shuffle(m_shuffled.begin() + 1, m_shuffled.end() - 1, seededRandomNumber<mt19937_64>());
	cout << showData(m_shuffled).str() << endl;
}

template <typename DataType> void saveAsCSV(const string& fileName, const vector<DataType>& values)
{
	if (values.size() == 0) return;
	ofstream file(fileName);
	if (!file) return;
	//Problems like this always require 1 less operation than the existing data points to operate on.
	//THERE ARE NO ALGORITHM THAT DOES THIS IN C++ AS OF 2020.
	ostream_iterator<DataType> trailingComma(file, ",");
	copy(values.cbegin(), values.cend() - 1, trailingComma);
	ostream_iterator<DataType> lastElem(file);
	copy(values.cend() - 1, values.cend(), lastElem);
}

void AlgorithmWorkshop::exercise12_13()
{
	//Ensures that I am instantiating the same type;
	auto evenOnly{ m_shuffled };
	evenOnly.erase(remove_if(evenOnly.begin(), evenOnly.end(), [](auto number) { return number % 2 == 1; }), evenOnly.cend());
	cout<<showData(evenOnly).str()<<endl;
	saveAsCSV("Exercise13.csv", evenOnly);
}

class Exercise14_18
{
public:
	explicit Exercise14_18(const string& fileName) : m_file{ fileName }
	{
		processFile();
	}
	Exercise14_18() = delete;

	void showWordCount()
	{
		for_each(m_wordCount.cbegin(), m_wordCount.cend(), [](typename decltype(m_wordCount)::value_type content) { cout << content.first << ":" << content.second << endl; });
	}
	void showLineCount()
	{
		cout << "Line Count " << m_lineCount << endl;
	}

	void showCharCount()
	{
		cout << "Character Count " << m_charCount << endl;
	}

	void showCommonWords(const Exercise14_18& other)
	{
		typedef typename decltype(m_wordCount)::value_type PairType;
		//This works because I don't care about the word count. It is double the size because, well, I just have to count the word occurences
		//if the assignment ask for the counts, then I need to roll out my own solution
		vector<PairType> commonWords;
		//OK I want to use the stl::set_intersect, eventhough the naive implementation of a sorted data intersection does not seem too bad
		set_intersection(m_wordCount.cbegin(), m_wordCount.cend(), other.m_wordCount.cbegin(), other.m_wordCount.cend(), back_inserter(commonWords),
			[](const PairType& lhs, const PairType& rhs) { return lhs.first < rhs.first; });
		for_each(commonWords.cbegin(), commonWords.cend(), [](const PairType& tuple) {cout << tuple.first << ' '; });
		cout << endl;
	}
	

private:
	fstream m_file;
	size_t m_charCount = 0;
	size_t m_lineCount = 0;
	class CaseSensitiveLessThan
	{
	public:
		bool operator()(const string& lhs, const string& rhs) const
		{
			return lowercase(lhs) < lowercase(rhs);
		}
	private:
		string lowercase(const string& source) const
		{
			string result;
			transform(source.cbegin(), source.cend(), back_inserter(result), [](string::value_type c) { return tolower(c); });
			return result;
		}
	};

	map < string, uint32_t> m_wordCount;

	void processFile()
	{
		string line;
		//I get line count here, at the expense of extra string
		while (getline(m_file, line))
		{
			++m_lineCount;
			m_charCount += line.size();
			istringstream stream(line);
			istream_iterator<string> words(stream);
			//vector<string> individualWords(words, istream_iterator<string>()); //initialize a vector using a data range from iter begin to end.
			vector<string> individualWords;
			transform(words, istream_iterator<string>(), back_inserter(individualWords),
				[](const string& word)->string {
					string out;
					for_each(word.cbegin(), word.cend(), [&out](unsigned char c) { if (isalpha(c)) out.push_back(tolower(c)); });
					return out; }
			);
			//now add each words to the map
			//now Im going lambda crazy.
			for_each(individualWords.cbegin(), individualWords.cend(),
					[this](const string& key) {
						auto iter = m_wordCount.find(key);
						if (iter == m_wordCount.end()) {
							m_wordCount.insert({ key, static_cast<uint32_t>(1) });
							return;
						}
						iter->second += 1;
					});
		}
	}
};

void exercise14_18(const string& first, const string& second)
{
	Exercise14_18 firstFile(first);
	firstFile.showWordCount();
	firstFile.showLineCount();
	firstFile.showCharCount();
	Exercise14_18 secondFile(second);
	secondFile.showCommonWords(firstFile);
}

void palindrome()
{
	string inputLine;
	cout << "Palindrome Evaluator. Enter a sentence: ";
	getline(cin, inputLine);
	string inputCopy = inputLine;
	inputCopy.erase(remove_if(inputCopy.begin(), inputCopy.end(), [](string::value_type c) { return !isalpha(static_cast<unsigned char>(c)); }), inputCopy.end());
	string inputReverse = inputCopy;
	reverse(inputReverse.begin(), inputReverse.end());
	cout << inputLine << " is ";
	size_t firstMismatch{ 0 };	//The position of the first alphanumeric mismatch, as an alphanumeric count
	bool aPalindrome = equal(inputCopy.begin(), inputCopy.end(), inputReverse.begin(), inputReverse.end(), [&firstMismatch](auto lhs, auto rhs) { ++firstMismatch; return tolower(lhs) == tolower(rhs); });
	if (!aPalindrome)
	{
		cout << "NOT";
	}
	cout << " a Palindrome."<<endl;
	//now find the firstMistmatch alphabet using the firstMistmatch positions
	if (!aPalindrome)
	{
		cout << "The first mismatched character is " << inputCopy[firstMismatch-1];
		auto mismatchPos = find_if(inputLine.cbegin(), inputLine.cend(), [&firstMismatch](auto c) { if (isalpha(c)) --firstMismatch; return firstMismatch == 0; });
		cout << " right after :";
		for_each(inputLine.cbegin(), mismatchPos, [](auto c) {cout << c; });
		cout << endl;
	}
}

void permutation()
{
	cout << "Enter a string to display its permutation :";
	string input;
	getline(cin, input);
	while (next_permutation(input.begin(), input.end()))
	{
		cout << input << endl;
	}
}

void algorithmWorkshop()
{
	cout << "Algorithm Workshop" << endl;
	/*AlgorithmWorkshop workshop;
	workshop.exercise3_4();
	workshop.exercise5();
	workshop.exercise6();
	workshop.exercise7();
	workshop.exercise8();
	workshop.exercise9();
	workshop.exercise10();
	workshop.exercise11();
	workshop.exercise12_13();*/
	//exercise14_18("words.txt", "words2.txt");
	palindrome();
	permutation();
}