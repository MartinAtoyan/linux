#include <iostream>
#include <vector>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>

void do_command(char** argv) {
    auto start_time = std::chrono::steady_clock::now();
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (execvp(argv[0], argv) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        if (WIFEXITED(status)) {
            std::cout << "Command completed with " << WEXITSTATUS(status) << " exit code and took " << duration.count() << " seconds." << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Command terminated by signal " << WTERMSIG(status) << "." << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [args...]" << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<char*> command_args;
    for (int i = 1; i < argc; ++i) {
        command_args.push_back(argv[i]);
    }
    command_args.push_back(nullptr);
    do_command(command_args.data());
    return EXIT_SUCCESS;
}

