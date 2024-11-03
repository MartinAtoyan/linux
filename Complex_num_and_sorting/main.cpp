#include <iostream>
#include <vector>
#include "ComplexNumber.h"
#include "ComplexSort.h"

int main() {
    std::vector<ComplexNumber> numbers = {
        ComplexNumber(3, 4),
        ComplexNumber(1, 2),
        ComplexNumber(0, 1),
        ComplexNumber(4, 3),
        ComplexNumber(5, 0)
    };

    std::cout << "Before sorting:" << std::endl;
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Sort the complex numbers by absolute value
    sortComplexNumbers(numbers);

    std::cout << "After sorting by absolute value:" << std::endl;
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}

