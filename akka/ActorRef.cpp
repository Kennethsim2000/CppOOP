#include "ActorRef.hpp"

ActorRef::ActorRef(std::shared_ptr<ActorInstance> actorInstance) : actorInstance_(std::move(actorInstance))
{
}

void ActorRef::tell(const std::string &message)
{
    actorInstance_->enqueue(message);
}
