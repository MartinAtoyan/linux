#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

void initialize(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    const char* file_path = argv[1];
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (dup2(file_fd, STDIN_FILENO) == -1) {
        perror("Error redirecting standard input");
        close(file_fd);
        exit(EXIT_FAILURE);
    }

    close(file_fd);
}

int main(int argc, char** argv) {
    initialize(argc, argv);

    std::string input;
    std::cin >> input;

    std::string reversed(input.rbegin(), input.rend());
    std::cout << reversed << std::endl;

    return 0;
}

