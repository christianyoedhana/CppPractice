#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <array>
#include "AlgorithmExercise.h"

using namespace std;

void sortingExercise()
{
	class SortByID
	{
	public:
		using IDType = uint32_t;	//Either this or a typedef. C++ 11 encourages the keyword using for type alias instead of defining a whole new type
		SortByID() = default;
		//Lets do implicit conversion
		SortByID(IDType id) : m_id{ id } {}
		bool operator<(const SortByID& lhs)	//Why can't we just this overload?
		{
			return m_id < lhs.m_id;
		}
		
		IDType id() const
		{
			return m_id;
		}

	private:
		IDType m_id;	//4 billion ids. Not enough for the entire earthlings.

	};

	vector<SortByID> items{ 3,2,4, 0, 333, 124, 6, 1 };
	//just to be fancy
	ostream_iterator<SortByID::IDType> outIter(cout, " ");
	cout << "Sorting exercise for :";
	for (const auto& id : items)
	{
		*outIter++ = id.id();
	}
	cout << endl;
	sort(items.begin(), items.end());
	cout << "Sorted ids :";
	for (const auto& id : items)
	{
		*outIter++ = id.id();
	}

	//Just curious if implicit conversion kicks in here. Uhh nope!
	//auto smallest = min<SortByID>( { 3,2,4, 0, 333, 124, 6, 1 } );
	//cout << "The smallest ID is " << smallest.id() << endl;
	cout << endl;
}

void readAlgorithmExercise()
{
	//how do I simulate find_all string that contains a certain substring? I mean we can do this using stl regex, but this is a fun exercise
	//incorportaing find_if and functors
	vector<string> input{ "Los Angeles", "Pendleton", "Football", "Oatmeal", "Le Bourget", "Lottery" };
	cout << "Exercise using find_if over the following input: ";
	for (const auto& str : input)
	{
		cout << str << ' ';
	}
	cout << endl;

	//Find all input that contains substring "le"
	class find_substring
	{
	public:
		find_substring() = delete;
		explicit find_substring(const string& substr) : m_substr{ substr } {}
		//This is an exact match search.
		bool operator()(const string& toSearch) const
		{
			return toSearch.find(m_substr) != string::npos;
		}

		const string& searchTerm() const
		{
			return m_substr;
		}

	private:
		string m_substr;
	};

	find_substring findLE("le");
	cout << "Looking for all string containing the substring " << findLE.searchTerm() << endl;
	auto iterStart = input.cbegin();
	while ((iterStart = find_if(iterStart, input.cend(), findLE)) != input.cend())
	{
		cout << *iterStart++ << " ";
	}

	//now the search predicate can be tailored easily using a lambda, with a more limited search option
	iterStart = input.cbegin();
	cout << "Ad-hoc looking for all string containing a substring defined in a lambda. In this case the string et" << endl;
	while ((iterStart = find_if(iterStart, input.cend(), [](const string& toSearch){ return toSearch.find("et") != string::npos; })) != input.cend())
	{
		cout << *iterStart++ << " ";
	}
	cout << endl;
}

template <typename Lambda>
class SimpleScopeGuard
{
public:
	SimpleScopeGuard() = delete;
	//Currently in violation of rule 5. I just want to see a scope guard demo using lambdas
	explicit SimpleScopeGuard(Lambda&& func) : m_destructorFunc{ std::move(func) } {}
	~SimpleScopeGuard()
	{
		m_destructorFunc();
	}
private:
	Lambda m_destructorFunc;
};

void lambdaExercise()
{
	cout << "Lambda exercise " << endl;
	SimpleScopeGuard aScopeGuard([]() {cout << "Smile, You Have Been Erased" << endl;});
}

void minMaxExercise()
{
	cout << "MinMaxExercise" << endl;
	auto smallest = min({ 5,6,7,1,3,4,2,9 });
	cout << "Min says the smallest is " << smallest << endl;
}

void writeOnlyExercise()
{
	//Using gnereate to generate fibonacci number
	cout << "Using generate() to generate a fibonacci sequence :";
	array<uint32_t, 20> fibonacci;
	uint32_t fibBuffer[2] = { 0,1 };
	generate(fibonacci.begin(), fibonacci.end(), [&fibBuffer]() { uint32_t retVal = fibBuffer[0]; fibBuffer[0] = fibBuffer[1]; fibBuffer[1] += retVal; return retVal; });
	for (auto num : fibonacci)
	{
		cout << num << " ";
	}
	cout << endl;

	vector<uint32_t> empty;
	fill_n(back_inserter(empty), 20, 0);
	cout << "Now there will be 20 zeros, eventhough the vector starts empty" << endl;
	for (auto num : empty)
	{
		cout << num << " ";
	}
	cout << endl;
}

