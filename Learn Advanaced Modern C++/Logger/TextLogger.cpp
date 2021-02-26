/*
Logger is a text logger.
User specifies the complete file path and name in the constructor.
Logger constructor must be non-converting and take a std::string
User enters the log using a function "log" that takes a std::string
Logger does not provide a timestamp entry. User is responsible for giving a complete string to save, including timestamps if necessary. This is to ensure that
the user can create a timestamped log entry as close as possible to the incident
"log" must be as lock-free as possible to the caller of log
*/
#include "TextLogger.h"
#include <chrono>
using namespace std;

//Text file output only
TextLogger::TextLogger(const string& fileName) : m_file{ fileName } {}

void TextLogger::log(const string& entry) {
	m_file << entry;
}

string TextLogger::createTimestampedEntry(const string& entry) {
	return string{ string{"Time to be implemented"} + string{" "} + entry };
}