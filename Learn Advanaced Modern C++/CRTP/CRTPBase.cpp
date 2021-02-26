#include "CRTPBase.h"
#include <iostream>

using namespace std;
//This default constructor can be complicated, and hiding its implementation from the header file would have been highly desired
template <typename T>
CRTPBase<T>::CRTPBase() {}

//this-> is mandatory.
template<typename T>
void CRTPBase<T>::task1(){
	cout << __FUNCSIG__ << endl;
	subclass().act1();	
}

template<typename T>
void CRTPBase<T>::task2() const{
	cout << __FUNCSIG__ << endl;
	subclass().act2();
}

template<typename T>
T& CRTPBase<T>::subclass() {
	cout << __FUNCSIG__ << endl;
	return static_cast<T&>(*this); 
}

template<typename T>
const T& CRTPBase<T>::subclass() const {
	cout << __FUNCSIG__ <<" const "<< endl;
	return static_cast<const T&>(*this); 
}