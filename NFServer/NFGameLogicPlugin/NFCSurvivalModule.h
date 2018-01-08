// -------------------------------------------------------------------------
//    @FileName			:    NFCSurvivalModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-29
//    @Module           :    NFCSurvivalModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SURVIVAL_MODULE_H
#define NFC_SURVIVAL_MODULE_H

#include "NFComm/NFCore/NFConsistentHash.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFICreateRoleModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFISurvivalModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIRankModule.h"

class NFCSurvivalModule
    : public NFISurvivalModule
{
public:
    NFCSurvivalModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCSurvivalModule() {};

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

protected:
	int CheckSurvivalList(const std::string& strScheduleName, const float fIntervalTime, const int nCount);


private:
    int mnMinPeopleForSurvivalMode;
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