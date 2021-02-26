#include "RaceConditionDemo.h"
#include "MutexBasedSync.h"
#include "ConditionVariableSignalling.h"
#include "Futures.h"
#include <stdint.h>
#include <iostream>

int main() {
	const uint32_t TESTITERATON = 10;
	std::cout << "Demonstrating race condition" << std::endl;
	for (uint32_t count = 0; count < 10; ++count) {
		raceConditionDemo();
	}

	std::cout << "Demonstrating synchronized count" << std::endl;
	for (uint32_t count = 0; count < 10; ++count) {
		synchronizedOp();
	}

	std::cout << "Demonstrating promise-future " << std::endl;
	promiseFuture();
	packagedFuture();
	conditionVariableDemo();
	movingFutureInStorage();
	futureOutOfScopeTest();
	return 0;
}
/*Double Checked Pattern
atomic<Type*> g_ptr{nullptr};

void someInitializer()
{
	if (g_ptr == nullptr)	//first check has the atomic variable been initialized/modified. This is an atomic operation given g_ptr is an atomic type
	{
		lock_guard<mutex> lcx(aMutexSomewhere);	//lets get to our critical section
		if (g_ptr == nullptr)	//Is the thing still need modification? This the the Double Check. The second atomic operation
		{
			g_ptr = allocate();
		}
	}
}
*/