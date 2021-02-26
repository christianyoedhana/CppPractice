#include "StreamOverloadExercise.h"
using std::string;
using std::ostream;

Car::Car(const string& make, const string& model, const string& color) : m_make(make), m_model(model), m_color(color) {}

ostream& operator<<(ostream& stream, const Car& car) {
	stream << car.m_make << " " << car.m_model << " " << car.m_color;
	return stream;
}