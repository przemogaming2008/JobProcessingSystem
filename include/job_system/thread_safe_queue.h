#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T value);
    T waitAndPop();
    bool empty() const;

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
};

template <typename T>
bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

template <typename T>
void ThreadSafeQueue<T>::push(T value)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
    }

    condition_.notify_one();
}

template <typename T>
T ThreadSafeQueue<T>::waitAndPop()
{
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(
        lock,
        [this]()
        {
            return !queue_.empty();
        }
    );

    T value = std::move(queue_.front());
    queue_.pop();

    return value;
}