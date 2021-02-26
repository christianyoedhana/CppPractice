#include "Test.h"
#include "BlockingQueue.cpp"

template class Concurrency::BlockingQueue<uint32_t>;

using namespace std;
void test()
{
	Concurrency::BlockingQueue<uint32_t> blockingQueue;

	random_device seed;
	mt19937 engine(seed());
	uniform_int_distribution generator{ 10, 15 };
	enum {
		NUM_WORK = 12
	};

	//vector<uint32_t> testData{ NUM_WORK };
	//generate(testData.begin(), testData.end(), [&engine]() { uniform_int_distribution generate{ 1,100 }; return generate(engine); });
	//Simulate work with random sleep time
	vector<future<void>> workSimulation;
	for (uint32_t count = 0; count < NUM_WORK; ++count) {
		workSimulation.push_back(async([&, count]() { this_thread::sleep_for(std::chrono::microseconds(generator(engine)));
		blockingQueue.push(count); }));
	}

	for (auto& work : workSimulation) {
		work.wait();
	}

	cout << "Dumping Queue Content" << endl;
	//This is the expensive part. Three locks
	while (!blockingQueue.empty())
	{
		cout << blockingQueue.front() << endl;
		blockingQueue.pop();
	}
}