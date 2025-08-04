#include "Dispatcher.hpp"

// TODO: Implement dispatcher methods
Dispatcher::Dispatcher(int numThreads)
{
}

void Dispatcher::submit(std::function<void()>)
{
}

// Destructor, make sure to free all threads
Dispatcher::~Dispatcher()
{
}
// The :: in C++ is the scope resolution operator. It tells the compiler where to look for a name — such as a class member, function, constant, or type — by
// specifying its namespace or class context.

// This dispatcher is in charge of dispatching threads to execute functions, similar to a thread pool that spawns threads that busy waits on the queue for tasks submitted to the
// queue.