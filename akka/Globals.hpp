#pragma once
#include <iostream>
#include <mutex>

namespace globals // namespace are used to group related variables/functions together to avoid collision
{
    extern std::mutex cout_mutex; // extern means that this is a declaration, not a definition.
    // this line does not create the mutex, it just says that in a .cpp file, there will be std::mutex cout_mutex
}