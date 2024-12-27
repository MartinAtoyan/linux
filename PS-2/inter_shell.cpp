#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>

void execute_command(const std::vector<std::string>& args, bool silent) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (silent) {
            std::string log_file = std::to_string(getpid()) + ".log";
            int fd = open(log_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        std::vector<char*> c_args;
        for (const auto& arg : args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        c_args.push_back(nullptr);

        char* path = getenv("PATH");
        std::string new_path = ".:" + std::string(path ? path : "");
        setenv("PATH", new_path.c_str(), 1);

        if (execvp(c_args[0], c_args.data()) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            std::cout << "Command exited with code " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Command terminated by signal " << WTERMSIG(status) << std::endl;
        }
    }
}

int main() {
    while (true) {
        std::cout << "> ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            break;
        }
        if (input == "exit") {
            break;
        }

        std::vector<std::string> args;
        bool silent = false;
        size_t pos = 0;
        if (input.rfind("silent ", 0) == 0) {
            silent = true;
            input = input.substr(7);
        }

        while ((pos = input.find(' ')) != std::string::npos) {
            args.push_back(input.substr(0, pos));
            input.erase(0, pos + 1);
        }
        if (!input.empty()) {
            args.push_back(input);
        }

        if (!args.empty()) {
            execute_command(args, silent);
        }
    }
    return EXIT_SUCCESS;
}

