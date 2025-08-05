#include "ActorSystem.hpp"
#include "Actor.hpp"

#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

class PrinterActor : public Actor
{
public:
    void receive(std::string message) override
    {
    }
};

// TODO: Complete implementation of printer actor

int main()
{
    ActorSystem ActorSystem;
}

// TODO: Testing of actors