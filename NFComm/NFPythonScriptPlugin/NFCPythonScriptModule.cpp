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
    m_pKernelModule->ResgisterCommonHeartBeatEvent(this, &NFCPythonScriptModule::OnHeartBeatCommonCB);
    m_pKernelModule->ResgisterCommonEvent(this, &NFCPythonScriptModule::OnEventCommonCB);

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

int NFCPythonScriptModule::OnRecordCommonEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& arg )
{
    NFCSriptData* pScriptData = m_pScriptKernelModule->GetElement(self);
    if (pScriptData)
    {
        NFList<NFCScriptName>* pList = pScriptData->mxRecordCallBackData.GetElement(strRecordName);
        if (pList)
        {
            NFCScriptName xScriptName;
            bool bRet = pList->First(xScriptName);
            while (bRet)
            {
                DoScriptRecordCallBack(self, strRecordName, xScriptName.strComponentName, xScriptName.strFunctionName, nOpType, nRow, nCol, NFCScriptVarList(oldVar), NFCScriptVarList(newVar), NFCScriptVarList(arg));

                bRet = pList->Next(xScriptName);
            }
        }
    }

    return 0;
}

int NFCPythonScriptModule::OnPropertyCommEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& arg )
{
    NFCSriptData* pScriptData = m_pScriptKernelModule->GetElement(self);
    if (pScriptData)
    {
        NFList<NFCScriptName>* pList = pScriptData->mxPropertyCallBackData.GetElement(strPropertyName);
        if (pList)
        {
            NFCScriptName xScriptName;
            bool bRet = pList->First(xScriptName);
            while (bRet)
            {
                DoScriptPropertyCallBack(self, strPropertyName, xScriptName.strComponentName, xScriptName.strFunctionName, NFCScriptVarList(oldVar), NFCScriptVarList(newVar), NFCScriptVarList(arg));

                bRet = pList->Next(xScriptName);
            }
        }
    }

    return 0;
}

int NFCPythonScriptModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    std::string strSerializationName;

    switch (eClassEvent)
    {
    case COE_CREATE_NODATA:
        strSerializationName = "Init";
        break;

    case COE_CREATE_HASDATA:
        strSerializationName = "AfterInit";
        break;

    case COE_BEFOREDESTROY:
        strSerializationName = "BeforeShut";
        break;

    case COE_DESTROY:
        strSerializationName = "Shut";
        break;

    default:
        break;
    }

    if (!strSerializationName.empty())
    {
        NFIComponentManager* pComponentManager = m_pLogicClassModule->GetClassComponentManager(strClassName);
        if (pComponentManager)
        {
            NFIComponent* pComponent = pComponentManager->First();
            while (pComponent && pComponent->Enable())
            {
                DoScript(self, pComponent->ComponentName(), strSerializationName, NFCScriptVarList(var));

                pComponentManager->Next();
            }
        }
    }

    return 0;
}

int NFCPythonScriptModule::OnHeartBeatCommonCB( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    NFCSriptData* pScriptData = m_pScriptKernelModule->GetElement(self);
    if (pScriptData)
    {
        NFList<NFCScriptName>* pList = pScriptData->mxHeartBeatData.GetElement(strHeartBeat);
        if (pList)
        {
            NFCScriptName xScriptName;
            bool bRet = pList->First(xScriptName);
            while (bRet)
            {
                DoHeartBeatScript(self, strHeartBeat, fTime, nCount, xScriptName.strComponentName, xScriptName.strFunctionName, NFCScriptVarList(var));

                bRet = pList->Next(xScriptName);
            }
        }
    }

    return 0;
}

int NFCPythonScriptModule::OnEventCommonCB( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
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

int NFCPythonScriptModule::DoHeartBeatCommonCB( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    return 0;
}

int NFCPythonScriptModule::DoEventCommonCB( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}
