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
#include "NFComm\NFCore\NFCValueList.h"
#include "NFComm\NFCore\NFIdentID.h"
#include "NFComm\NFPluginModule\NFIElementInfoModule.h"
#include "NFComm\NFPluginModule\NFIKernelModule.h"

static  NFIValueList* NFVarList_New()
{
    NFIValueList* pVarList = new NFCValueList();
    return pVarList;
}

static void NFVarList_Del(NFIValueList* pVarList)
{
    if (pVarList)
    {
        delete pVarList;
        pVarList = NULL;
    }
}

static void NFVarList_Clear(NFIValueList* pVarList)
{
    if (pVarList)
    {
        pVarList->Clear();
    }
}

static bool NFVarList_IsEmpty(NFIValueList* pVarList)
{
    if (pVarList)
    {
        pVarList->IsEmpty();
    }

    return false;
}

static int NFVarList_GetCount(NFIValueList* pVarList)
{
    if (pVarList)
    {
        return pVarList->GetCount();
    }

    return 0;
}

static VARIANT_TYPE NFVarList_Type(NFIValueList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->Type(index);
    }

    return VTYPE_UNKNOWN;
}

static bool NFVarList_AddInt(NFIValueList* pVarList, const int value)
{
    if (pVarList)
    {
        return pVarList->AddInt(value);
    }

    return false;
}

static bool NFVarList_AddFloat(NFIValueList* pVarList, const float value)
{
    if (pVarList)
    {
        return pVarList->AddFloat(value);
    }

    return false;
}

static bool NFVarList_AddString(NFIValueList* pVarList, const char* value)
{
    if (pVarList)
    {
        return pVarList->AddString(value);
    }

    return false;
}

static bool NFVarList_AddObject(NFIValueList* pVarList, const NFIDENTID& value)
{
    if (pVarList)
    {
        return pVarList->AddObject(value);
    }

    return false;
}

static bool NFVarList_SetInt(NFIValueList* pVarList, const int index, const int value)
{
    if (pVarList)
    {
        return pVarList->SetInt(index, value);
    }

    return false;
}

static bool NFVarList_SetFloat(NFIValueList* pVarList, const int index, const float value)
{
    if (pVarList)
    {
        return pVarList->SetFloat(index, value);
    }

    return false;
}

static bool NFVarList_SetString(NFIValueList* pVarList, const int index, const char* value)
{
    if (pVarList)
    {
        return pVarList->SetString(index, value);
    }

    return false;
}

static bool NFVarList_SetObject(NFIValueList* pVarList, const int index, const NFIDENTID& value)
{
    if (pVarList)
    {
        return pVarList->SetObject(index, value);
    }

    return false;
}

static int NFVarList_IntVal(NFIValueList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->IntVal(index);
    }

    return 0;
}

static float NFVarList_FloatVal(NFIValueList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->FloatVal(index);
    }

    return 0.0f;
}

static const std::string& NFVarList_StringVal(NFIValueList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->StringVal(index);
    }

    return NULL_STR;
}

static NFIDENTID NFVarList_ObjectVal(NFIValueList* pVarList, const int index)
{
    if (pVarList)
    {
        return pVarList->ObjectVal(index);
    }

    return 0;
}

class NFCScriptVarList
{
public:
    NFCScriptVarList()
    {

    }

    NFCScriptVarList(const NFIValueList& varList)
    {
        mVar = varList;
    }

    void Clear()
    {
        mVar.Clear();
    }

    // 是否为空
    bool IsEmpty() const
    {
        return mVar.IsEmpty();
    }

    int GetCount() const
    {
        return mVar.GetCount();
    }

    VARIANT_TYPE Type(const int index) const
    {
        return mVar.Type(index);
    }

    bool AddInt(const int value)
    {
        return mVar.AddInt(value);
    }

    bool AddFloat(const float value)
    {
        return mVar.AddFloat(value);
    }

    bool AddString(const char* value)
    {
        return mVar.AddString(value);
    }

    bool AddObject(const NFIDENTID& value)
    {
        return mVar.AddObject(value);
    }

    bool SetInt(const int index, const int value)
    {
        return mVar.SetInt(index, value);
    }

    bool SetFloat(const int index, const float value)
    {
        return mVar.SetFloat(index, value);
    }

