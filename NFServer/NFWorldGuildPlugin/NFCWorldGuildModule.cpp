// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#include "NFWorldGuildPlugin.h"
#include "NFCWorldGuildModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

bool NFCWorldGuildModule::Init()
{


    return true;
}

bool NFCWorldGuildModule::Shut()
{
    return true;
}

bool NFCWorldGuildModule::Execute(const float fLasFrametime, const float fStartedTime)
{


    return true;
}

bool NFCWorldGuildModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));


    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);

    return true;
}

NFIDENTID NFCWorldGuildModule::CreateGuild( const NFIDENTID& self, const std::string& strName )
{
	return NFIDENTID();
}

bool NFCWorldGuildModule::JoinGuild( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
	return false;
}

bool NFCWorldGuildModule::LeaveGuild( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
	return false;

}

bool NFCWorldGuildModule::UpGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
{
	return false;

}

bool NFCWorldGuildModule::DownGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
{
	return false;

}

bool NFCWorldGuildModule::KickGuildMmember( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
{
	return false;

}

bool NFCWorldGuildModule::GetGuildBaseInfo( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
	return false;

}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFIDENTID& self, const NFIDENTID& xGuildID )
{
	return false;

}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember )
{
	return false;

}
