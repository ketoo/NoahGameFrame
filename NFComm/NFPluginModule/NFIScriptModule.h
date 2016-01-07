// -------------------------------------------------------------------------
//    @FileName         ：    NFIScriptModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-07-08
//    @Module           ：    NFIScriptModule
//    @Desc             :     every script plugin need to export two interfaces(NFCScriptVarList and most of NFCScriptKernel)
// -------------------------------------------------------------------------

#ifndef _NFI_SCRIPT_MODULE_H
#define _NFI_SCRIPT_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCScriptName
{
public:
    bool operator < (const NFCScriptName& id) const
    {
        return this->strComponentName.length() < id.strComponentName.length() && this->strFunctionName.length() < id.strFunctionName.length();
    }

    bool operator == (const NFCScriptName& id) const
    {
        if (this->strComponentName != id.strComponentName
            || this->strFunctionName != id.strFunctionName)
        {
            return false;
        }

        return true;
    }

    NFCScriptName(){}

    NFCScriptName(const std::string& strComponentName, const std::string& strFunctionName)
    {
        this->strComponentName = strComponentName;
        this->strFunctionName = strFunctionName;
    }

    std::string strComponentName;
    std::string strFunctionName;
};


class NFCSriptData
{
public:
    //eventlist: id->ScriptName,FunctionName
    NFMap<int, NFList<NFCScriptName>> mxEventData;

    //heartbeatlist: name->ScriptName,FunctionName
    NFMap<std::string, NFList<NFCScriptName>> mxHeartBeatData;

    //propertycblist: name->ScriptName,FunctionName
    NFMap<std::string, NFList<NFCScriptName>> mxPropertyCallBackData;

    //recordcblist: name->ScriptName,FunctionName
    NFMap<std::string, NFList<NFCScriptName>> mxRecordCallBackData;

protected:
private:
};

