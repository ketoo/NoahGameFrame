// -------------------------------------------------------------------------
//    @FileName			:    NFCHomeModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-01-21
//    @Module           :    NFCHomeModule
//
// -------------------------------------------------------------------------

#ifndef NFC_HOME_MODULE_H
#define NFC_HOME_MODULE_H

#include "NFComm/NFCore/NFConsistentHash.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFICreateRoleModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIHomeModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIRankModule.h"

class NFCHomeModule
	: public NFIHomeModule
{
public:
	NFCHomeModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFCHomeModule() {};

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();


	virtual void Relive(const NFGUID& self);

protected:

	void NFCHomeModule::OnRquireReliveProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFIScheduleModule* m_pScheduleModule;
	NFINetModule* m_pNetModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFISceneAOIModule* m_pSceneAOIModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFILogModule* m_pLogModule;
	NFIRankModule* m_pRankModule;
};

#endif