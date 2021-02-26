#include <numeric>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>

using namespace std;

//since we are on the subject of stl numeric exercise
template<uint32_t fib> struct Fibonacci
{
	static constexpr uint32_t value = Fibonacci<fib - 1>::value + Fibonacci<fib - 2>::value;
};

template<> struct Fibonacci<1> { static constexpr uint32_t value = 1; };
template<> struct Fibonacci<0> { static constexpr uint32_t value = 0; };

//Now using constxpr
constexpr int fibonacci(int fib)
{
	return (fib == 1) ? 1 : (fib == 0) ? 0 : fibonacci(fib - 1) + fibonacci(fib - 2);
}

void compileTimeFibonacci()
{
	cout << "Reviewing compile-time Fibonacci" << endl;
	const uint32_t fib = 7;
	constexpr int result = Fibonacci<fib>::value;
	cout << "Compile time Fibonacci of " << fib << " equal to " << result << endl;
	cout << "Since C++ 11 introduced constexpr, the fibonacci function should be compile time fibonacci. The way to prove it is to calculate Fibonacci<fibonacci("<<fib<<")>::value" << endl;
	cout << "Program should compile if I have crafted a compile-time fibonacci()" << endl;
	constexpr uint32_t fibFromFib = Fibonacci<fibonacci(fib)>::value;
	constexpr uint32_t theFib = fibonacci(fib);
	cout << "Fibonacci<fibonacci("<<fib<<")>::value = Fibonacci<" << theFib << ">::value = " << fibFromFib << endl;
}
//In the old days before calculators, we have to use a pre-computed logarithmic table, and linearly interpolate between 2 table entries to get our
//log computation. This is a log10 table entry
class LogTableEntryCompute
{
private:
	double m_log;
	double m_x;
	double m_incr;	//Value increment
public:
	explicit LogTableEntryCompute(double incr = 0.1) : m_incr(incr), m_x(0.0), m_log(0.0) {};
	LogTableEntryCompute operator()(double x){
		m_x = x;
		m_log = log10(m_x);
		return *this;
	}
	friend ostream& operator<<(ostream& out, const LogTableEntryCompute& logValue);

	//The postfix version syntax is retType operator++(int), so in this case the int parameter is useless.
	LogTableEntryCompute& operator++()
	{
		m_x += m_incr;
		m_log = log10(m_x);
		return *this;
	}

	//This is a rare instance where I have to include an unused parameter in my function definition.
	LogTableEntryCompute operator++(int)	//There I decided to adapt this for silencing unused parameter warning
	{
		LogTableEntryCompute retVal(*this);
		operator++();
		return retVal;
	}
};

ostream& operator<<(ostream& out, const LogTableEntryCompute& logValue)
{
	out << logValue.m_x<<":"<<logValue.m_log;
	return out;
}

void calculateLogTableUsing_iota()
{
	LogTableEntryCompute logCompute(0.1);
	array<LogTableEntryCompute, 20> tableEntry;
	iota(tableEntry.begin(), tableEntry.end(), logCompute(10));
	ostream_iterator<LogTableEntryCompute> logStream(cout, " ");
	for (const auto& entry : tableEntry)
	{
		*logStream =  entry;
	}
	cout << endl;
	cout << "Testing postfix ++ operator" << endl;
	cout << logCompute(100)++ << endl;	//This should display 2
}

//Naive dot product calculated using std::inner_product
//If the iterator ranges are not the same, inner_product will throw some sort of iterator out of range exception
template <typename NumType> NumType dot(const vector<NumType>& lhs, const vector<NumType>& rhs)
{
	return inner_product(lhs.begin(), lhs.end(), rhs.begin(), static_cast<NumType>(0));
}

void innerProductExercise()
{
	cout << "InnerProduct for calculating dot product" << endl;
	vector<double> v1{ 1,2,3 };
	vector<double> v2{ 4,5,6 };
	double dotResult = dot(v1, v2);
	cout << "V1.V2 = " << dotResult << endl;
	vector<double> v3{ 1,2,3,4 };
	//this should throw an exception. Uh nope, calculation stops at the first end iterator. OK, so any dot product implementation must do a range check
	double mayThrow = dot(v1, v3);
	cout << "MayThrow = " << mayThrow << endl;
}