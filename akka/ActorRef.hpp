#pragma once
#include "ActorInstance.hpp"

class ActorRef
{
    std::shared_ptr<ActorInstance> actorInstance_;

public:
    explicit ActorRef(std::shared_ptr<ActorInstance> actorInstance);
    void tell(const std::string &message);
};

// ActorRef is a reference to an actorInstance. This provides an interface for us to create reference to actor instances, and to pass messages to it.