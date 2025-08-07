cmake_minimum_required(VERSION 3.16)
project(ActorSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(ActorSystem
    main.cpp
    Actor.hpp
    ActorRef.hpp ActorRef.cpp
    Mailbox.hpp
    Dispatcher.hpp Dispatcher.cpp
    ActorInstance.hpp ActorInstance.cpp
    ActorSystem.hpp ActorSystem.cpp
)
