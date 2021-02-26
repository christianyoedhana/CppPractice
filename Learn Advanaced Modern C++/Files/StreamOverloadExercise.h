#pragma once
#include <iostream>
#include <string>
class Car
{
public:
	Car(const std::string& make, const std::string& model, const std::string& color);
	friend std::ostream& operator<<(std::ostream& output, const Car& car);
private:
	std::string m_make;
	std::string m_model;
	std::string m_color;
};

std::ostream& operator<<(std::ostream& output, const Car& car);