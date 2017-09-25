// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Actor.h>
#include <Theron/Framework.h>


namespace Theron
{


Actor::Actor(Framework &framework, const char *const name) :
  mAddress(),
  mFramework(&framework),
  mMessageHandlers(),
  mDefaultHandlers(),
  mMailboxContext(0),
  mMemory(0)
{
    // Claim an available directory index and mailbox for this actor.
    mFramework->RegisterActor(this, name);
}


Actor::~Actor()
{
    mFramework->DeregisterActor(this);
}


void Actor::Fallback(
    Detail::FallbackHandlerCollection *const fallbackHandlers,
    const Detail::IMessage *const message)
{
    // If default handlers are registered with this actor, execute those.
    if (mDefaultHandlers.Handle(this, message))
    {
        return;
    }

    // Let the framework's fallback handlers handle the message.
    fallbackHandlers->Handle(message);
}


} // namespace Theron
