#include <functional>
#include <vector>
#include <thread>
#include <queue>

class Dispatcher : std::enable_shared_from_this<Dispatcher>
{
    std::vector<std::thread> workers_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false; // this is an indicator to the threads to stop working
    std::queue<std::function<void()>> tasks_;

public:
    explicit Dispatcher(int numThreads = 4);
    void submit(std::function<void()>);
    ~Dispatcher();
};

// std::enable_shared_from_this is a C++ utility that allows an object, which is already managed by a std::shared_ptr, to safely obtain a new std::shared_ptr to itself
// from within its own member functions.

// This is the dispatcher, which contains a thread pool, as well as a queue of runnable functions to execute.
