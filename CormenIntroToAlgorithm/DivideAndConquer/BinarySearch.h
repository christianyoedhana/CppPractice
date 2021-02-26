#pragma once
//Returns true if the search succeeded. foundAt will contain a useful value
//Returns false otherwise
//I mean binarySearch can return a pair <foundAt, success>
template<class T>
bool binarySearch(const T& sortedContainer, typename T::value_type searchTerm, typename T::size_type& foundAt);
