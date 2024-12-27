#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

class shared_array {
private:
    int* data;
    size_t size;
    std::string name;
    sem_t* semaphore;

public:
    shared_array(const std::string& array_name, size_t array_size) : name(array_name), size(array_size) {
        if (array_size < 1 || array_size > 1000000000) {
            throw std::out_of_range("Array size must be between 1 and 1,000,000,000");
        }

        int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
            throw std::runtime_error("Failed to create shared memory");
        }

        if (ftruncate(fd, size * sizeof(int)) == -1) {
            close(fd);
            throw std::runtime_error("Failed to set shared memory size");
        }

        data = static_cast<int*>(mmap(nullptr, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        close(fd);

        if (data == MAP_FAILED) {
            throw std::runtime_error("Failed to map shared memory");
        }

        semaphore = sem_open((name + "_sem").c_str(), O_CREAT, 0666, 1);
        if (semaphore == SEM_FAILED) {
            munmap(data, size * sizeof(int));
            throw std::runtime_error("Failed to create semaphore");
        }
    }

    ~shared_array() {
        munmap(data, size * sizeof(int));
        sem_close(semaphore);
        sem_unlink((name + "_sem").c_str());
        shm_unlink(name.c_str());
    }

    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void lock() {
        sem_wait(semaphore);
    }

    void unlock() {
        sem_post(semaphore);
    }

    size_t get_size() const {
        return size;
    }
};

void first_process(shared_array& array) {
    size_t size = array.get_size();
    for (size_t i = 0; i < size; ++i) {
        array.lock();
        array[i] = static_cast<int>(i * 2);
        std::cout << "[First] Writing " << array[i] << " at index " << i << std::endl;
        array.unlock();
        sleep(1);
    }
}

void second_process(shared_array& array) {
    size_t size = array.get_size();
    for (size_t i = 0; i < size; ++i) {
        array.lock();
        int value = array[i];
        std::cout << "[Second] Reading " << value << " from index " << i << std::endl;
        array.unlock();
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <array_name> <array_size> <process_type>\n";
        return EXIT_FAILURE;
    }

    std::string array_name = argv[1];
    size_t array_size = std::stoull(argv[2]);
    std::string process_type = argv[3];

    try {
        shared_array array(array_name, array_size);

        if (process_type == "first") {
            first_process(array);
        } else if (process_type == "second") {
            second_process(array);
        } else {
            std::cerr << "Invalid process type. Use 'first' or 'second'.\n";
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

