#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cerr << "Usage: ./read-file <file_path>" << std::endl;
        return 1;
    }

    
    std::ifstream file(argv[1], std::ios::in);
    if (!file) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    
    file.close();
    return 0;
}

