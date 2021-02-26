#include "MutexBasedSync.h"
#include <mutex>
#include <iostream>
#include <future>
#include <exception>

using namespace std;

//Typically we don't call mutex directly. its destructor does not call unlock. Therefore, if an exception arises inside the critical section, it gets locked until program shutdown
void synchronizedOp() {
	//still need a mutex somewhere
	mutex lock;
	//target shared variable
	uint32_t sharedVar = 0;
	auto threadFunc = [](uint32_t& shared, mutex& mtx) {
		for (uint32_t count = 0; count < static_cast<uint32_t>(1e6); ++count) {
			//create a scope guard
			lock_guard<mutex> criticalSection(mtx);
			shared += 1;
			this_thread::yield();
		}
	};
	thread t1(threadFunc, ref(sharedVar), ref(lock));
	thread t2(threadFunc, ref(sharedVar), ref(lock));

	t1.join();
	t2.join();

	cout << sharedVar << endl;
}

//A thread is performing an internal computing and another thread needs the result of this calculation.
//Instead of using an external shared variable, the producing thread can own a promise for the computation result,
//But shared variable is still needed for sharing concurrent computation result
//I think writing the try-catch block is almost a mandatory requirement to write an exception-safe thread func when using promise-future pair.
void promiseFuture()
{
	promise<uint32_t> producer;
	future<uint32_t> consumer = producer.get_future();
	promise<uint32_t> promisedResult;
	future<uint32_t> finalResult = promisedResult.get_future();

	auto producerFunc = [&producer]() {
		try {
			uint32_t result = 0;
			for (uint32_t count = 0; count < static_cast<uint32_t>(1e6); ++count, ++result) {
				this_thread::yield();
			}
			producer.set_value(result);
		}
		catch (...){
			producer.set_exception(current_exception());
		}
	};

	auto consumerFunc = [&consumer, &promisedResult ]() {
		try {
			uint32_t result = consumer.get();
			for (uint32_t count = 0; count < static_cast<uint32_t>(1e6); ++count, ++result) {
				this_thread::yield();
			}
			promisedResult.set_value(result);
		}
		catch (...) {
			promisedResult.set_exception(current_exception());
		}
	};

	thread worker(producerFunc);
	thread millenials(consumerFunc);

	worker.join();
	millenials.join();

	try{
		cout << "Final Result " << finalResult.get() << endl;
	}
	catch (...) {
		cout << "Computation terminated abnormally. Check log" << endl;
	}
}

//Demonstrate passing the return value of a thread func automatically as a std::promise without explicitly creating a promise
void packagedFuture() {
	const uint32_t increment = 4;
	auto work = [=](uint32_t numPass) {
		uint32_t result = 0;
		for (uint32_t count = 0; count < numPass; ++count, result+=increment) {
			this_thread::yield();
		}
		return result;
	};

	//Function signature that takes a uint32 and returns a uint32
	packaged_task<uint32_t(uint32_t)> task(work);
	auto result = task.get_future();

	thread workUnit(move(task), static_cast<uint32_t>(1E6));
	cout << "Future result is " << result.get() << endl;
	workUnit.join();
}