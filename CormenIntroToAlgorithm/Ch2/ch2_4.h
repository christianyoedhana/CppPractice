#pragma once
#include <utility>
#include <tuple>
//data represents the stock value at a given day.
//Function returnsa pair of days representing the day to buy and the day to sell that yields the greatest profit
//This will be the test answer to use when I immplement the optimal algorithm
//Hmm should I use std::pair for the return type?
template <class T>
std::pair<typename T::size_type, typename T::size_type> maximumSubarrayBruteForce(T& data);

template <class T>
std::tuple<typename T::size_type, typename T::size_type, typename T::value_type> maximumSubarrayUsingTransformation(T& data);