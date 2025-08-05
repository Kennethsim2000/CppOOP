#include "ActorInstance.hpp"
#include <functional>

ActorInstance::ActorInstance(std::shared_ptr<Dispatcher> dispatcher, std::shared_ptr<Actor> actor) : dispatcher_(std::move(dispatcher)), actor_(std::move(actor))
{
}

void ActorInstance::enqueue(std::string &message)
{
    mailbox_.push(message);
    if (!scheduled_.exchange(true)) // if it is not previously scheduled
    {
        std::function<void()> func = [self = shared_from_this()]() // contains a reference to the ActorInstance
        {
            self->process_message();
        };
        dispatcher_->submit(func);
    }
}

// TODO: complete process_message()
void ActorInstance::process_message()
{
    std::string message;
    while (mailbox_.try_pop(message))
    {
        // TODO: complete
    }
}

// Why shared_from_this()?
// When you're in a multithreaded actor system and you submit a task (like process_messages) to be executed asynchronously (possibly on a different thread), there's a risk:
// The ActorInstance object may be destroyed before process_messages runs!
// If that happens and you're using [this] inside your lambda, you now have a dangling pointer — leading to undefined behavior or segmentation faults.
// By capturing a shared_ptr to this using shared_from_this():, you're telling the system:
// “Keep this object alive at least until this lambda is done executing.”
// The captured shared_ptr ensures the object won't be deleted while the lambda is still referencing it.