    bool SetString(const int index, const char* value)
    {
        return mVar.SetString(index, value);
    }

    bool SetObject(const int index, const NFIDENTID& value)
    {
        return mVar.SetObject(index, value);
    }

    int IntVal(const int index) const
    {
        return mVar.IntVal(index);
    }

    float FloatVal(const int index) const
    {
        return mVar.FloatVal(index);
    }

    const std::string& StringVal(const int index) const
    {
        return mVar.StringVal(index);
    }

    NFIDENTID ObjectVal(const int index) const
    {
        return mVar.ObjectVal(index);
    }

    //////////dont't export this function////////////////////////////////////////////////////////////////
    const NFIValueList& GetVar() const
    {
        return mVar;
    }

protected:
private:
    NFCValueList mVar;
};


class NFScriptInt64
{
public:
    NFScriptInt64(){};
    NFScriptInt64(const NFIDENTID& ID)
    {
        mID = ID;
    }

    NFIDENTID ObjectVal() const
    {
        return mID;
    }

    NFINT64 Int64Val() const
    {
        return mID.nData64;
    }

public:
    NFINT32 GetA()
    {
        return mID.nIdent;
    }

    NFINT32 GetB()
    {
        return mID.nSerial;
    }

    void SetA( NFINT32 var)
    {
        mID.nIdent = var;
    }

    void SetB( NFINT32 var)
    {
        mID.nSerial = var;
    }

private:
    NFIDENTID mID;
};

//////////////////////////////////////////////////////////////////////////

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
    : public NFILogicModule
{

public:
    virtual int DoHeartBeatCommonCB(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var) = 0;
    virtual int DoEventCommonCB(const NFIDENTID& self, const int nEventID, const NFIValueList& var) = 0;

    //call script
    virtual int DoScript(const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;
    virtual int DoEventScript(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;
    virtual int DoHeartBeatScript(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;


    virtual int DoScriptPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg) = 0;
    virtual int DoScriptRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg) = 0;

};

class NFCScriptKernelModule : public NFMap<NFIDENTID, NFCSriptData>
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
        m_pEventProcessModule->DoEvent(objectID, nEventID, valueList.GetVar());
        return false;
    }

    bool ExistElement(const std::string& strConfigName)
    {
        return m_pElementInfoModule->ExistElement(strConfigName);
    }

    int QueryPropertyInt(const std::string& strConfigName, const std::string& strPropertyName)
    {
        return m_pElementInfoModule->QueryPropertyInt(strConfigName, strPropertyName);
    }

    float QueryPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName)
    {
        return m_pElementInfoModule->QueryPropertyFloat(strConfigName, strPropertyName);
    }

    const std::string& QueryPropertyString(const std::string& strConfigName, const std::string& strPropertyName)
    {
        return m_pElementInfoModule->QueryPropertyString(strConfigName, strPropertyName);
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

    int OnEventCommonCB(const NFIDENTID& self, const int nEventID, const NFIValueList& var)
    {
        m_pScriptModule->DoEventCommonCB(self, nEventID, var);
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

        m_pKernelModule->AddHeartBeat(self, strHeartBeatName, this, &NFCScriptKernelModule::OnHeartBeatCommonCB, NFCValueList(), fTime, nCount);

        return true;
    }
    
    int OnHeartBeatCommonCB(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var)
    {
        m_pScriptModule->DoHeartBeatCommonCB(self, strHeartBeat, fTime, nCount, var);
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
        NFIObject* pObject = m_pKernelModule->CreateObject(self, nContainerID, nGroupID, strClassName, strConfigIndex, arg.GetVar());
        if (pObject)
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

    int QueryPropertyInt(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->QueryPropertyInt(self, strPropertyName);
    }

    float QueryPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->QueryPropertyFloat(self, strPropertyName);
    }

    const std::string& QueryPropertyString(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->QueryPropertyString(self, strPropertyName);
    }

    NFIDENTID QueryPropertyObject(const NFIDENTID& self, const std::string& strPropertyName)
    {
        return m_pKernelModule->QueryPropertyObject(self, strPropertyName);
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

    int QueryRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {
        return m_pKernelModule->QueryRecordInt(self, strRecordName, nRow, nCol);
    }

    float QueryRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {

        return m_pKernelModule->QueryRecordFloat(self, strRecordName, nRow, nCol);
    }

    const std::string& QueryRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {
        return m_pKernelModule->QueryRecordString(self, strRecordName, nRow, nCol);
    }

    NFIDENTID QueryRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
    {
        return m_pKernelModule->QueryRecordObject(self, strRecordName, nRow, nCol);
    }

    bool AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const VARIANT_TYPE varType, bool bPublic ,  bool bPrivate ,  bool bSave, int nIndex, const std::string& strScriptFunction)
    {
        return m_pKernelModule->AddProperty(self, strPropertyName, varType, bPublic, bPrivate, bSave, nIndex, strScriptFunction);
    }

    bool AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFCScriptVarList& varData, const NFCScriptVarList& varKey, const NFCScriptVarList& varDesc, const NFCScriptVarList& varTag, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, int nIndex)
    {
        return m_pKernelModule->AddRecord(self, strRecordName, varData.GetVar(), varKey.GetVar(), varDesc.GetVar(), varTag.GetVar(), varTag.GetVar(), nRows, bPublic, bPrivate, bSave, nIndex);
    }

