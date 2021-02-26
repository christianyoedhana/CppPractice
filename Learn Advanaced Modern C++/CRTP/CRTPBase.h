#pragma once

//CRTP exercise. T::act1 and T::act2 behavior depends on the implementation of T
template <class T>
class CRTPBase
{
public:
	void task1();
	void task2() const;

private:
	//Got this solution from https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/
	//If T is different from the deriving type, compilation will fail
	CRTPBase();
	T& subclass();
	const T& subclass() const;
	friend T;
};
