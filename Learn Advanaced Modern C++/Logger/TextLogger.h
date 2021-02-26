#pragma once
#include <string>
#include <fstream>
#include <future>
#include <condition_variable>
#include <queue>
/*
TextLogger is an ASCII text logger. No support for double-byte character provided for now
User specifies the complete file path and name in the constructor.
TextLogger constructor must be non-converting and take a std::string
User enters the log using a function "log" that takes a std::string
TextLogger does not provide a timestamp entry. User is responsible for giving a complete string to save, including timestamps if necessary. This is to ensure that
the user can create a timestamped log entry as close as possible to the incident.
TextLogger provides a helper function to prepend a time stamp string into the text log entry
*/

class TextLogger {
	std::ofstream m_file;
	//Guarded producer thread queue containing the string. The producer queue returns the std::string contained in the thread-local copy of the string log entry
	//std::queue < std::future < std::string >> m_logEntries;
	//@TODO test-only
	//std::queue<std::string> m_logEntries;
	//One consumer worker thread working on the string log queue
	//std::thread m_workerThread;
public:
	using StringType = std::string;
	TextLogger() = delete;
	explicit TextLogger(const std::string& file);
	void log(const std::string& entry);
	static std::string createTimestampedEntry(const std::string& entry);
};
