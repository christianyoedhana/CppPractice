#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>

//Take pivot value from the end of the list
//front will move forward while the value is less than pivot
//back move backward while the value is greater or equal to pivot.
//stop process after front > back
//swap pivot position
//This is inefficient because there are 4 conditional statements in the loop.
//However, no typecasting from signed to unsigned for the container access index
//Turns out this is a variant of Hoare's partition. But instead of while true, I check that front < back for a valid loop invariant
template< class T, class Pred>
typename T::size_type partitionNoCast(T& list, typename T::size_type front, typename T::size_type back, Pred const &compare)
{
	auto pivot = list.at(back);
	const auto pivotPos = back;
	while (front < back) {
		//while (list.at(front) < pivot) front += 1;
		while (compare(list.at(front), pivot)) front += 1;
		while (!compare(list.at(back), pivot)) back -= 1;
		if (back > front) {
			std::swap(list.at(front), list.at(back));
			front += 1;
		}
	}
	//final swap
	std::swap(list.at(pivotPos), list.at(front));
	return front;
}

//Cormen's partition. Super efficient because there are only 2 conditional statement per loop
//Still does not require type casting.
//However, the largest partition size has been reduced to half because this implementation uses a signed int as an indexer
//I can type cast the signed int to unsigned int. However, an overflow warning might be issue by the compiler.
template< class T, class Pred>
typename T::size_type partition(T& list, typename T::size_type front, typename T::size_type back, Pred const &compare)
{
	//nextToSwap points to the position prior to the potential swap index.
	auto nextToSwap = front - 1;	//This is an underflow once, at the very beginning of the partition
	auto pivot = list.at(back);
	for (auto toSwap = front; toSwap < back; ++toSwap) {
		if (compare(list.at(toSwap), pivot)) {
			std::swap(list.at(toSwap), list.at(++nextToSwap));
		}
	}
	std::swap(list.at(++nextToSwap), list.at(back));
	return nextToSwap;
}

//I think Cormen's partition can be adapted to take an arbitrary pivot in the list
//This one randomly select a pivot from front to back
//Call me a cheater, but I think this will do
template <class T, class Pred>
auto partitionArbitrary(T& list, typename T::size_type front, typename T::size_type back, Pred const &compare) {
	using namespace std;
	random_device seed;
	mt19937 engine(seed());
	uniform_int_distribution const gen{ front, back };
	std::swap(list.at(gen(engine)), list.at(back));
	return partition(list, front, back, compare);
}

//Checks a complete list T for partition correctness.
template<class T>
bool isPartitionValid(T& list, typename T::size_type pivotIndex) {
	using namespace std;
	const auto pivot = list.at(pivotIndex);
	//Check left
	for (auto index = 0; index < pivotIndex; ++index) {
		//Value == pivot is a meh.
		if (auto const value = list.at(index) > pivot) {
			cout << "Invalid value at " << index << ": " << value << " > " << pivot << endl;
			return false;
		}
	}
	//Check right
	for (auto index = pivot; index < list.size(); ++index) {
		if (auto const value = list.at(index) < pivot) {
			cout << "Invalid value at " << index << ": " << value << " < " << pivot << endl;
			return false;
		}
	}
	return true;
}

void partitionTest()
{
	using namespace std;
	vector<int> listToPartition{ 15, -1, 22, 4, 10, 121, 1, -2, 5 };
	auto firstRun(listToPartition);
	cout << "Before partition : ";
	for_each(firstRun.cbegin(), firstRun.cend(), [](auto value) { cout << value << " "; });
	cout << endl;

	auto const ascending = [](auto const& left, auto const& right)->bool { return left <= right; };
	auto pivotPos = partition(firstRun, 0, firstRun.size() - 1, ascending);
	cout << "After partition : ";
	for_each(firstRun.cbegin(), firstRun.cend(), [](auto value) { cout << value << " "; });
	cout << endl;
	cout << "Pivot at index " << pivotPos << endl;
	if (!isPartitionValid(firstRun, pivotPos)) return;

	//Now compare with my partition
	cout << "Using my partition ";
	pivotPos = partitionNoCast(listToPartition, 0, listToPartition.size() - 1, ascending);
	for_each(listToPartition.cbegin(), listToPartition.cend(), [](auto value) { cout << value << " "; });
	cout << endl;
	cout << "Pivot at index " << pivotPos << endl;
	isPartitionValid(listToPartition, pivotPos);
}

