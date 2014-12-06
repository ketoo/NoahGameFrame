// -------------------------------------------------------------------------
//    @FileName         ：    NFIScriptModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-07-08
//    @Module           ：    NFIScriptModule
//    @Desc             :     every script plugin need to export two interfaces(NFCScriptVarList and most of NFCScriptKernel)
// -------------------------------------------------------------------------

#ifndef _NFI_SCRIPT_MODULE_H_
#define _NFI_SCRIPT_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm\NFCore\NFCDataList.h"
#include "NFComm\NFCore\NFIdentID.h"
#include "NFComm\NFPluginModule\NFIElementInfoModule.h"
#include "NFComm\NFPluginModule\NFIKernelModule.h"
#include "NFComm\NFPluginModule\NFIScriptKernelModule.h"
#include "NFComm\NFPluginModule\NFILogicClassModule.h"


class NFIScriptModule
    : public NFILogicModule
{

public:
    virtual int DoHeartBeatCommonCB(NFIScriptKernelModule* pScriptKernelModule, const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var)
    {
        NFCSriptData* pScriptData = pScriptKernelModule->GetElement(self);
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

    virtual int DoEventCommonCB(NFIScriptKernelModule* pScriptKernelModule, const NFIDENTID& self, const int nEventID, const NFIDataList& var)
    {
        NFCSriptData* pScriptData = pScriptKernelModule->GetElement(self);
        if (pScriptData)
        {
            NFList<NFCScriptName>* pList = pScriptData->mxEventData.GetElement(nEventID);
            if (pList)
            {
                NFCScriptName xScriptName;
                bool bRet = pList->First(xScriptName);
                while (bRet)
                {
                    //(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg)
                    DoEventScript(self, nEventID, xScriptName.strComponentName, xScriptName.strFunctionName, NFCScriptVarList(var));

                    bRet = pList->Next(xScriptName);
                }
            }
        }

        return 0;
    }

    virtual int DoPropertyCommEvent(NFIScriptKernelModule* pScriptKernelModule, const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg)
    {
        NFCSriptData* pScriptData = pScriptKernelModule->GetElement(self);
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

    virtual int DoRecordCommonEvent(NFIScriptKernelModule* pScriptKernelModule, const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg)
    {
        NFCSriptData* pScriptData = pScriptKernelModule->GetElement(self);
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

    virtual int DoClassCommonEvent(NFILogicClassModule* pLogicClassModule, const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
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
            std::shared_ptr<NFIComponentManager> pComponentManager = pLogicClassModule->GetClassComponentManager(strClassName);
            if (pComponentManager.get())
            {
                std::shared_ptr<NFIComponent> pComponent = pComponentManager->First();
                while (pComponent.get() && pComponent->Enable())
                {
                    DoScript(self, pComponent->ComponentName(), strSerializationName, NFCScriptVarList(var));

                    pComponent = pComponentManager->Next();
                }
            }
        }

        return 0;
    }

    //call script
    virtual int DoScript(const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;
    virtual int DoEventScript(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;
    virtual int DoHeartBeatScript(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;


    virtual int DoScriptPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg) = 0;
    virtual int DoScriptRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg) = 0;
};

class NFCScriptKernelModule : public NFIScriptKernelModule
{
public:
    NFCScriptKernelModule(NFIKernelModule* pKernelModule, NFIScriptModule* pScriptModule, NFIElementInfoModule* pElementInfoModule, NFIEventProcessModule* pEventProcessModule)
    {
        m_pKernelModule = pKernelModule;
        m_pScriptModule = pScriptModule;
        m_pElementInfoModule = pElementInfoModule;
        m_pEventProcessModule = pEventProcessModule;
    }

    //becall from script
    ///////need to export function///////////////////////////////////
    bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID)
    {
        NFCSriptData* pScriptData = GetElement(objectID);
        if (pScriptData)
        {
            if(pScriptData->mxEventData.GetElement(nEventID))
            {
                return true;
            }
        }

        return false;
    }

    bool RemoveEvent(const NFIDENTID& objectID)
    {
        NFCSriptData* pScriptData = RemoveElement(objectID);
        if (pScriptData)
        {
            NFList<NFCScriptName>* pScriptNameList = pScriptData->mxEventData.First();
            while (pScriptNameList)
            {
                delete pScriptNameList;
                pScriptNameList = NULL;

                pScriptNameList = pScriptData->mxEventData.Next();
            }

            pScriptData->mxEventData.ClearAll();

            delete pScriptData;
            pScriptData = NULL;

            return true;
        }

        return false;
    }

    bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID)
    {
        NFCSriptData* pScriptData = GetElement(objectID);
        if (pScriptData)
        {
            int nEventID = 0;
            NFList<NFCScriptName>* pScriptNameList = pScriptData->mxEventData.RemoveElement(nEventID);
            if (pScriptNameList)
            {
                delete pScriptNameList;
                pScriptNameList = NULL;

                return true;
            }
        }

        return false;
    }

    bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFCScriptVarList& valueList)
    {
        //do c++ event
        return m_pEventProcessModule->DoEvent(objectID, nEventID, valueList.GetVar());
    }

    bool ExistElement(const std::string& strConfigName)
    {
        return m_pElementInfoModule->ExistElement(strConfigName);
    }

    int GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName)
    {
        return m_pElementInfoModule->GetPropertyInt(strConfigName, strPropertyName);
    }

    float GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName)
    {
        return m_pElementInfoModule->GetPropertyFloat(strConfigName, strPropertyName);
    }

    const std::string& GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName)
    {
        return m_pElementInfoModule->GetPropertyString(strConfigName, strPropertyName);
    }
    //////////////////////////////////////////////////////////////////


    bool AddPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction)
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
        return pScriptNameList->Add(xScriptName);

        return false;
    }

    bool AddRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction)
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
        return pScriptNameList->Add(xScriptName);

        return false;
    }

    bool AddEventCallBack(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction)
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

        m_pKernelModule->AddEventCallBack(self, nEventID, this, &NFCScriptKernelModule::OnEventCommonCB);

        return true;
    }

    int OnEventCommonCB(const NFIDENTID& self, const int nEventID, const NFIDataList& var)
    {
        m_pScriptModule->DoEventCommonCB(this, self, nEventID, var);
        return 0;
    }

    bool AddHeartBeat(const NFIDENTID self, const std::string& strHeartBeatName, const std::string& strComponentName, const std::string& strFunction, const float fTime, const int nCount)
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

        m_pKernelModule->AddHeartBeat(self, strHeartBeatName, this, &NFCScriptKernelModule::OnHeartBeatCommonCB, NFCDataList(), fTime, nCount);

        return true;
    }
    
    int OnHeartBeatCommonCB(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var)
    {
        m_pScriptModule->DoHeartBeatCommonCB(this, self, strHeartBeat, fTime, nCount, var);
        return 0;
    }

    bool FindHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (!pScriptData)
        {
            return false;
        }

        NFList<NFCScriptName>* pScriptNameList = pScriptData->mxHeartBeatData.GetElement(strHeartBeatName);
        if (!pScriptNameList)
        {
            return false;
        }

        return true;
    }

    bool RemoveHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName)
    {
        NFCSriptData* pScriptData = GetElement(self);
        if (!pScriptData)
        {
            return false;
        }

        NFList<NFCScriptName>* pScriptNameList = pScriptData->mxHeartBeatData.RemoveElement(strHeartBeatName);
        if (!pScriptNameList)
        {
            delete pScriptNameList;
            pScriptNameList = NULL;

            return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////
    bool SetComponentEnable(const NFIDENTID& self, const std::string& strComponentName, const bool bEnable)
    {
        return m_pKernelModule->SetComponentEnable(self, strComponentName, bEnable);
    }

    bool QueryComponentEnable(const NFIDENTID& self, const std::string& strComponentName)
    {
        return m_pKernelModule->QueryComponentEnable(self, strComponentName);
    }

    NFIDENTID CreateContainer(const int nContainerIndex, const std::string& strSceneConfigID)
    {
        return m_pKernelModule->CreateContainer(nContainerIndex, strSceneConfigID);
    }

    bool ExistContainer(const int nContainerIndex)
    {
        return m_pKernelModule->ExistContainer(nContainerIndex);
    }

    bool CreateObject(const NFIDENTID& self, const int nContainerID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFCScriptVarList& arg)
    {
        std::shared_ptr<NFIObject> pObject = m_pKernelModule->CreateObject(self, nContainerID, nGroupID, strClassName, strConfigIndex, arg.GetVar());
        if (pObject.get())
        {
            return true;
        }

        return false;
    }

    bool DestroyObject(const NFIDENTID& self)
    {
        return m_pKernelModule->DestroyObject(self);
    }

    bool FindProperty(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->FindProperty(self, strPropertyName);
    }

    bool SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const int nValue)
    {
        return m_pKernelModule->SetPropertyInt(self, strPropertyName, nValue);
    }

    bool SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName,  const float fValue)
    {
        return m_pKernelModule->SetPropertyFloat(self, strPropertyName, fValue);
    }

    bool SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue)
    {
        return m_pKernelModule->SetPropertyString(self, strPropertyName, strValue);
    }

    bool SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& objectValue)
    {
        return m_pKernelModule->SetPropertyObject(self, strPropertyName, objectValue);
    }

    int GetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->GetPropertyInt(self, strPropertyName);
    }

    float GetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->GetPropertyFloat(self, strPropertyName);
    }

    const std::string& GetPropertyString(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->GetPropertyString(self, strPropertyName);
    }

    NFIDENTID GetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->GetPropertyObject(self, strPropertyName);
    }

    bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const int nValue)
    {
        return m_pKernelModule->SetRecordInt(self, strRecordName, nRow, nCol, nValue);
    }

    bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue)
    {
        return m_pKernelModule->SetRecordFloat(self, strRecordName, nRow, nCol, fValue);
    }

    bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
    {
        return m_pKernelModule->SetRecordString(self, strRecordName, nRow, nCol, strValue);
    }

    bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& objectValue)
    {
        return m_pKernelModule->SetRecordObject(self, strRecordName, nRow, nCol, objectValue);
    }

    int GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {
        return m_pKernelModule->GetRecordInt(self, strRecordName, nRow, nCol);
    }

    float GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {

        return m_pKernelModule->GetRecordFloat(self, strRecordName, nRow, nCol);
    }

    const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {
        return m_pKernelModule->GetRecordString(self, strRecordName, nRow, nCol);
    }

    NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {
        return m_pKernelModule->GetRecordObject(self, strRecordName, nRow, nCol);
    }

    bool AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic ,  bool bPrivate ,  bool bSave, bool bView, int nIndex, const std::string& strScriptFunction)
    {
        return m_pKernelModule->AddProperty(self, strPropertyName, varType, bPublic, bPrivate, bSave, bView, nIndex, strScriptFunction);
    }

    bool AddRow(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& var)
    {
        std::shared_ptr<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
        if ( pRecord.get() )
        {
            if (pRecord->AddRow(-1, var) >= 0)
            {
                return true;
            }
        }

        return false;
    }

    bool AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFCScriptVarList& TData, const NFCScriptVarList& varKey, const NFCScriptVarList& varDesc, const NFCScriptVarList& varTag, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex)
    {
        return m_pKernelModule->AddRecord(self, strRecordName, TData.GetVar(), varKey.GetVar(), varDesc.GetVar(), varTag.GetVar(), varTag.GetVar(), nRows, bPublic, bPrivate, bSave, bView, nIndex);
    }

