#include <mutex>
#pragma once
namespace MyPatterns {
	//In his Concurrency lecture in C++ And Beyond 2012, Herb Sutter presented a Monitor class that wraps a shared data type T while allowing access to all of T's public members
	//Monitor implements Brinch Hansen and C.A.R Hoare's monitor concept, essentially with a Mutex object acting as the blocker.
	//Template function operator() takes a function type F that executes/do some work using the wrapped shared data T.
	//Type cannot be copied or moved because std::mutex is neither copyable nor movable.
	//Monitor pattern uses a very powerful wrapper idiom. An object of a type using this pattern allows any operations on the wrapped obj type via the operator().
	//The key is for the operator() to take a function type with the wrapped object type as the only parameter
	template<typename T> 
	class Monitor {
	private:
		mutable T m_sharedData;
		//This can be made as a template type as long as there is an agreeable blocking call std::async can use
		mutable std::mutex m_mutex;

	public:
		using SharedData = typename T;
		//Herb Sutter allows conversion. Lets try not allowing convesion
		explicit Monitor(T sharedData = T{}) : m_sharedData{ sharedData } {}
		//Here is the magic of auto combination with ->decltype. The return value can be anything. Very convenient to use as long as nobody complains about the
		//binary size. Speaking of which, this is a templated function anyway, so the price will be the same as hand-crafting guarded functions for every object type
		//to share. So the complains will be baseless!
		template <typename F>
		auto operator()(F func)->decltype(func(m_sharedData)) const {
			std::lock_guard<std::mutex> _{ m_mutex };	//OK, so _ is a valid C++ variable name!
			return func(m_sharedData);
		}
		//This is read-only, so a lock guard should not be needed
		SharedData data() const { return m_sharedData; }
	};
}

//Herb talked about a possible implementation of a concurrent_queue as a non-blocking message queue system that takes the func in operator(). We have std::function
//for storing function calls. Push() will put the function object in the queue
//Pop() is a blocking call served by a worker thread in Monitor, whose job is to pop a function object and execute it

//Also, after further consideration, I think I am not that big in #including a cpp file in another cpp compilation unit. I lose shareability.