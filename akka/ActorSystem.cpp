#include "ActorSystem.hpp"
#include <memory>
#include <iostream>

ActorSystem::ActorSystem()
{
    dispatcher_ = std::make_shared<Dispatcher>(4);
}

ActorRef ActorSystem::spawn(std::shared_ptr<Actor> actor)
{
    std::cout << "actorSystem spawn() called" << std::endl;
    auto actorInstance = std::make_shared<ActorInstance>(dispatcher_, std::move(actor));
    std::weak_ptr<ActorInstance> weak_check = actorInstance;
    std::cout << "ActorInstance created at address: " << actorInstance.get()
              << ", use_count: " << actorInstance.use_count() << std::endl;

    // Ensure that the shared_ptr is properly established
    if (weak_check.expired())
    {
        std::cout << "Warning: weak_ptr check failed" << std::endl;
    }

    ActorRef actorRef(actorInstance);
    std::cout << "ActorRef created, ActorInstance use_count: " << actorInstance.use_count() << std::endl;

    return actorRef;
}

// Why do we move the actor and not the dispatcher_? Because dispatcher_ is a member variable of ActorSystem, and you likely want to keep it alive as long as the system lives. If we move,
// the reference count of the shared pointer stays the same, meaning that this dispatcher may be freed once the ActorRef terminates.
// You should only std::move a member field if you no longer need it afterward. In this case, an actor system can be used to spawn multiple actors, so we should pass copies of the dispatcher.
