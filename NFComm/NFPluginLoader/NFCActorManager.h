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

	virtual NFIPluginManager* GetPluginManager();

	virtual int OnRequireActor(const NF_SHARE_PTR<NFIComponent> pComponent);
	virtual bool OnRequireCPUCycle( const int nActorIndex, const NFIDENTID& objectID, const int nEventID, const std::string& strArg, const NF_SHARE_PTR<NFAsyncEventFunc> xActorEventFunc);
    virtual NFIActor* GetActor(const int nActorIndex);

private:
	NFIPluginManager* m_pPluginManager;

    Theron::Framework* m_pFramework;
	NFIActor* m_pMainActor;
	std::vector<NFIActor*> mvActorList;
	std::map<int, NF_SHARE_PTR<NFIActor>> mxActorMap;

};

#endif