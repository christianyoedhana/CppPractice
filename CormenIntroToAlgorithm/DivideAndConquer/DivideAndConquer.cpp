// DivideAndConquer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <string>
#include <algorithm>
#include "BinarySearch.cpp"
#include "Ex2_3_7.h"

void binarySearchTest()
{
    using namespace std;
    const string falsePositive{ "binarySearch is not supposed to return true on an empty container" };
    const string falseNegative{ "binarySearch is supposed to find the seach term" };
    vector<int> emptyContainer;
    using Index = vector<int>::size_type;
    Index index = 0;
    cout << "binarySearch(emptyContainer, 0, index)" << endl;
    if (binarySearch(emptyContainer, 0, index)) {
        cout << falsePositive << endl;
        return;
    }

    vector<int> singleItem{ -1 };
    cout << "binarySearch(singleItem, -1, index)" << endl;
    if (!binarySearch(singleItem, -1, index)){
        cout << falseNegative << endl;
        return;
    }

    cout << "binarySearch(singleItem, 0, index)" << endl;
    if (binarySearch(singleItem, 0, index)) {
        cout << falsePositive << endl;
        return;
    }

    mt19937 engine;
    const int edge = 100;
    uniform_int_distribution<> generator{ -edge, edge };
    constexpr size_t N = 10;
    vector<int> regularSearchItem;

    generate_n(back_inserter(regularSearchItem), N, [&]() { return generator(engine); });
    //Need to sort
    sort(regularSearchItem.begin(), regularSearchItem.end());
    cout << "Searching from :";
    for_each(regularSearchItem.cbegin(), regularSearchItem.cend(), [](auto item) {cout << item << " "; });
    cout << endl;
    cout << "binarySearch(regularSearchItem, *(regularSearchItem.begin()), index)" << endl;
    //OK begin() is the iterator at the start of the container. end() is the iterator one-item-past the container
    if (!binarySearch(regularSearchItem, *regularSearchItem.begin(), index)) {
        cout << falseNegative << endl;
        return;
    }
    const string wrongResult{ "Item found at the wrong index" };
    if (index != 0) {
        cout << wrongResult << endl;
        return;
    }

    cout << "binarySearch(regularSearchItem, *(--regularSearchItem.end()), index)" << endl;
    if (!binarySearch(regularSearchItem, *(--regularSearchItem.end()), index)) {
        cout << falseNegative << endl;
        return;
    }
    if (index != regularSearchItem.size() - 1) {
        cout << wrongResult << endl;
        return;
    }

    cout << "binarySearch(regularSearchItem, edge + 1, index)" << endl;
    if (binarySearch(regularSearchItem, edge + 1, index)) {
        cout << falsePositive << endl;
        return;
    }
    cout << "All binarySearch tests have completed successfully" << endl;
}

int main()
{
    using namespace std;
    binarySearchTest();
    Ex2_3_7();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