protected:

private:
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIScriptModule*m_pScriptModule;
    NFIEventProcessModule* m_pEventProcessModule;
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static  NFIDataList* NFVarList_New()
{
    NFIDataList* pVarList = new NFCDataList();
    return pVarList;
}

static void NFVarList_Del(NFIDataList* pVarList)
{
    if (pVarList)
    {
        delete pVarList;
        pVarList = NULL;
    }
}

static void NFVarList_Clear(NFIDataList* pVarList)
{
    if (pVarList)
    {
        pVarList->Clear();
    }
}

static bool NFVarList_IsEmpty(NFIDataList* pVarList)
{
    if (pVarList)
    {
        pVarList->IsEmpty();
    }

    return false;
}

static int NFVarList_GetCount(NFIDataList* pVarList)
{
    if (pVarList)
    {
        return pVarList->GetCount();
    }

    return 0;
}

static TDATA_TYPE NFVarList_Type(NFIDataList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->Type(index);
    }

    return TDATA_UNKNOWN;
}

static bool NFVarList_AddInt(NFIDataList* pVarList, const NFINT64 value)
{
    if (pVarList)
    {
        return pVarList->Add(value);
    }

    return false;
}

static bool NFVarList_AddFloat(NFIDataList* pVarList, const float value)
{
    if (pVarList)
    {
        return pVarList->Add(value);
    }

    return false;
}

