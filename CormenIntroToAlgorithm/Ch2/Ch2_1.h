#pragma once
//Exercise 2.1-2 in-place insertion sort that takes a predicate and a container supporting operator[]
template <class Container, class Predicate> void insertionSort(Container& toSort, Predicate&& pred);
