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
#include "NFIElementInfoModule.h"

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
        mVar.IsEmpty();
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

class NFIScriptModule;
class NFCScriptKernelModule : public NFMap<NFIDENTID, NFCSriptData>
{
public:
    NFCScriptKernelModule(NFIKernelModule* pKernelModule, NFIScriptModule* pScriptModule, NFIElementInfoModule* pElementInfoModule)
    {
        m_pKernelModule = pKernelModule;
        m_pScriptModule = pScriptModule;
        m_pElementInfoModule = pElementInfoModule;
    }

    bool QueryEnableComponent(const NFIDENTID self, const std::string& strComponentName)
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (!pObject)
        {
            return false;
        }

        if(!pObject->GetComponentManager()->QueryEnable(strComponentName))
        {
            return false;
        }

        return true;
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

        return true;
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

        return true;
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
        return m_pKernelModule->AddRecord(self, strRecordName, varData.GetVar(), varKey.GetVar(), varDesc.GetVar(), varTag.GetVar(), nRows, bPublic, bPrivate, bSave, nIndex);
    }

protected:

private:
    NFIKernelModule* m_pKernelModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIScriptModule*m_pScriptModule;
};

class NFIScriptModule
    : public NFILogicModule
{

public:
    //call script
    virtual int DoScript(const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;
    virtual int DoEventScript(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;
    virtual int DoHeartBeatScript(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg) = 0;


    virtual int DoScriptPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& neVar, const NFCScriptVarList& arg) = 0;
    virtual int DoScriptRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg) = 0;

};

#endif