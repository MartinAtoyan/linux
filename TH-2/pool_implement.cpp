#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class parallel_scheduler {
public:
    parallel_scheduler(size_t capacity) : capacity_(capacity), stop_(false) {
        for (size_t i = 0; i < capacity_; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
                        condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) {
                            return;
                        }
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    ~parallel_scheduler() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        for (std::thread& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    template <typename Function, typename Argument>
    void run(Function&& func, Argument&& arg) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            tasks_.emplace([=]() { func(arg); });
        }
        condition_.notify_one();
    }

private:
    size_t capacity_;
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};

// Demo Application
#ifdef DEMO
void print_number(int n) {
    std::cout << "Processing number: " << n << " in thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    parallel_scheduler scheduler(4);

    for (int i = 0; i < 10; ++i) {
        scheduler.run(print_number, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(3)); // Allow tasks to complete
    return 0;
}
#endif

