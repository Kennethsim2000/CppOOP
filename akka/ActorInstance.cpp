#include "ActorInstance.hpp"
#include <functional>
#include <iostream>

ActorInstance::ActorInstance(std::shared_ptr<Dispatcher> dispatcher, std::shared_ptr<Actor> actor) : dispatcher_(std::move(dispatcher)), actor_(std::move(actor))
{
}

void ActorInstance::enqueue(const std::string &message, std::shared_ptr<ActorInstance> self)
{
    mailbox_.push(message);
    if (!scheduled_.exchange(true)) // if it is not previously scheduled
    {

        try
        {
            std::function<void()> func = [self]()
            {
                self->process_message();
            };
            dispatcher_->submit(func);
        }
        catch (const std::bad_weak_ptr &e)
        {
            scheduled_ = false;
            std::cout << "error: " << e.what() << std::endl;
        }
    }
}

// TODO: complete process_message()
void ActorInstance::process_message()
{
    std::string message;
    while (mailbox_.try_pop(message)) // process all the messages in the mailbox
    {
        actor_->receive(message);
    }
    scheduled_ = false;
    if (!mailbox_.try_pop(message)) // check if new messages arrive after setting our scheduled to false
    {                               // mailbox is empty
        return;
    }
    // mailbox is not empty, new messages have arrived while setting scheduled to false, we need to set scheduled to true, and submit a new dispatcher task
    if (!scheduled_.exchange(true))
    { // if previously not scheduled, we set scheduled to true and submit a new task to dispatcher
        dispatcher_->submit([self = shared_from_this()]()
                            { self->process_message(); });
    }
}

// Why shared_from_this()?
// When you're in a multithreaded actor system and you submit a task (like process_messages) to be executed asynchronously (possibly on a different thread), there's a risk:
// The ActorInstance object may be destroyed before process_messages runs!
// If that happens and you're using [this] inside your lambda, you now have a dangling pointer — leading to undefined behavior or segmentation faults.
// By capturing a shared_ptr to this using shared_from_this():, you're telling the system:
// “Keep this object alive at least until this lambda is done executing.”
// The captured shared_ptr ensures the object won't be deleted while the lambda is still referencing it.