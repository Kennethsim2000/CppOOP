cmake_minimum_required(VERSION 3.16)
project(ActorSystem)

set(CMAKE_CXX_STANDARD 17) # tells cmake to compile with c++ 17

# These create CMAKE boolean flags you can pass from the command line
option(ENABLE_ASAN "Enable AddressSanitizer" OFF) # -DENABLE_ASAN=ON turns on address sanitisers for memory errors, leaks, use after free
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF) # -DENABLE_TSAN=ON turns on thread sanitisers

if (ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")
    set(SANITIZER_FLAGS -fsanitize=address -fno-omit-frame-pointer)
    # -fno-omit-frame-pointer keeps frame pointers in the compiled code, which makes sanitizer stack traces readable.

elseif (ENABLE_TSAN)
    message(STATUS "ThreadSanitizer enabled")
    set(SANITIZER_FLAGS -fsanitize=thread -fno-omit-frame-pointer)
endif()

add_executable(ActorSystem
    main.cpp
    Actor.hpp
    ActorRef.hpp ActorRef.cpp
    Mailbox.hpp
    Globals.hpp
    Dispatcher.hpp Dispatcher.cpp
    ActorInstance.hpp ActorInstance.cpp
    ActorSystem.hpp ActorSystem.cpp
)

target_compile_options(ActorSystem PRIVATE ${SANITIZER_FLAGS})
target_link_options(ActorSystem PRIVATE ${SANITIZER_FLAGS})

# rm -rf build
# mkdir build && cd build
# cmake -DENABLE_ASAN=ON -DCMAKE_BUILD_TYPE=Debug ../CMakeLists.txt 
# cmake -DENABLE_TSAN=ON -DCMAKE_BUILD_TYPE=Debug ../CMakeLists.txt 

# compile with address sanitisers