// -------------------------------------------------------------------------
//    @FileName      :    NFCPythonScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCPythonScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCPythonScriptModule.h"
#include "NFPythonScriptPlugin.h"

bool NFCPythonScriptModule::Init()
{

    return true;
}

bool NFCPythonScriptModule::AfterInit()
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
    m_pKernelModule->ResgisterCommonPropertyEvent(this, &NFCPythonScriptModule::OnPropertyCommEvent);
    m_pKernelModule->ResgisterCommonRecordEvent(this, &NFCPythonScriptModule::OnRecordCommonEvent);
    m_pKernelModule->ResgisterCommonClassEvent(this, &NFCPythonScriptModule::OnClassCommonEvent);

    return true;
}

bool NFCPythonScriptModule::BeforeShut()
{
    delete m_pScriptKernelModule;
    m_pScriptKernelModule = NULL;

    return true;
}

bool NFCPythonScriptModule::Shut()
{

    return true;
}

bool NFCPythonScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

int NFCPythonScriptModule::OnPropertyCommEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoPropertyCommEvent(m_pScriptKernelModule, self,strPropertyName, oldVar, newVar, arg);

    return 0;
}

int NFCPythonScriptModule::OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg )
{
    DoRecordCommonEvent(m_pScriptKernelModule, self,strRecordName, nOpType, nRow, nCol, oldVar, newVar, arg);

    return 0;
}

int NFCPythonScriptModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    DoClassCommonEvent(m_pLogicClassModule, self, strClassName, eClassEvent, var);

    return 0;
}
//////////////////////////////////////////////////////////////////////////

int NFCPythonScriptModule::DoScriptPropertyCallBack( const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& neVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCPythonScriptModule::DoScriptRecordCallBack( const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCPythonScriptModule::DoScript( const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCPythonScriptModule::DoHeartBeatScript( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCPythonScriptModule::DoEventScript( const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}
