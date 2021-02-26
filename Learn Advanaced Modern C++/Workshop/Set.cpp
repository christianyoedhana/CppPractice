#include <set>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

class DistinctWords
{
public:
	DistinctWords() = delete;
	explicit DistinctWords(istream& inputStream)
	{
		string line;
		getline(inputStream, line);
		addWords(line);
	}

	explicit DistinctWords(ifstream& file)
	{
		string line;
		while (getline(file, line))
		{
			addWords(line);
		}
	}

	inline friend ostream& operator<<(ostream& out, const DistinctWords& words)
	{
		for_each(words.m_distinctWords.cbegin(), words.m_distinctWords.cend(), [&out](const string& key) { out << key << " "; });
		out << "-->Word Count: " << words.m_distinctWords.size();
		return out;
	}

private:
	set<string> m_distinctWords;
	void addWords(const string& line)
	{
		istringstream input(line);
		istream_iterator<string> inputStream(input);	//get rid of the whitespace
		istream_iterator<string> eof;
		set<string> distinctWords;
		copy(inputStream, eof, inserter(m_distinctWords, m_distinctWords.begin()));
	}
};

void readDistinctWords()
{
	cout << "Please type a sentence. When you are done, press enter :";
	DistinctWords distinctWords(cin);
	
	cout << "The distinct words entered by the user are :" << distinctWords<<endl;
}

void readDistinctWordsFromFile()
{
	cout << "Reading unique words from words.txt" << endl;
	ifstream inputFile("words.txt");
	DistinctWords distinctWords(inputFile);
	cout << "Unique worods from words.txt are " << distinctWords << endl;
}