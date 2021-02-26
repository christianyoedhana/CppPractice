#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <future>

#include "RaceConditionDemo.h"
using namespace std;

//Demonstrates how result value is not as expected when 2 thread stomp each other's write on a shared data
void raceConditionDemo(){
	cout << __FUNCSIG__ << endl;
	uint32_t result = 0;
	mutex mtx;
	//WAIT, I CAN'T CAPTURE THIS IMPLICITLY?
	const int ITERATION = static_cast<int>(1E6);

	auto func = [=, &result, &mtx]() { 
		for (int i = 0; i < ITERATION; ++i) {
			++result; 
			this_thread::yield();	//Just to slow down the thread execution to induce race condition
			//this_thread::sleep_for(chrono::nanoseconds(0)); 
		}};
	thread t1(func);
	thread t2(func);

	//A common rookie mistake is to forget to wait for the thread to complete before printing the result, for example.
	t1.join();
	t2.join();
	
	cout << "Result = " << result << endl;
}

void movingFutureInStorage()
{
	cout << __FUNCSIG__ << endl;
	vector<future<uint32_t>> workResult;

	for (uint32_t count = 0; count < thread::hardware_concurrency(); ++count)
	{
		//Garbled cout, but demonstrates moving the result of async to workResult vector
		workResult.push_back(async([&count]() { cout << count << endl;  return count; }));
	}

	vector<future<uint32_t>> anotherResult;
	for (uint32_t count = 0; count < thread::hardware_concurrency(); ++count)
	{
		future<uint32_t> moreResult = async([&count]() { return count; });
		anotherResult.push_back(move(moreResult));
	}
}