void replaceExercise()
{
	cout << "Replace exercise with the original string: " << endl;
	string quickFox{ "The quick brown fox jumps over the brown wall. The fox then jumps over the same wall back from where it has started." };
	//Using replace to replace all spaces into underscrore. I know, string has functions for this. Just demonstrating the potential implemetation of the substring replacement
	cout << quickFox << endl;
	//now lets copy and Sentence Case each word
	string sentenceCaseQuickFox;
	//I wonder if this is more efficient if you have to do the copying 1 million times
	sentenceCaseQuickFox.reserve(quickFox.size() + 1);
	cout << "Sentence-casing the quick fox " << endl;
	class SentenceCase
	{
	private:
		typename string::value_type m_whitespace = ' ';
		bool m_first = true;
		bool m_whitespaceEncountered = false;
	public:
		typename string::value_type operator()(string::value_type eval)
		{
			//static bool whitespaceEncountered = eval == m_whitespace;	//not worried about parallel execution
			if (m_first && eval != m_whitespace)
			{
				m_first = false;
				return toupper(eval);
			}

			if (eval == m_whitespace) {
				m_whitespaceEncountered = true;
			}
			else
			{
				if (m_whitespaceEncountered)
				{
					m_whitespaceEncountered = false;
					return toupper(eval);
				}
			}
			return eval;
		}
	};

	transform(quickFox.cbegin(), quickFox.cend(), back_inserter(sentenceCaseQuickFox), SentenceCase{});
	cout << sentenceCaseQuickFox << endl;
	cout << "Now replace all spaces with underscores, still maintaining the original length of the string" << endl;
	replace(quickFox.begin(), quickFox.end(), ' ', '_');
	cout << quickFox << endl;
	//A string is essentially a vector of characters, so the old and new value types must match the iterator element type.
	class ReplaceAfterWhiteSpace 
	{
	private:
		typename string::value_type m_whitespace = ' ';
		bool m_first = true;
		bool m_whitespaceEncountered = false;
	public:
		ReplaceAfterWhiteSpace() = default;
		explicit ReplaceAfterWhiteSpace(string::value_type whitespace) : m_whitespace(whitespace) {}
		bool operator()(typename string::value_type eval)
		{
			//static bool whitespaceEncountered = eval == m_whitespace;	//not worried about parallel execution
			if (m_first && eval != m_whitespace)
			{
				m_first = false;
				return true;
			}
						
			if (eval == m_whitespace) {
				m_whitespaceEncountered = true;
			}
			else
			{
				if (m_whitespaceEncountered)
				{
					m_whitespaceEncountered = false;
					return true;
				}
			}
			return false;
		}
	};

	ReplaceAfterWhiteSpace afterUnderscore('_');
	//If Elmer Fudds were to recite the quickFox line
	replace_if(quickFox.begin(), quickFox.end(), afterUnderscore, 'W');
	cout << "If Elmer Fudds were to recite the quickFox line, it will sound like this" << endl;
	cout << quickFox << endl;
	/*The functor object gets copied by value, not reference, so these test cases are useless
	//Now lets change the underscrore back to space. Let see if we can use the same predicate
	replace(quickFox.begin(), quickFox.end(), '_', ' ');
	ReplaceAfterWhiteSpace afterSpace(' ');
	replace_if(quickFox.begin(), quickFox.end(), afterSpace, 'w');
	cout << quickFox << endl;
	//The UnaryPredicate better be cheap to copy, because replace_if passes the UnaryPredicate type by value!
	//Helps that the original object is not modified, so replace_if calls are always using the predicate in the same beginning state.
	replace_if(quickFox.begin(), quickFox.end(), afterSpace, 'F');
	cout << quickFox << endl;
	*/
}

void removeExercise()
{
	cout << "Remove does not shorten the container object, it merely move the removed member to an inaccessible area, then turn these values undefined." << endl;
	//Need to see good use cases of remove without erase.
	string quickFox{ "The quick brown fox jumps over the brown wall. The fox then jumps over the same wall back from where it has started." };
	cout << quickFox << " SIZE = " << quickFox.size() << endl;
	//remove all vowel
	auto defunct = remove_if(quickFox.begin(), quickFox.end(), [](string::value_type c) { return string{ "AEIOU" }.find(toupper(c)) != string::npos; });
	cout << quickFox << " SIZE = " << quickFox.size() << endl;
	//for_each(defunct, quickFox.end(), [](string::value_type c) {cout << c; });//not getting the removed vowels.
	cout << endl;
	quickFox.erase(defunct, quickFox.end());
	cout << quickFox << " SIZE = " << quickFox.size() << endl;
}

void copyExercise()
{
	cout << "So what happen if we try to copy too many elements?" << endl;
	vector<int> toCopy{ 1,2,3,4,5,6,7,8,9 };
	vector<int> target;
	class SizeOrConstLimit
	{
	public:
		size_t operator()(const vector<int>& container, size_t limit) {
			return (container.size() < limit) ? container.size() : limit;
		}
	};
	try {
		//This won't compile
		copy_n(toCopy.begin(), [&toCopy]()->size_t { return (toCopy.size() < 12) ? toCopy.size() : 12; }() //12 will trip a debug-assert
		//copy_n(toCopy.begin(), SizeOrConstLimit{}(toCopy, 12) //12 will trip a debug-assert
			, back_inserter(target));
		for_each(target.begin(), target.end(), [](int i) {cout << i << ' '; });
		cout << endl;
	}
	catch (...) {}
}