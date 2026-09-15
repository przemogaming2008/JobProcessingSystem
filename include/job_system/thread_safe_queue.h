#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
#include <optional>

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T value);
    std::optional<T> waitAndPop();
    void close();
    bool empty() const;

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    bool closed_ = false;
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
std::optional<T> ThreadSafeQueue<T>::waitAndPop()
{
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(
        lock,
        [this]()
        {
            return closed_ || !queue_.empty();
        }
    );

    if (queue_.empty())
    {
        return std::nullopt;
    }

    T value = std::move(queue_.front());
    queue_.pop();

    return value;
}

template <typename T>
void ThreadSafeQueue<T>::close()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }

    condition_.notify_all();
}