static bool NFVarList_AddString(NFIDataList* pVarList, const char* value)
{
    if (pVarList)
    {
        return pVarList->Add(value);
    }

    return false;
}

static bool NFVarList_AddObject(NFIDataList* pVarList, const NFIDENTID& value)
{
    if (pVarList)
    {
        return pVarList->Add(value);
    }

    return false;
}

static bool NFVarList_SetInt(NFIDataList* pVarList, const int index, const NFINT64 value)
{
    if (pVarList)
    {
        return pVarList->Set(index, value);
    }

    return false;
}

static bool NFVarList_SetFloat(NFIDataList* pVarList, const int index, const float value)
{
    if (pVarList)
    {
        return pVarList->Set(index, value);
    }

    return false;
}

static bool NFVarList_SetString(NFIDataList* pVarList, const int index, const char* value)
{
    if (pVarList)
    {
        return pVarList->Set(index, value);
    }

    return false;
}

static bool NFVarList_SetObject(NFIDataList* pVarList, const int index, const NFIDENTID& value)
{
    if (pVarList)
    {
        return pVarList->Set(index, value);
    }

    return false;
}

static int NFVarList_IntVal(NFIDataList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->Int(index);
    }

    return 0;
}

static float NFVarList_FloatVal(NFIDataList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->Float(index);
    }

    return 0.0f;
}

