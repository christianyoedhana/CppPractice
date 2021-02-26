#include "Monitor.h"

namespace Concurrency {
	using std::lock_guard;

	template<typename T> 
	Monitor<T>::Monitor(T sharedData) : m_sharedData{ sharedData } {}

	template<typename T> template <typename F>
	auto Monitor<T>::operator()(F func)->decltype(func(m_sharedData)) const {
		lock_guard<std::mutex> _{ m_mutex };	//Holy shit I don't know that _ is a valid C++ variable name!
		return func(m_sharedData);
	}
}