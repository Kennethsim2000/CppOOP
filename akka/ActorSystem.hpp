#pragma once

#include "Dispatcher.hpp"
#include "ActorRef.hpp"
#include "Actor.hpp"
#include <memory>

class ActorSystem
{

    std::shared_ptr<Dispatcher> dispatcher_; // Shared_ptr here because multiple objects require access to the dispatcher

public:
    ActorSystem();
    ActorRef spawn(std::shared_ptr<Actor> actor);
};

// This is the actor system, and has a method called spawn that takes in an Actor and returns an ActorRef
// This is the main system where we can use to spawn multiple ActorRef, basically references to our actor instances.