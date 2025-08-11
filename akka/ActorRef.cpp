#include "ActorRef.hpp"
#include <iostream>

ActorRef::ActorRef(std::shared_ptr<ActorInstance> actorInstance) : actorInstance_(actorInstance)
{
}

void ActorRef::tell(const std::string &message)
{
    actorInstance_->enqueue(message, actorInstance_);
}
