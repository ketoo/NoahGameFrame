// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCGuildRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCGuildRedisModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"

NFCGuildRedisModule::NFCGuildRedisModule(NFIPluginManager * p)
{
    pPluginManager = p;
}

bool NFCGuildRedisModule::Init()
{
    return true;
}

bool NFCGuildRedisModule::Shut()
{
    return true;
}

bool NFCGuildRedisModule::Execute()
{
    return true;
}

bool NFCGuildRedisModule::AfterInit()
{
    m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
    m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

    return true;
}

bool NFCGuildRedisModule::ExistGuild(const std::string & strName)
{
	return false;
}

bool NFCGuildRedisModule::CreateGuild(const NFGUID & self, const std::string & strGuildName, const NFGUID & xCreater)
{
	return false;
}


