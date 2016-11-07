// -------------------------------------------------------------------------
//    @FileName			:    NFCAccountRedisModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-11-07
//    @Module           :    NFCAccountRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCAccountRedisModule.h"

NFCAccountRedisModule::NFCAccountRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCAccountRedisModule::Init()
{
	return true;
}

bool NFCAccountRedisModule::Shut()
{
	return true;
}

bool NFCAccountRedisModule::Execute()
{
	return true;
}

bool NFCAccountRedisModule::AfterInit()
{
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();

	return true;
}


int NFCAccountRedisModule::VerifyAccount(const std::string& strAccount, const std::string& strPwd)
{
	return 0;
}
