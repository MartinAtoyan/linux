#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>

void print_usage(const char* prog_name) {
    std::cerr << "Usage: " << prog_name << " <source-file> <destination-file>" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char* src_path = argv[1];
    const char* dest_path = argv[2];

    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening/creating destination file");
        close(src_fd);
        return EXIT_FAILURE;
    }

    char buffer[4096];
    ssize_t bytes_read, bytes_written;
    off_t total_bytes = 0, total_hole = 0, total_data = 0;

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; ++i) {
            if (buffer[i] == '\0') {
                if (lseek(dest_fd, 1, SEEK_CUR) == -1) {
                    perror("Error creating hole in destination file");
                    close(src_fd);
                    close(dest_fd);
                    return EXIT_FAILURE;
                }
                ++total_hole;
            } else {
                if (write(dest_fd, &buffer[i], 1) != 1) {
                    perror("Error writing to destination file");
                    close(src_fd);
                    close(dest_fd);
                    return EXIT_FAILURE;
                }
                ++total_data;
            }
        }
        total_bytes += bytes_read;
    }

    if (bytes_read == -1) {
        perror("Error reading from source file");
        close(src_fd);
        close(dest_fd);
        return EXIT_FAILURE;
    }

    close(src_fd);
    close(dest_fd);

    std::cout << "Successfully copied " << total_bytes << " bytes (data: " << total_data << ", hole: " << total_hole << ")." << std::endl;

    return EXIT_SUCCESS;
}

