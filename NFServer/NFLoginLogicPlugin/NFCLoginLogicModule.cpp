// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
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

int NFCLoginLogicModule::OnLoginEvent(const NFGUID& object, const int nEventID, const NFIDataList& var)
{
    if (3 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////
    const NFGUID xIdent = var.Object(0);
    const std::string& strAccount = var.String(1);
    const std::string& strPassword = var.String(2);
	int64_t nState = -1;

	std::vector<std::string> xFieldVec;
	std::vector<std::string> xValueVec;
	xFieldVec.push_back("Password");

	if (m_pClusterSqlModule->Query("AccountInfo", strAccount, xFieldVec, xValueVec)
		&& xFieldVec.size() == xValueVec.size()
		&& strPassword == xValueVec[0])
	{
		//µÇÂ¼³É¹¦
		nState = 0;
	}

    NFCDataList valEventInfo;
    valEventInfo << nState << xIdent << strAccount;
    m_pEventProcessModule->DoEvent(NFGUID(), NFED_ON_CLIENT_LOGIN_RESULTS, valEventInfo);

    return 0;
}

int NFCLoginLogicModule::OnDisconnectEvent(const NFGUID& object, const int nEventID, const NFIDataList& var)
{
    if (2 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return -1;
    }

    //std::string strAccount = var.String(0);

    return 0;
}

bool NFCLoginLogicModule::Execute()
{


    return true;
}


bool NFCLoginLogicModule::AfterInit()
{



    //////////////////////////////////////////////////////////////////////////
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pClusterSqlModule = pPluginManager->FindModule<NFIClusterModule>("NFCMysqlClusterModule");


    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pClusterSqlModule);

    //////////////////////////////////////////////////////////////////////////
    // register event calback
    m_pEventProcessModule->AddEventCallBack(NFGUID(), NFED_ON_CLIENT_LOGIN, this, &NFCLoginLogicModule::OnLoginEvent);

    return true;
}
