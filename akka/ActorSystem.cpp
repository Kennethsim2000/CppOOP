#include "ActorSystem.hpp"
#include <memory>
#include <iostream>

ActorSystem::ActorSystem()
{
    dispatcher_ = std::make_shared<Dispatcher>(4);
}

ActorRef ActorSystem::spawn(std::shared_ptr<Actor> actor)
{
    auto actorInstance = std::make_shared<ActorInstance>(dispatcher_, std::move(actor));

    ActorRef actorRef(actorInstance);
    return actorRef;
}

// Why do we move the actor and not the dispatcher_? Because dispatcher_ is a member variable of ActorSystem, and you likely want to keep it alive as long as the system lives. If we move,
// the reference count of the shared pointer stays the same, meaning that this dispatcher may be freed once the ActorRef terminates.
// You should only std::move a member field if you no longer need it afterward. In this case, an actor system can be used to spawn multiple actors, so we should pass copies of the dispatcher.
