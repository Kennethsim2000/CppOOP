#pragma once
#include <iostream>
#include <mutex>

namespace globals
{
    extern std::mutex cout_mutex;
}