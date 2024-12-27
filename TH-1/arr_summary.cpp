#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <numeric>

void sum_array_section(const std::vector<int>& array, size_t start, size_t end, long long& result) {
    result = 0;
    for (size_t i = start; i < end; ++i) {
        result += array[i];
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <M>" << std::endl;
        return EXIT_FAILURE;
    }

    size_t N = std::stoull(argv[1]);
    size_t M = std::stoull(argv[2]);

    if (N <= 1000000 || M == 0) {
        std::cerr << "N must be > 1,000,000 and M must be > 0." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<int> array(N);
    for (size_t i = 0; i < N; ++i) {
        array[i] = rand() % 100;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    long long total_sum = std::accumulate(array.begin(), array.end(), 0LL);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_without_threads = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    std::vector<long long> partial_sums(M, 0);
    size_t chunk_size = N / M;

    for (size_t i = 0; i < M; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == M - 1) ? N : start + chunk_size;
        threads.emplace_back(sum_array_section, std::cref(array), start, end, std::ref(partial_sums[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    long long threaded_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
    end_time = std::chrono::high_resolution_clock::now();
    auto duration_with_threads = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Time spent without threads: " << duration_without_threads << " ms" << std::endl;
    std::cout << "Time spent with " << M << " threads: " << duration_with_threads << " ms" << std::endl;

    return EXIT_SUCCESS;
}

