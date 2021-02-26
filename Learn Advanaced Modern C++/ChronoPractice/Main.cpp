#include <chrono>
#include <iomanip>

using namespace std;

//OK chrono is not user-friendly.I have to figure out how to make the milliseconds out from chrono.
//I may have to subtract now from timeNow equivalent then convert to milliseconds representation
int main() {
	auto now = chrono::system_clock::now();
	auto timeNow = chrono::system_clock::to_time_t(now);
	localtime(&timeNow);
	return 0;
}