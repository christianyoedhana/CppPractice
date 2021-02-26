#include <vector>
#include <iostream>
#include <iterator>	//ostream_iterator and istream_iterator, back_inserter and front_inserter too
#include <array>
#include <algorithm>
using namespace std;

template <typename T> class Stack;
template< typename T> ostream& operator<<(ostream& out, const Stack<T>& toDisplay);
template<typename T> Stack<T> operator+(const Stack<T>&, const Stack<T>&);

//stack simulataion
template <typename type> class Stack
{
private:
	vector<type> m_stack;
	friend ostream& operator<<<type>(ostream& out, const Stack<type>& toDisplay);
	friend Stack<type> operator+<type>(const Stack<type>&, const Stack<type>&);
public:
	void push(const type& item)
	{
		m_stack.push_back(item);
	}

	type pop()
	{
		type toPop = m_stack.back();
		m_stack.pop_back();
		return toPop;
	}

	bool isEmpty() 
	{
		return m_stack.empty();
	}
	//implicit type conversion, which is messed up
	Stack(int toConvert) : m_stack(toConvert) {}
	Stack() = default;
};

template<typename T> Stack<T> operator+(const Stack<T>& lhs, const Stack<T>& rhs)
{
	Stack<T> result{ lhs };
	result.m_stack.insert(result.m_stack.end(), rhs.m_stack.begin(), rhs.m_stack.end());
	return result;
}

template< typename type> ostream& operator<<(ostream& out, const Stack<type>& toDisplay)
{
	for (const auto member : toDisplay.m_stack)
	{
		out << member << ' ';
	}
	return out;
}

void iteratorExercise()
{
	cout << "Iterator exercise" << endl;
	Stack<double> doubleStack;
	doubleStack.push(10.1);
	doubleStack.push(20.2);
	doubleStack.push(30.3);
	cout << "DoubleStack content :";
	cout << doubleStack << endl;
	if (!doubleStack.isEmpty())	//Fair requirement to avoid an exception?
	{ 
		auto top = doubleStack.pop();
		cout << "Popped " << top << endl;
	}

	Stack<double> doubleDoubleStack = doubleStack + doubleStack;
	cout << "Double Whopper Stack " << doubleDoubleStack << endl;

	int stackSize = 1;
	//This is not supposed to compile
	//Stack<double> illegalStack = doubleStack + stackSize;
	Stack<double> illegalStack = doubleStack + Stack<double>{stackSize};	//The explicit conversion will work.
}

//So there are 2 ways to deal with wanting the implicit conversion to work. But when do you want that implicit conversion to work?
//I can think of using complex number.
//I do not define a custom destructor, so rule of 3 does not apply
template<typename NumType>
class ComplexNumber
{
private:
	NumType m_real;
	NumType m_i;	//Trust me, i is the symbol for imaginary number
public:
	ComplexNumber() :m_real{ 0 }, m_i{ 0 }{}
	ComplexNumber(const NumType& real, const NumType& i) :m_real{ real }, m_i{ i }{}
	ComplexNumber(const NumType& real) : m_real{ real }, m_i{ 0 }{}	//Just for the sake of argument
	//Using the inline friend function o a class template like this saves the aggravation of typing
	friend ostream& operator<<(ostream& out, const ComplexNumber<NumType>& toString)
	{
		out << toString.m_real << "+" << toString.m_i << "i";
		return out;
	}
	/*This is the only way to alllow implicit conversion when the class allows the conversion
	* I guess Bjarne makes it hard because implicit conversion is generally not desired. Explicit conversion makes the code more readable.
	friend ComplexNumber<NumType> operator+(const ComplexNumber<NumType>& lhs, const ComplexNumber<NumType>& rhs)
	{
		return ComplexNumber<NumType>{lhs.m_real + rhs.m_real, lhs.m_i + rhs.m_i};	//default move constructor should kick in, or RVO.
	}
	*/

	//Otherwise you have to perform an explicit conversion when defining the operator overload with this
	ComplexNumber<NumType>& operator+=(const ComplexNumber<NumType>& rhs)
	{
		m_real += rhs.m_real;
		m_i += rhs.m_i;
		return *this;
	}
};

