#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file-to-erase>\n";
        return EXIT_FAILURE;
    }

    const char *filepath = argv[1];
    struct stat file_stat;

    if (stat(filepath, &file_stat) != 0) {
        perror("Error accessing file");
        return EXIT_FAILURE;
    }

    int fd = open(filepath, O_WRONLY);
    if (fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("Error seeking file");
        close(fd);
        return EXIT_FAILURE;
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error resetting file pointer");
        close(fd);
        return EXIT_FAILURE;
    }

    char null_byte = '\0';
    for (off_t i = 0; i < file_size; ++i) {
        if (write(fd, &null_byte, 1) != 1) {
            perror("Error overwriting file");
            close(fd);
            return EXIT_FAILURE;
        }
    }

    close(fd);

    if (unlink(filepath) != 0) {
        perror("Error deleting file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

