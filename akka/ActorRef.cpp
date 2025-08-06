#include "ActorRef.hpp"

ActorRef::ActorRef(std::shared_ptr<ActorInstance> actorInstance) : actorInstance_(std::move(actorInstance))
{
}

void ActorRef::tell(std::string &message)
{
    actorInstance_->enqueue(message);
}
