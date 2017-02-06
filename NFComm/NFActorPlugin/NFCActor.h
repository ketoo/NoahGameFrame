// -------------------------------------------------------------------------
//    @FileName			:    NFIActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFIActor
//
// -------------------------------------------------------------------------

#ifndef NFC_ACTOR_H
#define NFC_ACTOR_H

#include <map>
#include <string>
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFCore/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCActor
    : public NFIActor
{
public:
    NFCActor(Theron::Framework& framework, NFIActorModule* pManager) : NFIActor(framework, pManager)
    {

    }

    ~NFCActor()
    {
		if (mxComponent)
		{
			mxComponent->BeforeShut();
			mxComponent->Shut();
		}
    }

	//handler in main thread, the purpose is to push message to main thread
    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);

	//hangler in component
    virtual void HandlerSelf(const NFIActorMessage& message, const Theron::Address from);

    virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent);
    virtual bool AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end);
    virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message);


protected:
    NF_SHARE_PTR<NFIComponent> mxComponent;
    EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR mxFunctorEndPtr;
};
#endif