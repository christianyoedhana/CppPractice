#pragma once
#include "..\Monitor\Monitor.h"
#include <queue>
namespace Concurrency {
	//BlockingQueue serialize access to a queue data structure
	//Currently acts as a serializer wrapper to a std::queue
	//Concurrency::Monitor wraps a std::queue instance and implements the serialization
	template <typename Data>
	class BlockingQueue {
	private:
		mutable Monitor<std::queue<typename Data>> m_sharedQueue;
	public:
		using QueueType = typename decltype(m_sharedQueue)::SharedData;
		void push(const Data& data);
		void push(Data&& data);

		//For testing only.
		//Mimicking std::queue as much as possible. 
		//Getting front data becomes expensive because now there are 3 locking instead of 1 lock-and-wait then pop-front
		//check empty queue
		bool empty() const;
		//Inspect data at front of queue.
		decltype(auto) front() const;
		//pop the queue
		void pop();
		//This is what I really want
		//Data pop();
	};
}

