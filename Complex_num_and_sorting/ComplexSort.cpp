#include "ComplexSort.h"
#include <algorithm>

void sortComplexNumbers(std::vector<ComplexNumber>& numbers) {
    std::sort(numbers.begin(), numbers.end(), [](const ComplexNumber& a, const ComplexNumber& b) {
        return a.abs() < b.abs();  // Sort based on absolute value
    });
}


