#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number_of_threads>" << std::endl;
        return 1;
    }

    
    int M = std::stoi(argv[1]);
    if (M <= 0)
    {
        std::cerr << "Number of threads must be > 0" << std::endl;
        return 1;
    }

    
    std::ifstream file("random.txt");
    if (!file.is_open())
    {
        std::cerr << "Cannot open file random.txt" << std::endl;
        return 1;
    }

    std::vector<int> numbers;
    numbers.reserve(2'000'000); 
    {
        int value;
        while (file >> value) {
            numbers.push_back(value);
        }
    }
    file.close();

    
    if (numbers.empty())
    {
        std::cerr << "File random.txt is empty or not valid" << std::endl;
        return 1;
    }

    
    size_t numCount = numbers.size();  
    
    auto startSingle = std::chrono::high_resolution_clock::now();
    int maxSingle = *std::max_element(numbers.begin(), numbers.end());

    auto endSingle = std::chrono::high_resolution_clock::now();
    double singleDuration = std::chrono::duration<double>(endSingle - startSingle).count();

    std::vector<int> partialMax(M, 0);

    
    auto findMaxRange = [&](size_t threadIndex, size_t start, size_t end)
    {
        int localMax = numbers[start];
        for (size_t i = start + 1; i < end; ++i)
       	{
            if (numbers[i] > localMax)
	    {
                localMax = numbers[i];
            }
        }
        partialMax[threadIndex] = localMax;
    };

    auto startMulti = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(M);

    size_t chunkSize = numCount / M;
    size_t remainder = numCount % M; 

    size_t currentStart = 0;
    for (int i = 0; i < M; ++i)
    {
        size_t currentEnd = currentStart + chunkSize;
        
        if (remainder > 0)
       	{
            currentEnd++;
            remainder--;
        }
        if (currentEnd > numCount)
       	{
            currentEnd = numCount;
        }

        
        threads.emplace_back(findMaxRange, i, currentStart, currentEnd);

        currentStart = currentEnd;
    }

    
    for (auto &t : threads)
    {
        t.join();
    }

    
    int maxMulti = partialMax[0];
    for (int i = 1; i < M; ++i)
    {
        if (partialMax[i] > maxMulti)
       	{
            maxMulti = partialMax[i];
        }
    }

    auto endMulti = std::chrono::high_resolution_clock::now();
    double multiDuration = std::chrono::duration<double>(endMulti - startMulti).count();

    
    bool resultsMatch = (maxSingle == maxMulti);

    
    std::cout << "Number count: " << numCount << std::endl;
    std::cout << "Maximum value: " << maxSingle << std::endl; 
    std::cout << "Single thread time: " << singleDuration << "s" << std::endl;
    std::cout << "Multiple threads time: " << multiDuration << "s" << std::endl;
    std::cout << "Results match: " << (resultsMatch ? "yes" : "no") << std::endl;

    return 0;
}

