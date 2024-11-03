#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H

#include <iostream>
#include <cmath>

class ComplexNumber {
public:
    double real;
    double imag;

    ComplexNumber(double r = 0.0, double i = 0.0);

    // Overloaded operators
    ComplexNumber operator+(const ComplexNumber& other) const;
    ComplexNumber operator-(const ComplexNumber& other) const;
    ComplexNumber operator*(double scalar) const;

    // Method to get the absolute value (magnitude)
    double abs() const;

    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& c);
};

#endif // COMPLEXNUMBER_H

