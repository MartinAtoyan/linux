#include "ComplexNumber.h"

ComplexNumber::ComplexNumber(double r, double i) : real(r), imag(i) {}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
    return ComplexNumber(real + other.real, imag + other.imag);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
    return ComplexNumber(real - other.real, imag - other.imag);
}

ComplexNumber ComplexNumber::operator*(double scalar) const {
    return ComplexNumber(real * scalar, imag * scalar);
}

double ComplexNumber::abs() const {
    return std::sqrt(real * real + imag * imag);
}

std::ostream& operator<<(std::ostream& os, const ComplexNumber& c) {
    os << "(" << c.real << ", " << c.imag << ")";
    return os;
}

