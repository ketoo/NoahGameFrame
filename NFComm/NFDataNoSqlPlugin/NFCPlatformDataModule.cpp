// -------------------------------------------------------------------------
//    @FileName      :    NFCPlatformDataModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPlatformDataModule
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <algorithm>
#include "NFCPlatformDataModule.h"

NFCPlatformDataModule::NFCPlatformDataModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

NFCPlatformDataModule::~NFCPlatformDataModule()
{

}

bool NFCPlatformDataModule::Init()
{

    return true;
}

bool NFCPlatformDataModule::Shut()
{
    return true;
}

bool NFCPlatformDataModule::Execute( const float fLasFrametime, const float fStartedTime )
{
	return true;
}

bool NFCPlatformDataModule::AfterInit()
{
	return true;
}

int NFCPlatformDataModule::AddAccountInfo( const std::string& strAccount, const std::string& strPassword )
{
	return 0;
}

int NFCPlatformDataModule::ConfirmAccountInfo( const std::string& strAccount, const std::string& strPassword )
{
	return 0;

}

int NFCPlatformDataModule::SetAccountState( const std::string& strAccount, const int nState )
{
	return 0;

}
