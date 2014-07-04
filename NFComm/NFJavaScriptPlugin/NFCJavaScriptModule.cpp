// -------------------------------------------------------------------------
//    @FileName      :    NFCJavaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCJavaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCJavaScriptModule.h"
#include "NFJavaScriptPlugin.h"

bool NFCJavaScriptModule::Init()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);


    return true;
}

bool NFCJavaScriptModule::AfterInit()
{


    return true;
}

bool NFCJavaScriptModule::Shut()
{

    return true;
}

bool NFCJavaScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

int NFCJavaScriptModule::DoScript( const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoEventScript( const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoHeartBeatScript( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoScriptPropertyCallBack( const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& neVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoScriptRecordCallBack( const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCJavaScriptModule::DoHeartBeatCommonCB( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    return 0;
}

int NFCJavaScriptModule::DoEventCommonCB( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}
