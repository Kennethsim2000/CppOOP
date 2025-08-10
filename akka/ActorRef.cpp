#include "ActorRef.hpp"
#include <iostream>

ActorRef::ActorRef(std::shared_ptr<ActorInstance> actorInstance) : actorInstance_(actorInstance)
{
    std::cout << "ActorRef constructor called, actorRef use_count: " << actorInstance_.use_count() << std::endl;
}

void ActorRef::tell(const std::string &message)
{
    actorInstance_->enqueue(message, actorInstance_);
}
