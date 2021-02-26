#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <vector>
#include <random>
#include "..\Monitor\Monitor.cpp"
using namespace std;

template class Concurrency::Monitor<vector<int>>;
using TestMonitor = typename Concurrency::Monitor<vector<int>>;

template <typename MonitorImpl>
class ConcurrentVector : public MonitorImpl {
private:
	condition_variable m_proceed;
public:
	using Product = typename MonitorImpl::SharedData;
	//This 
	//void produce(Product product);
	///Product consume();
};

template class ConcurrentVector<TestMonitor>;
//I use copies of the product for thread correctness
//out is a reference to the synchronized ostream operator
/*template<typename T>
void produce(ostream& out, T product) {
	out << "Producing " << product << endl;
}

template<typename T>
void consume(ostream& out, T product) {
	out << "Consuming " << product << endl;
}
*/

//The Standard says that std::future's destructor is non-blocking, unless if it is created by a std::async
//so these 2 calls behaves differently:
//std::async([](){dofirsttask()};);
//std::async([](){dosecondtask()};);
//versus
//std::future<void> f1 = std::async([](){dofirsttask()};);
//std::future<void> f2 = std::async([](){dosecondtask()};);
//The first group is guaranteed to execute in the same order, i.e dofirsttask() lambda always execute before dosecondtask() lamba because of the blocking nature of the 
//temporary future<void> return value from the async function call.
//The second group has a non-deterministic thread launch sequence, and they are both non-blocking.
//Now i need to know what if the future destructor gets called without anyone waiting for the execution to complete. Now what?
void futureOutOfScopeTest()
{
	//I know I have a BlockingQueue type. This is my proof of concept on a potential non-blocking push and blocking pop
	//Important concept: cout is a global object that cannot be copied. So I use ostream&, which is a type of reference to an ostream object, and initialize it with
	//the reference to the global object cout
	//I am pretty sure this is a bad idea. Can't prove it
	Concurrency::Monitor<ostream*> coutGuard{ &cout };
	using ostreamPtr = decltype(coutGuard)::SharedData;
	random_device seeder;
	mt19937 engine(seeder());
	//ms time duration generator
	uniform_int_distribution generator{ 1,100 };
	{
		vector<future<void>> producers;
		vector<future<void>> consumers;
		for (int i = 0; i < 5; ++i) {
			int sleepTime = generator(engine);
			producers.push_back(async([=, &coutGuard]() { coutGuard([=](ostreamPtr out) { 
				ostream& st = *out;
				st << "Producer " << i << " sleep time (" << sleepTime << "ms) ";
				this_thread::sleep_for(chrono::milliseconds(sleepTime));
				st << "Producing " << i << endl; }); }));

			sleepTime = generator(engine);
			consumers.push_back(async([=, &coutGuard]() { coutGuard([=](ostreamPtr out) { 
				ostream& st = *out;
				st << "Consumer " << i << " sleep time (" << sleepTime << "ms) ";
				this_thread::sleep_for(chrono::milliseconds(sleepTime)); 
				st << "Consuming " << i << endl; }); }));
		}
	}
	//Still a non-deterministic order. Basically, unless you need the result of a thread, and provided that you can still keep the thread resources alive
	//during the lifetime of the thread, you don't need std::future. Therefore async should still allow future destructor to be non-blocking.
	//So just make sure that the variables are alive and well while the threads are still alive?
}