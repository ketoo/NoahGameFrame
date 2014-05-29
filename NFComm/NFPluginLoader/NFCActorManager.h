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
#include "NFComm/NFCore/NFSingleton.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIActorManager.h"

class NFCActorManager
    : public NFIActorManager,
  public NFSingleton<NFCActorManager>
{
public:

	virtual bool Init();

	virtual bool AfterInit();

	virtual bool BeforeShut();

	virtual bool Shut();

	virtual bool Execute(const float fLasFrametime, const float fStartedTime);

	virtual void OnReload(const char* strModuleName, NFILogicModule* pModule)
	{
		BeforeShut();

		AfterInit();
	}

	virtual void OnCheckConfig()
	{

	}


	virtual const Theron::Address GetAddress( NFIActorManager::EACTOR eActor);

private:
	std::vector<NFIActor*> mActorVec;
    Theron::Framework* m_pFramework;
	//Theron::Framework framework; // ªª÷∏’Î
};

#endif