// -------------------------------------------------------------------------
//    @FileName			:    NFCActorModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCActorModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ACTOR_MANAGER_H
#define NFC_ACTOR_MANAGER_H

#include <map>
#include <string>
#include "NFCActor.h"
#include "Theron/Theron.h"
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFCore/NFQueue.hpp"

class NFCActorModule
    : public NFIActorModule
{
public:
	NFCActorModule(NFIPluginManager* p);
    virtual ~NFCActorModule();

    virtual bool Init();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Shut();

    virtual bool Execute();

	virtual int RequireActor();

    virtual bool SendMsgToActor(const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg);

	virtual bool HandlerEx(const NFIActorMessage& message, const int from);

	virtual bool ReleaseActor(const int nActorIndex);

	virtual NF_SHARE_PTR<NFIActor> GetActor(const int nActorIndex);

protected:

	virtual bool AddEndFunc(const int nActorIndex, const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr);

    virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent);

	virtual bool ExecuteEvent();
private:
	NF_SHARE_PTR<NFIActor> m_pMainActor;

    Theron::Framework mFramework;
    NFMapEx<int, NFIActor> mxActorMap;

	NFQueue<NFIActorMessage> mxQueue;
};

#endif
