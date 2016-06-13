// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFLoginLogicPlugin.h"
#include "NFCLoginLogicModule.h"


bool NFCLoginLogicModule::Init()
{

    return true;
}

bool NFCLoginLogicModule::Shut()
{
    return true;
}

int NFCLoginLogicModule::OnLoginProcess(const NFGUID& object, const std::string& strAccount, const std::string& strPwd)
{
	return 0;
}

bool NFCLoginLogicModule::Execute()
{


    return true;
}


bool NFCLoginLogicModule::AfterInit()
{
    m_pLoginNet_ServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>();

    return true;
}
