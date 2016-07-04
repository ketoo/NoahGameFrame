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
#include "NFComm/NFCore/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

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

    virtual bool SendMsgToActor(const int nActorIndex, const NFGUID& objectID, const int nEventID, const std::string& strArg);

	virtual bool HandlerEx(const NFIActorMessage& message, const Theron::Address from);

protected:
    virtual int RequireActor();
    virtual bool ReleaseActor(const int nActorIndex);
	virtual NF_SHARE_PTR<NFIActor> GetActor(const int nActorIndex);

    virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent);
    virtual bool AddEndFunc(const int nActorIndex, EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end);

private:

    Theron::Framework* m_pFramework;
    NF_SHARE_PTR<NFIActor> m_pMainActor;
    std::map<int, NF_SHARE_PTR<NFIActor> > mxActorMap;
};

#endif
