#include "Files.h"
#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;

bool fileIOExercise(const string& fileName)
{
	ifstream inputFile{ fileName };
	const string cloneFileName{ "Clone_" + fileName };
	ofstream outputClone{ cloneFileName };
	if (!inputFile) {
		cout << "Cannot open file " << fileName << std::endl;
		return false;
	}

	if (!outputClone) {
		cout << "Cannot create output file " << cloneFileName << std::endl;
		return false;
	}
	string fileLine;
	while (std::getline(inputFile, fileLine)) {
		cout << fileLine<<std::endl; //Flush buffer with std::endl
		outputClone << fileLine << std::endl;
	}

	return true;
}

//Write to fileName, then reads from the filename. Data should have the same values
bool binaryFileIOExercise(const string& fileName)
{
	struct BinStruct {
		char m_fourcc[4];
		uint16_t m_length : 16;
		uint32_t m_crc : 32;
		unsigned char m_byteBuffer[32];
	};
	return true;
}