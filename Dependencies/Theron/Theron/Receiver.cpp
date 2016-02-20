// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Defines.h>

// Must include xs.h before standard headers to avoid warnings in MS headers!
#if THERON_XS
#include <xs/xs.h>
#endif // THERON_XS

#include <Theron/EndPoint.h>
#include <Theron/Receiver.h>

#include <Theron/Detail/Directory/StaticDirectory.h>
#include <Theron/Detail/Network/Index.h>
#include <Theron/Detail/Network/NameGenerator.h>
#include <Theron/Detail/Strings/String.h>


namespace Theron
{


Receiver::Receiver() :
  mStringPoolRef(),
  mEndPoint(0),
  mName(),
  mAddress(),
  mMessageHandlers(),
  mCondition(),
  mMessagesReceived(0)
{
    Initialize();
}


Receiver::Receiver(EndPoint &endPoint, const char *const name) :
  mStringPoolRef(),
  mEndPoint(&endPoint),
  mName(name),
  mAddress(),
  mMessageHandlers(),
  mCondition(),
  mMessagesReceived(0)
{
    Initialize();
}


Receiver::~Receiver()
{
    Release();
}


void Receiver::Initialize()
{
    // Register this receiver, claiming a unique address for this receiver.
    const uint32_t receiverIndex(Detail::StaticDirectory<Receiver>::Register(this));

    if (mName.IsNull())
    {
        char rawName[16];
        Detail::NameGenerator::Generate(rawName, receiverIndex);

        const char *endPointName(0);
        if (mEndPoint)
        {
            endPointName = mEndPoint->GetName();
        }

        char scopedName[256];
        Detail::NameGenerator::Combine(
            scopedName,
            256,
            rawName,
            0,
            endPointName);

        mName = Detail::String(scopedName);
    }

    // Receivers are identified as a receiver by a framework index of zero.
    // All frameworks have non-zero indices, so all actors have non-zero framework indices.
    const Detail::Index index(0, receiverIndex);
    mAddress = Address(mName, index);

    // Register the receiver at its claimed address.
    Detail::Entry &entry(Detail::StaticDirectory<Receiver>::GetEntry(mAddress.AsInteger()));

    entry.Lock();
    entry.SetEntity(this);
    entry.Unlock();

    if (mEndPoint)
    {
        // Check that no mailbox with this name already exists.
        Detail::Index dummy;
        if (mEndPoint->Lookup(mName, dummy))
        {
            THERON_FAIL_MSG("Can't create two actors or receivers with the same name");
        }

        // Register the receiver 'mailbox' with the endPoint so it can be found using its name.
        if (!mEndPoint->Register(mName, index))
        {
            THERON_FAIL_MSG("Failed to register receiver with the network endpoint");
        }
    }
}


void Receiver::Release()
{
    const Address &address(GetAddress());

    // Deregister the receiver 'mailbox' with the endPoint so it can't be found anymore.
    if (mEndPoint)
    {
        mEndPoint->Deregister(address.GetName());
    }

    // Deregister the receiver, so that the worker threads will leave it alone.
    Detail::StaticDirectory<Receiver>::Deregister(address.AsInteger());

    mCondition.GetMutex().Lock();

    // Free all currently allocated handler objects.
    while (Detail::IReceiverHandler *const handler = mMessageHandlers.Front())
    {
        mMessageHandlers.Remove(handler);
        AllocatorManager::GetCache()->Free(handler);
    }

    mCondition.GetMutex().Unlock();
}


} // namespace Theron


