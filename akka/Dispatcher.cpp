#include "Dispatcher.hpp"

// TODO: Implement dispatcher methods
Dispatcher::Dispatcher(int numThreads)
{
    auto threadLambda = [&]() { // thread should be waiting on the condition variable
        while (true)            // the thread must be constantly polling tasks from the task pool
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [&]()
                         { return !tasks_.empty() || stop_; }); // if either there is a task or the thread has been ordered to stop
                if (stop_ && tasks_.empty())                    // all tasks have been completed and received the stop_ signal
                {
                    return;
                }
                task = std::move(tasks_.front());
                tasks_.pop();
            }
            task();
        }

    };

    for (int i = 0; i < numThreads; i++)
    {
        workers_.emplace_back(std::thread(threadLambda));
    }
}

void Dispatcher::submit(std::function<void()> task) // submit function handles concurrent adding of tasks
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(task);
    }
    cv_.notify_one();
}

// Destructor, make sure to join all threads
Dispatcher::~Dispatcher()
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto &worker : workers_)
    {
        worker.join();
    }
}
// The :: in C++ is the scope resolution operator. It tells the compiler where to look for a name — such as a class member, function, constant, or type — by
// specifying its namespace or class context.

// This dispatcher is in charge of dispatching threads to execute functions, similar to a thread pool that spawns threads that busy waits on the queue for tasks submitted to the
// queue.