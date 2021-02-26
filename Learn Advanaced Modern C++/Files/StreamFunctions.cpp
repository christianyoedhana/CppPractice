#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>

using std::cout;
using std::ostringstream;
using std::setw;
using std::left;
using std::endl;
using std::ifstream;

bool streamFunctionsExercise()
{
	cout << std::left<< std::setw(10) << "Panda";
	cout << 20 << std::endl;
	
	cout << std::setw(10) << "Orangutan";
	cout << 102 << std::endl;
	return true;
}

bool stringStreamExercise()
{
	struct Header {
		int m_length;
		int m_crc;
		ostringstream strstream()
		{
			ostringstream ost;
			ost << left << setw(20) << std::setfill('-') << m_length;
			ost << setw(20) << std::setfill('-') << m_crc<<endl;
			return ost;
		}
	};
	Header header{ sizeof(int), 123456789 };
	ostringstream ost;
	ost<<header.strstream().str();
	ost << left << setw(12) << "Panda " << 10 << endl;
	ost << setw(12) << "Polar Bear" << 101 << endl;
	ost << setw(12) << "Orangutan " << 6 << endl;
	cout << "Formatted string stream exercise" << endl;
	cout << ost.str();
	return true;
}

void udemySStreamExercise()
{
	const std::string fileName("data_is.txt");
	ifstream file(fileName);
	if (!file)
	{
		cout << "Cannot find " << fileName << endl;
		return;
	}
	cout << "Reading file buffer and dumping the content to a vector object" << endl;

	//sstream << file.rdbuf();
	std::string streamLine;
	while (getline(file, streamLine))
	{
		std::stringstream sstream;
		sstream << streamLine;
		std::vector<int> temp;
		int value = 0;
		while (sstream >> value)
		{
			temp.push_back(value);
		}

		for (auto val : temp)
		{
			cout << val << ' ';
		}
		cout << endl;
	}

	std::string stringLine;
	std::ios_base::iostate fileState = file.rdstate();
	if (fileState & std::ios_base::failbit)
	{
		cout << "Somehow failbit is set" << endl;
	}
	if (fileState & std::ios_base::badbit)
	{
		cout << "Somehow badbit is set" << endl;
	}
	if (fileState & std::ios_base::eofbit)
	{
		cout << "EOF set. This is expected" << endl;
	}

	file.clear();	//why is this still necessary? Because failbit is set. Failbit is set because our last call to getline fails. seekg only promises to clear the eofbit
	//so while failbit is set, our call to getline automatically fails.
	file.seekg(0);
	
	cout << "Reading file one line at a time and storing the line in a string object" << endl;
	
	while (getline(file, stringLine))
	{
		std::istringstream instream(stringLine);
		std::ostringstream outstream;
		int value = 0;
		while (instream >> value)	//I think this is the point of the exercise. Of course we can use a bidirectional stringstream
		{
			outstream << value<<' ';
		}
		cout << outstream.str() << endl;
	}
	cout << endl;
}

void udemySStreamExerciseAnswers()
{
	ifstream infile("data_is.txt");
	std::ios_base::iostate fileState = infile.rdstate();
	if (fileState & std::ios_base::failbit)
	{
		cout << "Somehow failbit is set" << endl;
	}
	else {
		cout << "File state is OK" << endl;
	}
	if (infile) {
		std::string line;
		std::vector<int> numbers;

		while (getline(infile, line)) {
			std::istringstream is(line);
			int num;
			while (is >> num) {
				numbers.push_back(num);
			}
		}
		for (auto n : numbers) {
			cout << n << endl;
		}
		std::ios_base::iostate fileState = infile.rdstate();
		if (fileState & std::ios_base::failbit)
		{
			cout << "Somehow failbit is set" << endl;
		}
	}
	else {
		cout << "Could not open data_is.txt" << endl;
	}

}
