// -------------------------------------------------------------------------
//    @FileName         £º    NFIScriptModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-07-08
//    @Module           £º    NFIScriptModule
//    @Desc             :     every script plugin need to export two interfaces(NFCScriptVarList and most of NFCScriptKernel)
// -------------------------------------------------------------------------

#ifndef _NFI_SCRIPT_KERNEL_MODULE_H_
#define _NFI_SCRIPT_KERNEL_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm\NFCore\NFCDataList.h"
#include "NFComm\NFCore\NFIdentID.h"
#include "NFComm\NFPluginModule\NFIElementInfoModule.h"
#include "NFComm\NFPluginModule\NFIKernelModule.h"
#include "NFComm\NFCore\NFMap.h"

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

private:
    NFIDENTID mID;
};

class NFCScriptVarList
{
public:
    NFCScriptVarList()
    {

    }

    NFCScriptVarList(const NFIDataList& varList)
    {
        mVar = varList;
    }

    void Clear()
    {
        mVar.Clear();
    }

    // ÊÇ·ñÎª¿Õ
    bool IsEmpty() const
    {
        return mVar.IsEmpty();
    }

    int GetCount() const
    {
        return mVar.GetCount();
    }

    TDATA_TYPE Type(const int index) const
    {
        return mVar.Type(index);
    }

    bool AddInt(const NFINT64 value)
    {
        return mVar.Add(value);
    }

    bool AddFloat(const float value)
    {
        return mVar.Add(value);
    }

    bool AddString(const char* value)
    {
        return mVar.Add(value);
    }

    bool AddObject(const NFScriptInt64& value)
    {
        return mVar.Add(value.ObjectVal());
    }

    bool SetInt(const int index, const NFINT64 value)
    {
        return mVar.Set(index, value);
    }

    bool SetFloat(const int index, const float value)
    {
        return mVar.Set(index, value);
    }

    bool SetString(const int index, const char* value)
    {
        return mVar.Set(index, value);
    }

    bool SetObject(const int index, const NFScriptInt64& value)
    {
        return mVar.Set(index, value.ObjectVal());
    }

    NFINT64 Int(const int index) const   
    {
        return mVar.Int(index);
    }

    float Float(const int index) const
    {
        return mVar.Float(index);
    }

    const std::string& String(const int index) const
    {
        return mVar.String(index);
    }

    NFScriptInt64 Object(const int index) const
    {
        return mVar.Object(index);
    }

    //////////dont't export this function////////////////////////////////////////////////////////////////
    const NFIDataList& GetVar() const
    {
        return mVar;
    }

protected:
private:
    NFCDataList mVar;
};


class NFIScriptKernelModule : public NFMap<NFIDENTID, NFCSriptData>
{
    virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool RemoveEvent(const NFIDENTID& objectID) = 0;

    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFCScriptVarList& valueList) = 0;

    virtual bool ExistElement(const std::string& strConfigName) = 0;

    virtual int GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName) = 0;

    virtual float GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName) = 0;

    virtual const std::string& GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName) = 0;

    virtual bool AddPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction) = 0;

    virtual bool AddRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction) = 0;

    virtual bool AddEventCallBack(const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction) = 0;

    virtual int OnEventCommonCB(const NFIDENTID& self, const int nEventID, const NFIDataList& var) = 0;

    virtual bool AddHeartBeat(const NFIDENTID self, const std::string& strHeartBeatName, const std::string& strComponentName, const std::string& strFunction, const float fTime, const int nCount) = 0;

    virtual int OnHeartBeatCommonCB(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var) = 0;

    virtual bool FindHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName) = 0;

    virtual bool RemoveHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName) = 0;

    //////////////////////////////////////////////////////////////////
    virtual bool SetComponentEnable(const NFIDENTID& self, const std::string& strComponentName, const bool bEnable) = 0;

    virtual bool QueryComponentEnable(const NFIDENTID& self, const std::string& strComponentName) = 0;

    virtual NFIDENTID CreateContainer(const int nContainerIndex, const std::string& strSceneConfigID) = 0;

    virtual bool ExistContainer(const int nContainerIndex) = 0;

    virtual bool CreateObject(const NFIDENTID& self, const int nContainerID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFCScriptVarList& arg) = 0;

    virtual bool DestroyObject(const NFIDENTID& self) = 0;

    virtual bool FindProperty(const NFIDENTID& self, const std::string& strPropertyName) = 0;

    virtual bool SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const int nValue) = 0;

    virtual bool SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName,  const float fValue) = 0;

    virtual bool SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue) = 0;

    virtual bool SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& objectValue) = 0;

    virtual int GetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName) = 0;

    virtual float GetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName) = 0;

    virtual const std::string& GetPropertyString(const NFIDENTID& self, const std::string& strPropertyName) = 0;

    virtual NFIDENTID GetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName) = 0;

    virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const int nValue) = 0;

    virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue) = 0;

    virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;

    virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& objectValue) = 0;

    virtual int GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual float GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual bool AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic ,  bool bPrivate ,  bool bSave, bool bView, int nIndex, const std::string& strScriptFunction) = 0;

    virtual bool AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFCScriptVarList& TData, const NFCScriptVarList& varKey, const NFCScriptVarList& varDesc, const NFCScriptVarList& varTag, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex) = 0;
};
#endif