class NFIScriptModule
    : public NFILogicModule, public NFMap<NFGUID, NFCSriptData>
{

public:
    virtual int DoHeartBeatCommonCB(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (pScriptData)
        {
            NFList<NFCScriptName>* pList = pScriptData->mxHeartBeatData.GetElement(strHeartBeat);
            if (pList)
            {
                NFCScriptName xScriptName;
                bool bRet = pList->First(xScriptName);
                while (bRet)
                {
                    DoHeartBeatScript(self, strHeartBeat, fTime, nCount, xScriptName.strComponentName, xScriptName.strFunctionName);

                    bRet = pList->Next(xScriptName);
                }
            }
        }
        return 0;
    }

    virtual int DoEventCommonCB(const NFGUID& self, const int nEventID, const NFIDataList& var)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (pScriptData)
        {
            NFList<NFCScriptName>* pList = pScriptData->mxEventData.GetElement(nEventID);
            if (pList)
            {
                NFCScriptName xScriptName;
                bool bRet = pList->First(xScriptName);
                while (bRet)
                {
                    DoEventScript(self, nEventID, xScriptName.strComponentName, xScriptName.strFunctionName, var);

                    bRet = pList->Next(xScriptName);
                }
            }
        }

        return 0;
    }

    virtual int DoPropertyCommEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (pScriptData)
        {
            NFList<NFCScriptName>* pList = pScriptData->mxPropertyCallBackData.GetElement(strPropertyName);
            if (pList)
            {
                NFCScriptName xScriptName;
                bool bRet = pList->First(xScriptName);
                while (bRet)
                {
                    DoScriptPropertyCallBack(self, strPropertyName, xScriptName.strComponentName, xScriptName.strFunctionName, oldVar, newVar);

                    bRet = pList->Next(xScriptName);
                }
            }
        }

        return 0;
    }

    virtual int DoRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (pScriptData)
        {
            NFList<NFCScriptName>* pList = pScriptData->mxRecordCallBackData.GetElement(xEventData.strRecordName);
            if (pList)
            {
                NFCScriptName xScriptName;
                bool bRet = pList->First(xScriptName);
                while (bRet)
                {
                    DoScriptRecordCallBack(self, xEventData.strRecordName, xScriptName.strComponentName, xScriptName.strFunctionName, xEventData.nOpType, xEventData.nRow, xEventData.nCol, oldVar, newVar);

                    bRet = pList->Next(xScriptName);
                }
            }
        }

        return 0;
    }

    virtual int DoClassCommonEvent(NFILogicClassModule* pLogicClassModule, const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
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
            NF_SHARE_PTR<NFIComponentManager> pComponentManager = pLogicClassModule->GetClassComponentManager(strClassName);
            if (pComponentManager.get())
            {
                NF_SHARE_PTR<NFIComponent> pComponent = pComponentManager->First();
                while (pComponent.get() && pComponent->Enable())
                {
                    DoClassCommonScript(self, pComponent->GetComponentName(), strSerializationName);

                    pComponent = pComponentManager->Next();
                }
            }
        }

        return 0;
    }

    //call script
	virtual int DoScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg) = 0;
	virtual int DoClassCommonScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction) = 0;
    virtual int DoEventScript(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg) = 0;
    virtual int DoHeartBeatScript(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction) = 0;

    virtual int DoScriptPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCDataList::TData& oldVar, const NFCDataList::TData& newVar) = 0;
    virtual int DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCDataList& oldVar, const NFCDataList& newVar) = 0;

    // operating function
    virtual bool AddPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (!pScriptData)
        {
            pScriptData = new NFCSriptData();
            AddElement(self, pScriptData);
        }

        NFList<NFCScriptName>* pScriptNameList = pScriptData->mxPropertyCallBackData.GetElement(strPropertyName);
        if (!pScriptNameList)
        {
            pScriptNameList = new NFList<NFCScriptName>();
            pScriptData->mxPropertyCallBackData.AddElement(strPropertyName, pScriptNameList);
        }

        NFCScriptName xScriptName(strComponentName, strFunction);
        pScriptNameList->Add(xScriptName);

        NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
        pKernelModule->AddPropertyCallBack(self, strPropertyName, this, &NFIScriptModule::OnPropertyCB);

        return true;
    }

    int OnPropertyCB(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
    {
        return DoPropertyCommEvent(self, strPropertyName, oldVar, newVar);
    }

    virtual bool AddRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (!pScriptData)
        {
            pScriptData = new NFCSriptData();
            AddElement(self, pScriptData);
        }

        NFList<NFCScriptName>* pScriptNameList = pScriptData->mxRecordCallBackData.GetElement(strRecordName);
        if (!pScriptNameList)
        {
            pScriptNameList = new NFList<NFCScriptName>();
            pScriptData->mxRecordCallBackData.AddElement(strRecordName, pScriptNameList);
        }

        NFCScriptName xScriptName(strComponentName, strFunction);
        pScriptNameList->Add(xScriptName);

        NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
        pKernelModule->AddRecordCallBack(self, strRecordName, this, &NFIScriptModule::OnRecordCB);

        return true;
    }

    int OnRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar)
    {
        return DoRecordCommonEvent(self, xEventData, oldVar, newVar);
    }

    virtual bool AddHeartBeatCallBack(const NFGUID& self, const std::string& strHeartBeatName, const std::string& strComponentName, const std::string& strFunction, const float fTime, const int nCount)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (!pScriptData)
        {
            pScriptData = new NFCSriptData();
            AddElement(self, pScriptData);
        }

        NFList<NFCScriptName>* pScriptNameList = pScriptData->mxHeartBeatData.GetElement(strHeartBeatName);
        if (!pScriptNameList)
        {
            pScriptNameList = new NFList<NFCScriptName>();
            pScriptData->mxHeartBeatData.AddElement(strHeartBeatName, pScriptNameList);
        }

        //应该是同时直接向系统注册.
        NFCScriptName xScriptName(strComponentName, strFunction);
        pScriptNameList->Add(xScriptName);

        NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
        pKernelModule->AddHeartBeat(self, strHeartBeatName, this, &NFIScriptModule::OnHeartBeatCB, fTime, nCount);

        return true;
    }

    int OnHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float fTime, const int nCount)
    {
        return DoHeartBeatCommonCB(self, strHeartBeatName, fTime, nCount);
    }

    virtual bool AddEventCallBack(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (!pScriptData)
        {
            pScriptData = new NFCSriptData();
            AddElement(self, pScriptData);
        }

        NFList<NFCScriptName>* pScriptNameList = pScriptData->mxEventData.GetElement(nEventID);
        if (!pScriptNameList)
        {
            pScriptNameList = new NFList<NFCScriptName>();
            pScriptData->mxEventData.AddElement(nEventID, pScriptNameList);
        }

        NFCScriptName xScriptName(strComponentName, strFunction);
        pScriptNameList->Add(xScriptName);

        NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
        pKernelModule->AddEventCallBack(self, nEventID, this, &NFIScriptModule::OnEventCB);

        return true;
    }

    int OnEventCB(const NFGUID& self, const int nEventID, const NFIDataList& argVar)
    {
        return DoEventCommonCB(self, nEventID, argVar);
    }
};

