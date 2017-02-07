// -------------------------------------------------------------------------
//    @FileName			:    NFIActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActor
//
// -------------------------------------------------------------------------

#ifndef NFI_ACTOR_H
#define NFI_ACTOR_H

#define THERON_USE_STD_THREADS 1

#include <map>
#include <string>
#include <Theron/Theron.h>
#include "NFIModule.h"
#include "NFIActorModule.h"
#include "NFComm/NFCore/NFIComponent.h"

class NFIActor : public Theron::Actor, public NFIModule
{
public:
    NFIActor(Theron::Framework& framework, NFIActorModule* pManager) : Theron::Actor(framework)
    {
        m_pActorModule = pManager;

        RegisterHandler(this, &NFIActor::Handler);
    }

	NFIActorModule* GetActorManager()
    {
        return m_pActorModule;
    }

    virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent) = 0;
    virtual bool AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end) = 0;
    virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message) = 0;

protected:

    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from) {};
    virtual void HandlerSelf(const NFIActorMessage& message, const Theron::Address from) {};

private:
    void Handler(const NFIActorMessage& message, const Theron::Address from)
    {
        if (message.bComponentMsg > 0 )
        {
            HandlerSelf(message, from);

        }
        else
        {
            HandlerEx(message, from);
        }
    }

protected:
	NFIActorModule* m_pActorModule;

};

#endif // !NFI_ACTOR_H