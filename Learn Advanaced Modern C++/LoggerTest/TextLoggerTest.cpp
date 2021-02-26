#include "TextLogger.h"
//as ugly as this looks like, including the cpp reduces code bloat because I can instantiate the template during compilation on-demand, instead of trying to anticipate
//the template instantiation types
#include "Logger.cpp"
#include <string>
#include <thread>
#include <random>

using namespace std;
//@todo Move to a google test framework
void textLoggerTestSingleThreaded()
{
	//Somewhere in the project that uses TextLogger, in the global scope, only one instance of TextLogger will be instantiated and initialized
	//I am pushing this to the responsibility of the user
	TextLogger textLogger("test.log");
	enum {
		NUM_LOOP=10
	};

	const string testEntry{ string{__FUNCSIG__} + string{" Logging Entry"} };

	for (uint32_t count = 0; count < NUM_LOOP; ++count) {
		textLogger.log(TextLogger::createTimestampedEntry(testEntry));
	}
}

template class Logger<TextLogger>;

void loggerTestSingleThreaded()
{
	Logger<TextLogger> logger("singleThread.log");
	enum {
		NUM_LOOP = 20
	};

	const string testEntry{ "Testing single threaded log" };

	//dont bother seeding. I need repeatable test case
	mt19937 engine;
	const uniform_int_distribution randomTime{ 1000, 20000 };
	for (uint32_t i = 0; i < NUM_LOOP; ++i)
	{
		logger.log(string{ testEntry } + to_string(i));
		this_thread::sleep_for(chrono::milliseconds(randomTime(engine)));
	}
	//The output should be sequential from 0-19
	//Just verify by eye
}