static const std::string& NFVarList_StringVal(NFIDataList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->String(index);
    }

    return NULL_STR;
}

static NFIDENTID NFVarList_ObjectVal(NFIDataList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->Object(index);
    }

    return NFIDENTID();
}

//////////////////////////////////////////////////////////////////////////

static bool KernelModule_HasEventCallBack(const NFScriptInt64& nPtrKernelModule,
                        const NFScriptInt64& objectID, const int nEventID)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->HasEventCallBack(objectID.ObjectVal(), nEventID);
    }

    return false;
}

static bool KernelModule_RemoveEvent(const NFScriptInt64& nPtrKernelModule,
                        const NFScriptInt64& objectID)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->RemoveEvent(objectID.ObjectVal());
    }

    return false;
}

static bool KernelModule_RemoveEventCallBack(const NFScriptInt64& nPtrKernelModule,
                        const NFScriptInt64& objectID, const int nEventID)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->RemoveEventCallBack(objectID.ObjectVal(), nEventID);
    }

    return false;
}

static bool KernelModule_DoEvent(const NFScriptInt64& nPtrKernelModule,
                                const NFScriptInt64& objectID, const int nEventID, const NFCScriptVarList& valueList)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->DoEvent(objectID.ObjectVal(), nEventID, valueList);
    }

    return false;
}

