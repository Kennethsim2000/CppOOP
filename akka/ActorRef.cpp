#include "ActorRef.hpp"

ActorRef::ActorRef(std::shared_ptr<ActorInstance> actorInstance) : actorInstance_(actorInstance)
{
}

void ActorRef::tell(std::string &message)
{
}

// TODO: Complete ActorRef.cpp implementation