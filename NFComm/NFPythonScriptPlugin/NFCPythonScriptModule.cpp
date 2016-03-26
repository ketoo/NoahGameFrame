// -------------------------------------------------------------------------
//    @FileName         :    NFCPythonScriptModule.cpp
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
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);

    //Py_Initialize();
    //Py_Finalize();
    bool ret = InstallPython("script_init.python");

    assert(ret);

    std::shared_ptr<NFILogicClass> pClass = m_pLogicClassModule->First();
    while (pClass.get())
    {
        std::shared_ptr<NFIComponent> pComponent = pClass->GetComponentManager()->First();
        while (pComponent.get())
        {
            if (!CheckCompomentStatus(pComponent->GetComponentName()))
            {
                assert(0);
            }

            pComponent = pClass->GetComponentManager()->Next();
        }

        pClass = m_pLogicClassModule->Next();
    }

    //add all callback
    m_pKernelModule->ResgisterCommonPropertyEvent(this, &NFCPythonScriptModule::OnPropertyCommEvent);
    m_pKernelModule->ResgisterCommonRecordEvent(this, &NFCPythonScriptModule::OnRecordCommonEvent);
    m_pKernelModule->ResgisterCommonClassEvent(this, &NFCPythonScriptModule::OnClassCommonEvent);

    return true;
}

bool NFCPythonScriptModule::BeforeShut()
{
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

int NFCPythonScriptModule::OnPropertyCommEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    //DoPropertyCommEvent(m_pScriptKernelModule, self, strPropertyName, oldVar, newVar, arg);

    return 0;
}

int NFCPythonScriptModule::OnRecordCommonEvent(const NFGUID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    //DoRecordCommonEvent(m_pScriptKernelModule, self, strRecordName, nOpType, nRow, nCol, oldVar, newVar, arg);

    return 0;
}

int NFCPythonScriptModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    DoClassCommonEvent(m_pLogicClassModule, self, strClassName, eClassEvent, var);

    return 0;
}
//////////////////////////////////////////////////////////////////////////

int NFCPythonScriptModule::DoScriptPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    return 0;
}

int NFCPythonScriptModule::DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    return 0;
}

int NFCPythonScriptModule::DoClassCommonScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction)
{
    return 0;
}

int NFCPythonScriptModule::DoScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg)
{
    return 0;
}

int NFCPythonScriptModule::DoHeartBeatScript(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction)
{
    return 0;
}

int NFCPythonScriptModule::DoEventScript(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg)
{
    return 0;
}
