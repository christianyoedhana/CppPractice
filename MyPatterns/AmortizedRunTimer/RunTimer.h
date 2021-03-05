#pragma once
#include <cinttypes>
#include <chrono>
#include <iostream>
#include <typeinfo>
#include <string>
#include <sstream>
/*
*RunTimer is a class with a functor that takes 2 template parameter: ToEval, and ToEvalParam. ToEval type is the entity whose runtime is to be measured. 
* NumLoop is the number of loop to evaluate.
* Runtimer stores the last runtime result. User can display the result later in various time units, as supported by std::chono, or user-defined time unit
* Use case:
* Runtimer evaluating a function once
* 
* RunTimer timer;
* timer(func, funcParam);
* cout<<timer.latency<std::chrono::milliseconds>()<<" ms"<<endl;
* Expected console output: 
* Running func(funcParam) in 1 loop
* Total time : x ms.
* //Runtimer evaluating a functor 100 times
* MyClass toEval;
* Runtimer timer;
* timer(toEval, functorParam, 100);
* cout<<timer.latency<std::chrono::minutes>()<<" minutes"<<endl;
* Expected console output: 
* Running Myclass::toEval(functorParam) in 100 loop
* Total time : y minutes. 
* @todo should RunTimer accept lambdas?
* 
* C++ concept: template argument, variadic template, function return type deduction
*/
namespace MyPatterns {
	class RunTimer {
	private:
		//Need to store time difference using chrono types.
		std::chrono::system_clock::duration m_latency{};
		std::string m_funcName;
		std::uint32_t m_loopCt = 0;

	public:
		template<typename ToEval, typename... Args>
		auto operator()(std::uint32_t numLoops, ToEval toEval, Args... args) -> decltype(toEval(args...)) {
			m_funcName = typeid(ToEval).name();
			//@todo parse Args...
			m_loopCt = numLoops;
			const auto start = std::chrono::system_clock::now();
			decltype(toEval(args...)) retVal = 0;
			for (std::uint32_t cnt = 0; cnt < numLoops; ++cnt) {
				retVal = toEval(args...);
			}
			m_latency = std::chrono::system_clock::now() - start;
			return retVal;
		}

		template<typename TimeUnit>
		std::string latency() {
			using std::cout;
			using std::endl;
			//Prefer stringstream to manual string formatting.
			std::stringstream output;
			output<< "Running "<<m_funcName<<" in "<<m_loopCt<<" loops"<<endl;
			output<< "Total time "<<std::chrono::duration_cast<TimeUnit>(m_latency).count();
			return output.str();
		}
	};
}