template <typename T> ComplexNumber<T> operator+(const ComplexNumber<T>& lhs, const ComplexNumber<T>& rhs)
{
	return ComplexNumber<T>{lhs} += rhs;
}

void inlineFriendDemo()
{
	cout << "Inline Friend Demo" << endl;
	ComplexNumber<double> cmp{ 1.0, 1.0 };
	
	cout << cmp << endl;
	
	cout << 1.0 << endl;	//This lacks the hint to the compiler that the inlined friend operator<< should be used. Not enough hint, so compiler uses the easiest cout resolution, which is the stl
	cout << ComplexNumber{ 1.0 } << endl;
	//cout << cmp + 3.0 << endl;	//And the implicit conversion from double to ComplexNumber works because of the fried operator+ function
	cout << cmp + ComplexNumber<double>{3} << endl;	//Implicit conversion fails because I redefine operator+ using ComplexNumber's public method operator+=
	//ComplexNumber<int> zero;
	//ComplexNumber<int> fail = zero.operator+(zero, zero);//compiler balk at this not because of the parameter list, but because operator+ is not defined in ComplexNumber
}

void ostreamIteratorExercise()
{
	array<int, 10> numbers{ 1,2,3,4,5,6,7,8,9,10 };
	ostream_iterator<int> addLine(cout, "\n");
	for (const auto num : numbers)
	{
		*addLine = num;
		++addLine;
	}
}

void istreamIteratorExercise()
{
	istream_iterator<string> inString{ cin };
	istream_iterator<string> eol;	//Supposedly this is ctrl+z in windows
	vector<string> inputFromStream;
	auto backInserter = back_inserter(inputFromStream);
	while (inString != eol)
	{
		*backInserter = *(inString++);	//I just want to be fancy with inserter iterators
	}

	//cin by default does not stream in newline character. Which sucks.
	for (const auto& inputs : inputFromStream)
	{
		cout << inputs;
	}
}

void vectorReserveVsResize()
{
	cout << "Demonstrating the difference between reserve and resize on iterators" << endl;
	vector<int> toReserve;
	vector<int> toResize;
	toReserve.reserve(100);
	toResize.resize(100, 8);
	//expect size to be 0
	cout << "toReserve.size() = " << toReserve.size() <<" .capacity() = "<<toReserve.capacity()<< endl;
	//This should not print at all
	cout << "Printing toReserve ";
	for_each(toReserve.cbegin(), toReserve.cend(), [](int i) { cout << i << ' '; });
	cout << endl;
	//Now lets do a back_inserter populate
	fill_n(back_inserter(toReserve), 10, 1024);
	cout << "toReserve.size() = " << toReserve.size() << " .capacity() = " << toReserve.capacity() << endl;
	//This should not print at all
	cout << "Printing toReserve ";
	for_each(toReserve.cbegin(), toReserve.cend(), [](int i) { cout << i << ' '; });
	cout << endl;

	cout << "toResize.size() = " << toResize.size() <<" .capacity() = "<<toResize.capacity()<< endl;
	ostream_iterator<int> spaceInBetween(cout, " ");
	//Should print all zeros
	cout << "Printing toResize ";
	for_each(toResize.cbegin(), toResize.cend(), [&spaceInBetween](int i) { *spaceInBetween = i; });
	cout << endl;
	vector<int> copyTarget;
	copyTarget.reserve(toResize.size());
	/*I was hoping for an exception. Instead, I am getting a debug assertion
	try {
		copy(toResize.cbegin(), toResize.cend(), copyTarget.begin());
	}
	catch (...)
	{
		cout << "Copy just crashed. copyTarget.size() = " << copyTarget.size() << " .capacity() = " << copyTarget.capacity() << endl;
	}*/

	//now try this
	copy(toResize.cbegin(), toResize.cend(), back_inserter(copyTarget));
	cout << "copyTarget.size() = " << copyTarget.size() << " .capacity() = " << copyTarget.capacity() << endl;
	cout << "Printing copyTarget ";
	for_each(copyTarget.cbegin(), copyTarget.cend(), [&spaceInBetween](int i) { *spaceInBetween = i; });
	cout << endl;
}