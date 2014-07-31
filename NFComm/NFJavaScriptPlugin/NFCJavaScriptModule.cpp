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
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);

    m_pScriptKernelModule = new NFCScriptKernelModule(m_pKernelModule, this, m_pElementInfoModule, m_pEventProcessModule);

    //add all callback
    m_pKernelModule->ResgisterCommonPropertyEvent(this, &NFCJavaScriptModule::OnPropertyCommEvent);
    m_pKernelModule->ResgisterCommonRecordEvent(this, &NFCJavaScriptModule::OnRecordCommonEvent);
    m_pKernelModule->ResgisterCommonClassEvent(this, &NFCJavaScriptModule::OnClassCommonEvent);


    return true;
}

bool NFCJavaScriptModule::AfterInit()
{
    return true;
}

bool NFCJavaScriptModule::BeforeShut()
{
    delete m_pScriptKernelModule;
    m_pScriptKernelModule = NULL;

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

int NFCJavaScriptModule::OnPropertyCommEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoPropertyCommEvent(m_pScriptKernelModule, self,strPropertyName, oldVar, newVar, arg);

    return 0;
}

int NFCJavaScriptModule::OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoRecordCommonEvent(m_pScriptKernelModule, self,strRecordName, nOpType, nRow, nCol, oldVar, newVar, arg);

    return 0;
}

int NFCJavaScriptModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    DoClassCommonEvent(m_pLogicClassModule, self, strClassName, eClassEvent, var);

    return 0;
}
