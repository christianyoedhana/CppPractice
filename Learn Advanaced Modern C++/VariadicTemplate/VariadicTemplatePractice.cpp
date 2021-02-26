#include <iostream>
#include <string>

using namespace std;

template<typename T>
void variadic(T last)
{
	cout << last;
}

template<typename T, typename... Args>
void variadic(T first, Args... args) {
	cout << first << " ";
	variadic(args...);
}
//This is silly, but can I define a function template that takes a function, and its arguments, then execute the function with the argument values?
//I don't think variadic template is the answer. Maybe I need to use std::argument_list (the {} type). Read more about variadic templates
/*
template<template<typename T> typename Func, typename Last>
void customBind(Func<Last> func, Last last) {
	func(last);
}*/
//The intention is dumb, but I am just wandering if I can create a tempate that takes a functio object, and a variable number of parameter.
template<template<typename... Fargs> typename Func, typename... Args>
void customBind(Func<Args...> func, Args... args)
{
	func(args...);
}


template<typename Content>
class Test {
public:
	Test() = default;
	explicit Test(const Content& content) : m_content{ content } {}

	inline friend ostream& operator<< (ostream& out, const Test & test) {
		out << test.m_content;
		return out;
	}
private:
	Content m_content;
};

int main() {
	variadic(0.5, 1, "Hahaha", Test<string>{ "Nice" }, Test<uint32_t>{ 100 });
	cout << endl;

	//This is beyond me
	//customBind(variadic, 0.5, 1, "Hahaha", Test<string>{ "Nice" }, Test<uint32_t>{ 100 });
}