protected:

private:
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIScriptModule*m_pScriptModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

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

static int KernelModule_QueryPropertyInt(const NFScriptInt64& nPtrKernelModule,
                         const std::string& strConfigName, const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryPropertyInt(strConfigName, strPropertyName);
    }

    return 0;
}

static float KernelModule_QueryPropertyFloat(const NFScriptInt64& nPtrKernelModule,
                             const std::string& strConfigName, const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryPropertyFloat(strConfigName, strPropertyName);
    }

    return 0.0f;
}

static const std::string& KernelModule_QueryPropertyString(const NFScriptInt64& nPtrKernelModule,
                                const std::string& strConfigName, const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryPropertyString(strConfigName, strPropertyName);
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

    return false;
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

static int KernelModule_QueryPropertyInt(const NFScriptInt64& nKernelModule, const NFScriptInt64& self, 
                            const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryPropertyInt(self.ObjectVal(), strPropertyName);
    }

    return 0;
}

static float KernelModule_QueryPropertyFloat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryPropertyFloat(self.ObjectVal(), strPropertyName);
    }

    return 0.0f;
}

static const std::string& KernelModule_QueryPropertyString(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self,
                                              const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryPropertyString(self.ObjectVal(), strPropertyName);
    }

    return NULL_STR;
}

static NFIDENTID KernelModule_QueryPropertyObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                     const std::string& strPropertyName)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryPropertyObject(self.ObjectVal(), strPropertyName);
    }

    return 0;
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

static int KernelModule_QueryRecordInt(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                          const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryRecordInt(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return 0;
}

static float KernelModule_QueryRecordFloat(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                              const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryRecordFloat(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return 0.0f;
}

static const std::string& KernelModule_QueryRecordString(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                            const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryRecordString(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return NULL_STR;
}

static NFIDENTID KernelModule_QueryRecordObject(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, 
                                   const std::string& strRecordName, const int nRow, const int nCol)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->QueryRecordObject(self.ObjectVal(), strRecordName, nRow, nCol);
    }

    return 0;
}

static bool KernelModule_AddProperty(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self,
                        const std::string& strPropertyName, const VARIANT_TYPE varType, bool bPublic ,  bool bPrivate ,  
                        bool bSave, int nIndex, const std::string& strScriptFunction)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddProperty(self.ObjectVal(), strPropertyName, varType, bPublic, bPrivate, bSave, nIndex, strScriptFunction);
    }

    return false;
}

static bool KernelModule_AddRecord(const NFScriptInt64& nPtrKernelModule, const NFScriptInt64& self, const std::string& strRecordName, 
                      const NFCScriptVarList& varData, const NFCScriptVarList& varKey, const NFCScriptVarList& varDesc, const NFCScriptVarList& varTag, 
                      const int nRows, bool bPublic,  bool bPrivate,  bool bSave, int nIndex)
{
    NFCScriptKernelModule* pScriptKernelModule = (NFCScriptKernelModule*)nPtrKernelModule.Int64Val();
    if (pScriptKernelModule)
    {
        return pScriptKernelModule->AddRecord(self.ObjectVal(), strRecordName, varData, varKey, varDesc, varTag, nRows, bPublic, bPrivate, bSave, nIndex);
    }

    return false;
}

#endif