static bool KernelModule_DoEvent(NFINT64 kernelAddress, const NFGUID& self, const int nEventID, const NFCDataList& valueList)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NFIEventProcessModule* pEventProcessModule = pKernelModule->GetPluginManager()->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    return pEventProcessModule->DoEvent(self, nEventID, valueList);
}

static bool KernelModule_FindHeartBeat(NFIKernelModule* pKernelModule, const NFGUID& self, const std::string& strHeartBeatName)
{
    return pKernelModule->FindHeartBeat(self, strHeartBeatName);
}

static bool KernelModule_RemoveHeartBeat(NFIKernelModule* pKernelModule, const NFGUID& self, const std::string& strHeartBeatName)
{
    return pKernelModule->RemoveHeartBeat(self, strHeartBeatName);
}

///////////////////////////////////////////////////
static bool KernelModule_ExistElement(NFIKernelModule* pKernelModule, const std::string& strConfigName)
{
    NFIElementInfoModule* pElementInfoModule = pKernelModule->GetPluginManager()->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    return pElementInfoModule->ExistElement(strConfigName);
}

static NFINT64 KernelModule_GetElementPropertyInt(NFINT64 kernelAddress, const std::string& strConfigName, const std::string& strPropertyName)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NFIElementInfoModule* pElementInfoModule = pKernelModule->GetPluginManager()->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    return pElementInfoModule->GetPropertyInt(strConfigName, strPropertyName);
}

static double KernelModule_GetElementPropertyFloat(NFINT64 kernelAddress, const std::string& strConfigName, const std::string& strPropertyName)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NFIElementInfoModule* pElementInfoModule = pKernelModule->GetPluginManager()->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    return pElementInfoModule->GetPropertyFloat(strConfigName, strPropertyName);
}

static const std::string& KernelModule_GetElementPropertyString(NFIKernelModule* pKernelModule, const std::string& strConfigName, const std::string& strPropertyName)
{
    NFIElementInfoModule* pElementInfoModule = pKernelModule->GetPluginManager()->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    return pElementInfoModule->GetPropertyString(strConfigName, strPropertyName);
}

static bool KernelModule_ExistContainer(NFIKernelModule* pKernelModule, const int nContainerIndex)
{
    return pKernelModule->ExistContainer(nContainerIndex);
}

static bool KernelModule_SetPropertyInt(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName, const int nValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetPropertyInt(self, strPropertyName, nValue);
}

static bool KernelModule_SetPropertyFloat(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName,  const double dValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetPropertyFloat(self, strPropertyName, dValue);
}

static bool KernelModule_SetPropertyString(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName, const std::string& strValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetPropertyString(self, strPropertyName, strValue);
}

static bool KernelModule_SetPropertyObject(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName, const NFGUID& objectValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetPropertyObject(self, strPropertyName, objectValue);
}

static NFINT64 KernelModule_GetPropertyInt(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetPropertyInt(self, strPropertyName);
}

static double KernelModule_GetPropertyFloat(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetPropertyFloat(self, strPropertyName);
}

static const std::string& KernelModule_GetPropertyString(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetPropertyString(self, strPropertyName);
}

static NFGUID KernelModule_GetPropertyObject(NFINT64 kernelAddress, const NFGUID& self, const std::string& strPropertyName)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetPropertyObject(self, strPropertyName);
}

static bool KernelModule_SetRecordInt(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const int nValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetRecordInt(self, strRecordName, nRow, nCol, nValue);
}

static bool KernelModule_SetRecordFloat(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol,  const double dValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetRecordFloat(self, strRecordName, nRow, nCol, dValue);
}

static bool KernelModule_SetRecordString(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetRecordString(self, strRecordName, nRow, nCol, strValue);
}

static bool KernelModule_SetRecordObject(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->SetRecordObject(self, strRecordName, nRow, nCol, objectValue);
}

static NFINT64 KernelModule_GetRecordInt(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetRecordInt(self, strRecordName, nRow, nCol);
}

static double KernelModule_GetRecordFloat(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetRecordFloat(self, strRecordName, nRow, nCol);
}

static const std::string& KernelModule_GetRecordString(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetRecordString(self, strRecordName, nRow, nCol);
}

static NFGUID KernelModule_GetRecordObject(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    return pKernelModule->GetRecordObject(self, strRecordName, nRow, nCol);
}

static int KernelModule_AddRow(NFINT64 kernelAddress, const NFGUID& self, const std::string& strRecordName, const NFCDataList& var)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NF_SHARE_PTR<NFIRecord> pRecord = pKernelModule->FindRecord(self, strRecordName);
    if (nullptr == pRecord)
    {
        return -1;
    }

    return pRecord->AddRow(-1, var);
}

#endif
