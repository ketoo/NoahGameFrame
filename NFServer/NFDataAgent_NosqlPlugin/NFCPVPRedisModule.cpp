// -------------------------------------------------------------------------
//    @FileName			:    NFCPVPRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPVPRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCPVPRedisModule.h"

NFCPVPRedisModule::NFCPVPRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCPVPRedisModule::Init()
{
	return true;
}

bool NFCPVPRedisModule::Shut()
{
	return true;
}

bool NFCPVPRedisModule::Execute()
{
	return true;
}

bool NFCPVPRedisModule::AfterInit()
{
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	
	return true;
}


bool NFCPVPRedisModule::SavePlayerData(const NFGUID & self)
{
	return false;
}
