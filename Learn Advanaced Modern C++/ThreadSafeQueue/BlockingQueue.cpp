#include <queue>
#include <vector>
#include <future>
#include <random>
#include <chrono>
#include <iostream>
#include "..\Monitor\Monitor.cpp"	//Type specialization requires this cpp to be included
#include "BlockingQueue.h"

namespace Concurrency {
	template <typename Data>
	void BlockingQueue<Data>::push(const Data& data) {
		m_sharedQueue([&data](QueueType& queueImpl) {
			queueImpl.push(data);
			});
	}

	template <typename Data>
	void BlockingQueue<Data>::push(Data&& data) {
		m_sharedQueue([&](QueueType& queueImpl) {
			queueImpl.push(std::move(data));
			});
	}

	template <typename Data>
	bool BlockingQueue<Data>::empty() const{
		return m_sharedQueue([](QueueType& queueImpl) { return queueImpl.empty(); });
	}

	template <typename Data>
	decltype(auto) BlockingQueue<Data>::front() const {
		return m_sharedQueue([](QueueType& impl) { return impl.front(); });
	}

	template <typename Data>
	void BlockingQueue<Data>::pop() {
		m_sharedQueue([](QueueType& impl) { if (!impl.empty()) impl.pop(); });
	}
}