static bool KernelModule_ExistElement(const NFScriptInt64& nPtrKernelModule,
                                const std::string& strConfigName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->ExistElement(strConfigName);
    }

    return false;
}

static int KernelModule_GetPropertyInt(const NFScriptInt64& nPtrKernelModule,
                         const std::string& strConfigName, const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetPropertyInt(strConfigName, strPropertyName);
    }

    return 0;
}

static float KernelModule_GetPropertyFloat(const NFScriptInt64& nPtrKernelModule,
                             const std::string& strConfigName, const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetPropertyFloat(strConfigName, strPropertyName);
    }

    return 0.0f;
}

static const std::string& KernelModule_GetPropertyString(const NFScriptInt64& nPtrKernelModule,
                                const std::string& strConfigName, const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetPropertyString(strConfigName, strPropertyName);
    }

    return NULL_STR;
}

static bool KernelModule_AddPropertyCallBack(const NFScriptInt64& nPtrKernelModule,
    const NFScriptInt64& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddPropertyCallBack(self.ObjectVal(), strPropertyName, strComponentName, strFunction);
    }

    return false;
}

static bool KernelModule_AddRecordCallBack(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                              const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddRecordCallBack(self.ObjectVal(), strRecordName, strComponentName, strFunction);
    }

    return false;
}

static bool KernelModule_AddEventCallBack(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                             const int nEventID, const std::string& strComponentName, const std::string& strFunction)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddEventCallBack(self.ObjectVal(), nEventID, strComponentName, strFunction);
    }

    return false;
}

static bool KernelModule_AddHeartBeat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64 self, 
    const std::string& strHeartBeatName, const std::string& strComponentName, const std::string& strFunction, const float fTime, const int nCount)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddHeartBeat(self.ObjectVal(), strHeartBeatName, strComponentName, strFunction, fTime, nCount);
    }

    return false;
}

static bool KernelModule_FindHeartBeat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, const std::string& strHeartBeatName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->FindHeartBeat(self.ObjectVal(), strHeartBeatName);
    }

    return false;
}

static bool KernelModule_RemoveHeartBeat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, const std::string& strHeartBeatName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->RemoveHeartBeat(self.ObjectVal(), strHeartBeatName);
    }

    return false;
}

static bool KernelModule_SetComponentEnable(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                               const std::string& strComponentName, const bool bEnable)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetComponentEnable(self.ObjectVal(), strComponentName, bEnable);
    }

    return false;
}

static bool KernelModule_QueryComponentEnable(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                 const std::string& strComponentName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryComponentEnable(self.ObjectVal(), strComponentName);
    }

    return false;
}

static NFScriptInt64 KernelModule_CreateContainer(const NFScriptInt64& nPtrKernelModule, const int nContainerIndex, const std::string& strSceneConfigID)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return  pScriptKernelModule->CreateContainer(nContainerIndex, strSceneConfigID);
    }

    return NFIDENTID();
}

static bool KernelModule_ExistContainer(const NFScriptInt64& nPtrKernelModule, const int nContainerIndex)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->ExistContainer(nContainerIndex);
    }

    return false;
}

static bool KernelModule_CreateObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                         const int nContainerID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFCScriptVarList& arg)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->CreateObject(self.ObjectVal(), nContainerID, nGroupID, strClassName, strConfigIndex, arg);
    }

    return false;
}

static  bool KernelModule_DestroyObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->DestroyObject(self.ObjectVal());
    }

    return false;
}
static bool KernelModule_FindProperty(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->FindProperty(self.ObjectVal(), strPropertyName);
    }

    return false;
}

