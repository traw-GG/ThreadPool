#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

class TaskQueue {
public:
    void push(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.push(task);
        m_condition.notify_one();
    }

    std::function<void()> pop() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return !m_tasks.empty(); });
        auto task = m_tasks.front();
        m_tasks.pop();
        return task;
    }

private:
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : m_stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            m_workers.emplace_back([this] {
                while (true) {
                    auto task = m_taskQueue.pop();
                    if (m_stop) return;
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stop = true;
        }
        m_condition.notify_all();
        for (std::thread &worker : m_workers) {
            worker.join();
        }
    }

    void enqueue(std::function<void()> task) {
        m_taskQueue.push(task);
    }

private:
    std::vector<std::thread> m_workers;
    TaskQueue m_taskQueue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_stop;
};