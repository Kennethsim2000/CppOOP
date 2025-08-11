#include "ActorSystem.hpp"
#include "Actor.hpp"
#include "Globals.hpp"

#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <iostream>

std::mutex globals::cout_mutex;

class PrinterActor : public Actor
{
public:
    void receive(std::string message) override
    {
        std::lock_guard<std::mutex> lock(globals::cout_mutex);
        std::cout << "Printer actor received " << message << std::endl;
    }
};

int main()
{
    ActorSystem actorSystem;
    auto sharedPrinter = std::make_shared<PrinterActor>();
    ActorRef printerOne = actorSystem.spawn(sharedPrinter);
    std::cout << "=== Spawning printerOne ===" << std::endl;
    ActorRef printerTwo = actorSystem.spawn(sharedPrinter);
    std::cout << "=== Spawning printerTwo ===" << std::endl;

    printerOne.tell("hello to printer one");
    printerTwo.tell("hello to printer two");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
