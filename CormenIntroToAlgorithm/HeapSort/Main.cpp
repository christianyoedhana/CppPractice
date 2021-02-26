#include <vector>
#include <deque>
#include <random>
#include <iostream>
#include <algorithm>
#include "HeapSort.cpp"

using namespace std;

//Instantiate the heapSort for vector<int> template parameter type to trigger Main.cpp rebuild everytime HeapSort.cpp changes
template void heapSort<vector<int>>(vector<int>&);

int main() {
	//test data
	constexpr size_t N = 8;//Full heap, skip element at index 0
	vector<int> heap(N);
	mt19937 engine;	//always generate the same sequence for now, just for debugging
	constexpr int R = 64;
	uniform_int_distribution gen{ -R + 1, R };
	cout << "Testing Heap Sort using vector :" << endl;
	//Skip element at index 0. The heapify parent-child calculation is simpler if heap root starts at index i=1;
	generate(heap.begin(), heap.end(), [&]() { auto item = gen(engine); cout << item << " "; return item; });
	cout << endl;
	heapSort(heap);

	cout << "Testing Heap Sort using deque :" << endl;
	deque<int> pqueue(N);
	generate(pqueue.begin(), pqueue.end(), [&]() { auto item = gen(engine); cout << item << " "; return item; });
	cout << endl;
	heapSort(pqueue);
	return 0;
}