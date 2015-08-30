// -------------------------------------------------------------------------
//    @FileName      :    NFCPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#ifndef _NFC_ACTOR_MANAGER_H_
#define _NFC_ACTOR_MANAGER_H_

#include <map>
#include <string>
#include "NFCActor.h"
#include "NFComm/NFCore/NFIComponent.h"
#include "NFComm/NFCore/NFSingleton.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIActorManager.h"

class NFCActorManager
    : public NFIActorManager,
  public NFSingleton<NFCActorManager>
{
public:
	NFCActorManager();

	virtual bool Init();

	virtual bool AfterInit();

    virtual bool CheckConfig();    

	virtual bool BeforeShut();

	virtual bool Shut();

	virtual bool Execute(const float fLasFrametime, const float fStartedTime);

	virtual void OnReload(const char* strModuleName, NFILogicModule* pModule)
	{
		BeforeShut();

		AfterInit();
	}

	virtual int RequireActor();
	virtual bool ReleaseActor(const int nActorIndex);

	virtual bool SendMsgToActor( const int nActorIndex, const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventFunc);
	virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent);

    virtual NF_SHARE_PTR<NFIActor> GetActor(const int nActorIndex);

private:

    Theron::Framework* m_pFramework;
	NF_SHARE_PTR<NFIActor> m_pMainActor;
	std::map<int, NF_SHARE_PTR<NFIActor> > mxActorMap;
};

#endif