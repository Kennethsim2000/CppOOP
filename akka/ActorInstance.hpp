#pragma once
#include "Mailbox.hpp"
#include "Actor.hpp"
#include "Dispatcher.hpp"

class ActorInstance : std::enable_shared_from_this<ActorInstance>
{
    MailBox<std::string> mailbox_;
    std::shared_ptr<Actor> actor_;
    std::shared_ptr<Dispatcher> dispatcher_;
    std::atomic<bool> scheduled_ = false;

public:
    ActorInstance(std::shared_ptr<Dispatcher> dispatcher, std::shared_ptr<Actor> actor);
    void enqueue(const std::string &message);
    void process_message();
};

// ActorInstance should contain a mailbox, an actor, and a dispatcher
// Design choices:
// for mailbox, we do not need to use any pointer because this ActorInstance fully owns it
// for dispatcher and actor, we use a shared pointer because multiple actorInstance have to access the same dispatcher, which is a thread pool. Actor is an abstract class, so we can have
// multiple actor instances referencing the same Actor.

//