static bool KernelModule_SetPropertyInt(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, const std::string& strPropertyName, const int nValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetPropertyInt(self.ObjectVal(), strPropertyName, nValue);
    }

    return false;
}

static bool KernelModule_SetPropertyFloat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                             const std::string& strPropertyName,  const float fValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetPropertyFloat(self.ObjectVal(), strPropertyName, fValue);
    }

    return false;
}

static bool KernelModule_SetPropertyString(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                              const std::string& strPropertyName, const std::string& strValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetPropertyString(self.ObjectVal(), strPropertyName, strValue);
    }

    return false;
}

static bool KernelModule_SetPropertyObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                              const std::string& strPropertyName, const NFScriptInt64& objectValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetPropertyObject(self.ObjectVal(), strPropertyName, objectValue.ObjectVal());
    }

    return false;
}

static int KernelModule_GetPropertyInt(const NFScriptInt64& nKernelModule, const NFScriptInt64& self, 
                            const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetPropertyInt(self.ObjectVal(), strPropertyName);
    }

    return 0;
}

static float KernelModule_GetPropertyFloat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetPropertyFloat(self.ObjectVal(), strPropertyName);
    }

    return 0.0f;
}

static const std::string& KernelModule_GetPropertyString(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self,
                                              const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetPropertyString(self.ObjectVal(), strPropertyName);
    }

    return NULL_STR;
}

static NFScriptInt64 KernelModule_GetPropertyObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                     const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetPropertyObject(self.ObjectVal(), strPropertyName);
    }

    return NFIDENTID();
}

static bool KernelModule_SetRecordInt(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                         const std::string& strRecordName, const int nRow, const int nCol, const int nValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetRecordInt(self.ObjectVal(), strRecordName, nRow, nCol, nValue);
    }

    return false;
}

static bool KernelModule_SetRecordFloat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                           const std::string& strRecordName, const int nRow, const int nCol,  const float fValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetRecordFloat(self.ObjectVal(), strRecordName, nRow, nCol, fValue);
    }

    return false;
}

static bool KernelModule_SetRecordString(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                            const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetRecordString(self.ObjectVal(), strRecordName, nRow, nCol, strValue);
    }

    return false;
}

static bool KernelModule_SetRecordObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                            const std::string& strRecordName, const int nRow, const int nCol, const NFScriptInt64& objectValue)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->SetRecordObject(self.ObjectVal(), strRecordName, nRow, nCol, objectValue.ObjectVal());
    }

    return false;
}

static int KernelModule_GetRecordInt(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                          const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetRecordInt(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return 0;
}

static float KernelModule_GetRecordFloat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                              const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetRecordFloat(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return 0.0f;
}

static const std::string& KernelModule_GetRecordString(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                            const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetRecordString(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return NULL_STR;
}

static NFIDENTID KernelModule_GetRecordObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                   const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->GetRecordObject(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return NFIDENTID();
}

static bool KernelModule_AddProperty(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self,
                        const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic ,  bool bPrivate ,  
                        bool bSave, bool bView, int nIndex, const std::string& strScriptFunction)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddProperty(self.ObjectVal(), strPropertyName, varType, bPublic, bPrivate, bSave, bView, nIndex, strScriptFunction);
    }

    return false;
}

static bool KernelModule_AddRow(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self,
                                     const std::string& strRecordName, const NFCScriptVarList& var)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddRow(self.ObjectVal(), strRecordName, var.GetVar());
    }

    return false;
}
static bool KernelModule_AddRecord(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, const std::string& strRecordName, 
                      const NFCScriptVarList& TData, const NFCScriptVarList& varKey, const NFCScriptVarList& varDesc, const NFCScriptVarList& varTag, 
                      const int nRows, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddRecord(self.ObjectVal(), strRecordName, TData, varKey, varDesc, varTag, nRows, bPublic, bPrivate, bSave, bView, nIndex);
    }

    return false;
}



#endif