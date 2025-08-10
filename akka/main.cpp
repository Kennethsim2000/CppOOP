#include "ActorSystem.hpp"
#include "Actor.hpp"

#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <iostream>

class PrinterActor : public Actor
{
public:
    void receive(std::string message) override
    {
        std::cout << "Printer actor received " << message << std::endl;
    }
};

int main()
{
    ActorSystem actorSystem;
    auto sharedPrinter = std::make_shared<PrinterActor>();
    ActorRef printerOne = actorSystem.spawn(sharedPrinter);
    std::cout << "=== Spawning printerOne ===" << std::endl;
    // ActorRef printerTwo = actorSystem.spawn(sharedPrinter);
    // std::cout << "=== Spawning printerTwo ===" << std::endl;

    printerOne.tell("hello to printer one");
    // printerTwo.tell("hello to printer two");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
