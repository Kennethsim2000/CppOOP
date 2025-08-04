#pragma once
#include <string>

class Actor
{

public:
    virtual void receive(std::string message) = 0;
    virtual ~Actor() = default;
    // If you delete a derived class via a Actor* pointer, the derived class’s destructor will still be called correctly.
};

// this is an abstract class Actor, which have one abstract method receive to determine what to do after receiving the message