void arbitraryPartitionTest() {
	using namespace std;
	constexpr size_t N = 16;
	vector<int> list(N);
	mt19937 engine;	//I need the same random number sets for debugging
	constexpr auto range = 64;
	uniform_int_distribution const generator{ -range, range };
	generate(list.begin(), list.end(), [&]() { return generator(engine); });
	cout << "Using my partition ";

	auto const ascending = [](auto const& left, auto const& right)->bool { return left <= right; };
	auto const pivotPos = partitionArbitrary(list, 0, N - 1, ascending);
	for_each(list.cbegin(), list.cend(), [](auto value) { cout << value << " "; });
	cout << endl;
	cout << "Pivot at index " << pivotPos << endl;
	isPartitionValid(list, pivotPos);
}

template <class T, class Predicate, class Partition>
void quickSortHelper(T& list, typename T::size_type start, typename T::size_type end, Predicate const& compare, Partition const &partitionPolicy) {
	if (end < start) return;
	auto const pivot = partitionPolicy(list, start, end, compare);
	//edge case: pivot is the largest or smallest element in the list
	if (pivot > start) {
		quickSortHelper(list, start, pivot - 1, compare, partitionPolicy);
	}
	quickSortHelper(list, pivot + 1, end, compare, partitionPolicy);
}

template <class T, class Predicate, class Partition>
void quickSort(T& list, Predicate const& compare, Partition const &partitionPolicy)
{
	quickSortHelper(list, 0, list.size() - 1, compare, partitionPolicy);
}

template <class T, class Predicate>
bool isSortingCorrect(T& list, Predicate const& compare) {
	using namespace std;
	bool correct = true;
	for (auto iter = list.cbegin(); iter != list.cend() - 1; ++iter) {
		correct= correct && compare(*iter, *(iter + 1));
	}
	return correct;
}

//Demonstrate the power offered by template type deduction. I can choose the Predicate and the Partition policy
//for the quicksort algorithm
void quicksortTest() {
	constexpr std::size_t N = 16;
	std::vector<int> list(N);
	std::mt19937 engine;	//I need the same random number sets for debugging
	constexpr auto range = 64;
	std::uniform_int_distribution const generator{ -range, range };
	generate(list.begin(), list.end(), [&]() { return generator(engine); });
	for_each(list.cbegin(), list.cend(), [](auto& item) { std::cout << item << " "; }); std::cout << std::endl;
	
	auto const ascending = [](auto const& left, auto const& right)->bool { return left <= right; };
	auto const descending = [](auto const& left, auto const& right)->bool { return left >= right; };
	
	//sort using non-randomized pivot
	quickSort(list, ascending, partition<decltype(list), decltype(ascending)>);
	//quickSort(list);	//C2783, The compiler cannot determine a template argument. Default arguments cannot be used to deduce a template argument.
	for_each(list.cbegin(), list.cend(), [](auto& item) { std::cout << item << " "; }); std::cout << std::endl;
	std::cout << "Sorting is correct? " <<std::boolalpha<< isSortingCorrect(list, ascending) << std::endl;;

	quickSort(list, descending, partition<decltype(list), decltype(descending)>);
	for_each(list.cbegin(), list.cend(), [](auto& item) { std::cout << item << " "; }); std::cout << std::endl;
	std::cout << "Sorting is correct? " <<std::boolalpha<< isSortingCorrect(list, descending)<< std::endl;

	//sort using randomized pivot
	quickSort(list, ascending, partitionArbitrary<decltype(list), decltype(ascending)>);
	for_each(list.cbegin(), list.cend(), [](auto& item) { std::cout << item << " "; }); std::cout << std::endl;
	std::cout << "Sorting is correct? " << std::boolalpha << isSortingCorrect(list, ascending) << std::endl;;
}