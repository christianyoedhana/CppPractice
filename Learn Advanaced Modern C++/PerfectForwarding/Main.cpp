#include <iostream>
#include <utility>
using namespace std;

class X {
public:
	X() = default;
	X(X&&) noexcept {}
};

void bar(X&){
	cout << __FUNCSIG__ << endl;
}

void bar(const X&){
	cout << __FUNCSIG__ << endl;
}

void bar(X&&) {
	cout << __FUNCSIG__ << endl;
}

//Should call l-value versions of bar. The r-value version won't get called
template <typename T> void FooL(T&& t) {
	cout << __FUNCSIG__ << " calls : ";
	bar(t);	//This always calls the l-value version because t is al l-value
	//bar(std::move(t));	//This is an abosolute request to the compiler to call bar(X&&) because of std::move rvalue-cast
}

//This one should call bar using the intended parameter value-type. Perfect forwarding should happen here
template <typename T> void Foo(T&& t) {
	cout << __FUNCSIG__ <<" calls : ";
	bar(std::forward<T>(t));
}

//However, without using a template for perfect forwarding, one has to do this
void FooX(X& x)
{
	cout << __FUNCSIG__ << " calls : ";
	bar(x);	
}

void FooX(X&& x)
{
	cout << __FUNCSIG__ << " calls : ";
	bar(x);//x is an l-value, so this will always call bar(X&). Need to memorize this weird rule as a consequence of reference collapsing
}

void FooYeah(X&& x)
{
	cout << __FUNCSIG__ << " calls : ";
	bar(std::move(x));	//now calling g(X&&) because of the rvalue-cast
}

int main()
{
	X x1;
	const X& xref = x1;
	
	FooL(x1);	//calls bar(X&)
	FooL(xref);	//calls bar(const X&)
	FooL(std::move(x1));	//calls bar(x&);

	Foo(x1);	//calls bar(X&)
	Foo(xref);	//calls bar(const X&)
	Foo(std::move(x1));	//calls bar(X&&)

	FooX(x1);	//calls bar(X&)
	FooX(std::move(x1));	//Still calls bar(X&)
	FooYeah(std::move(x1));	//now bar(X&&) gets called

	//IF you pass a template argument by reference, the type does not decay. so const char [5] remains const char[5], not const char*
	//cout << "Foo(\"Yeah\")" << endl;
	//Foo("Yeah");
}

//Given this demonstration, we can see how std::forward can correctly deduce the 'mapping' of the universal refercene to its semantic l-value or r-value reference.
//With the correct use of std::forward, you should never have to write functions with these overloading signatures:
//template <typename T> void Foo(T&& t);	//this would be sufficient to handle both l-value and r-value parameters
//template <typename T> void Foo(T& t);		//compiler allows this, but seriously not necessary