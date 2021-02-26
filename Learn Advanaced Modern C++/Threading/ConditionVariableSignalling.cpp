#include "ConditionVariableSignalling.h"
#include <mutex>
#include <future>
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

//Producer worker function. Returns a value after a milliseconds waitTime (Which I insist has to use chrono to minimize error)
//And produce a random value (which I have to review now :) ) A consumer will wait on the return value.
//The waitTime simulates the amout of time needed to produce the return value
//Right now I won't be using a packaged_task, because for 1 producer one consumer, 1 pass threads, using future.get would have sufficed.
//I suspect that later on, when the producer and consumer is constantly alive, condition_variable will become more important
//This use-case scenario is different from packaged_task. Packaged_task allows another work unit to retrieve the return value of another work unit.
//This implies that the work unit is done (its function returns, rite?)
//condition_variable use case is for work units that produces result inside its critical section, then let a waiting work unit know that the result is ready
//because the thread critical section can be in a loop, condition_variable must be used  
int producer(const chrono::milliseconds& waitTime, mutex& lock, condition_variable& condition, uint32_t& product) {
	mt19937 engine;
	uniform_int_distribution distro(1, 100);
	for (uint32_t numUpdate = 0; numUpdate < 100; ++numUpdate) {
		this_thread::sleep_for(waitTime);
		{
			lock_guard<mutex> scopeGuard(lock);
			product = distro(engine);
		}
		condition.notify_one();
	}
	
	return 0;	//some says that threadfunc retvals should be reserved for error codes.
}

int consumer(const chrono::milliseconds& processingTime, mutex& lock, condition_variable& condition, uint32_t& product) {

	for (uint32_t numConsume = 0; numConsume < 100; ++numConsume) {

		lock_guard<mutex> scopeGuard(lock);
		//condition.wait(lock);
		cout << product << endl;
		this_thread::sleep_for(processingTime);
	}
	
	return 0;
}

//Producer consumer demo not complete yet. What I want to have is a non-blocking consumer who will periodically 
void conditionVariableDemo()
{
	cout <<"Max simultaneous thread runnable "<< thread::hardware_concurrency() << endl;
}