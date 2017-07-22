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
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCActor
    : public NFIActor
{
public:
	NFCActor(Theron::Framework& framework, NFIActorModule* pModule);
	virtual ~NFCActor();

	
    virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent);
	virtual bool AddBeginunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor);
	virtual bool AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xEndFunctor);

    virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message);

protected:
	//handler in component
	virtual void Handler(const NFIActorMessage& message, const Theron::Address from);

	//handler in main thread, the purpose is to push message to main thread
	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);

protected:
	NFMapEx<std::string, NFIComponent> mxComponent;

	NFMapEx<int, ACTOR_PROCESS_FUNCTOR> mxProcessFuntor;
	NFMapEx<int, ACTOR_PROCESS_FUNCTOR> mxEndProcessFuntor;

	ACTOR_PROCESS_FUNCTOR_PTR mxDefaultEndProcessFuntor;
};
#endif