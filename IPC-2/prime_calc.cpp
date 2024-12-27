#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int find_nth_prime(int n) {
    int count = 0, candidate = 1;
    while (count < n) {
        ++candidate;
        if (is_prime(candidate)) {
            ++count;
        }
    }
    return candidate;
}

int main() {
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // Child process
        close(pipe1[1]); // Close write end of pipe1
        close(pipe2[0]); // Close read end of pipe2

        while (true) {
            int m;
            if (read(pipe1[0], &m, sizeof(m)) <= 0) {
                break;
            }

            std::cout << "[Child] Calculating " << m << "-th prime number..." << std::endl;
            int result = find_nth_prime(m);
            std::cout << "[Child] Sending calculation result of prime(" << m << ")..." << std::endl;

            if (write(pipe2[1], &result, sizeof(result)) == -1) {
                perror("write");
                break;
            }
        }

        close(pipe1[0]);
        close(pipe2[1]);
        return EXIT_SUCCESS;
    } else {
        // Parent process
        close(pipe1[0]); // Close read end of pipe1
        close(pipe2[1]); // Close write end of pipe2

        while (true) {
            std::string input;
            std::cout << "[Parent] Please enter the number (or 'exit' to quit): ";
            std::cin >> input;

            if (input == "exit") {
                break;
            }

            int m = std::stoi(input);
            std::cout << "[Parent] Sending " << m << " to the child process..." << std::endl;
            if (write(pipe1[1], &m, sizeof(m)) == -1) {
                perror("write");
                break;
            }

            std::cout << "[Parent] Waiting for the response from the child process..." << std::endl;
            int result;
            if (read(pipe2[0], &result, sizeof(result)) > 0) {
                std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << std::endl;
            } else {
                perror("read");
                break;
            }
        }

        close(pipe1[1]);
        close(pipe2[0]);

        wait(nullptr); // Wait for child process to finish
        return EXIT_SUCCESS;
    }
}

