#pragma once

#include <mutex>
#include <queue>
#include <optional>

template <typename T>
class MailBox
{
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable cv;

public:
    void push(T &elem)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(elem));
        cv.notify_one();
    }

    std::optional<T> try_pop()
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty())
        {
            return std::nullopt;
        }
        else
        {
            T &elem = queue.front();
            queue.pop();
            return elem;
        }
    }

    T &pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&]()
                { return !queue.empty(); });
        T &elem = queue.pop();
        return elem;
    }
};

/*This simulates a mailbox for actors to listen on incoming messages */