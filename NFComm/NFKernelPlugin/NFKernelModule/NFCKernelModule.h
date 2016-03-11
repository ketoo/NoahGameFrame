// -------------------------------------------------------------------------
//    @FileName      :    NFCKernelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_KERNEL_MODULE_H
#define NFC_KERNEL_MODULE_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFCore/NFIRecord.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"

class NFCKernelModule
    : public NFIKernelModule,
  public NFMapEx<NFGUID, NFIObject>
{
public:
    NFCKernelModule(NFIPluginManager* p);
    virtual ~NFCKernelModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute();

    ///////////////////////////////////////////////////////////////////////

    virtual bool FindHeartBeat(const NFGUID& self, const std::string& strHeartBeatName);
    virtual bool RemoveHeartBeat(const NFGUID& self, const std::string& strHeartBeatName);

    virtual bool IsContainer(const NFGUID& self);
    virtual bool ExistContainer(const int nSceneID);

    virtual NF_SHARE_PTR<NFIObject> GetObject(const NFGUID& ident);
    virtual NF_SHARE_PTR<NFIObject> CreateObject(const NFGUID& self, const int nSceneID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFIDataList& arg);

    virtual bool DestroyAll();
    virtual bool DestroySelf(const NFGUID& self);
    virtual bool DestroyObject(const NFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    virtual bool FindProperty(const NFGUID& self, const std::string& strPropertyName);

    virtual bool SetPropertyInt(const NFGUID& self, const std::string& strPropertyName, const NFINT64 nValue);
    virtual bool SetPropertyFloat(const NFGUID& self, const std::string& strPropertyName, const double dValue);
    virtual bool SetPropertyString(const NFGUID& self, const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const NFGUID& self, const std::string& strPropertyName, const NFGUID& objectValue);

    virtual NFINT64 GetPropertyInt(const NFGUID& self, const std::string& strPropertyName);
    virtual double GetPropertyFloat(const NFGUID& self, const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const NFGUID& self, const std::string& strPropertyName);
    virtual const NFGUID& GetPropertyObject(const NFGUID& self, const std::string& strPropertyName);

    //////////////////////////////////////////////////////////////////////////
    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const NFGUID& self, const std::string& strRecordName);
    virtual bool ClearRecord(const NFGUID& self, const std::string& strRecordName);

    virtual bool SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue);
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue);

    virtual bool SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
    virtual bool SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value);

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual double GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual double GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);

    virtual bool SwitchScene(const NFGUID& self, const int nTargetSceneID, const int nTargetGroupID, const float fX, const float fY, const float fZ, const float fOrient, const NFIDataList& arg);

    virtual bool AddProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex, const std::string& strScriptFunction);
    virtual bool AddRecord(const NFGUID& self, const std::string& strRecordName, const NFIDataList& TData, const NFIDataList& varKey, const NFIDataList& varDesc, const NFIDataList& varTag, const NFIDataList& varRelatedRecord, const int nRows, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex);
    ////////////////////////////////////////////////////////////////

    virtual bool CreateScene(const int nSceneID);
    virtual bool DestroyScene(const int nSceneID);

    virtual int GetOnLineCount();
    virtual int GetMaxOnLineCount();
    virtual int GetSceneOnLineCount(const int nSceneID);
    virtual int GetSceneOnLineCount(const int nSceneID, const int nGroupID);

    virtual int GetSceneOnLineList(const int nSceneID, NFIDataList& var);

    virtual int RequestGroupScene(const int nSceneID);
    virtual bool ReleaseGroupScene(const int nSceneID, const int nGroupID);
    virtual bool ExitGroupScene(const int nSceneID, const int nGroupID);

    virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, NFIDataList& list);
    virtual int GetObjectByProperty(const int nSceneID, const std::string& strPropertyName, const NFIDataList& valueArgArg, NFIDataList& list);

    virtual void Random(int nStart, int nEnd, int nCount, NFIDataList& valueList);

    //////////////////////////////////////////////////////////////////////////
    virtual bool LogStack();
    virtual bool LogInfo(const NFGUID ident);
    virtual bool LogSelfInfo(const NFGUID ident);

    //////////////////////////////////////////////////////////////////////////

    virtual bool DoEvent(const NFGUID& self, const std::string& strClassName, CLASS_OBJECT_EVENT eEvent, const NFIDataList& valueList);
    virtual bool DoEvent(const NFGUID& self, const int nEventID, const NFIDataList& valueList);

protected:

    //只能网络[脚本]模块注册，回调用来同步对象类事件,所有的类对象都会回调
    virtual bool ResgisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb);

    //只能网络[脚本]模块注册，回调用来同步对象属性事件,所有的类属性都会回调
    virtual bool ResgisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    //只能网络[脚本]模块注册，回调用来同步对象类表事件,所有的类表都会回调
    virtual bool ResgisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb);

protected:

    virtual bool AddEventCallBack(const NFGUID& self, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb);
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);

    void InitRandom();

    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
    int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

    void ProcessMemFree();

protected:

    std::list<NFGUID> mtDeleteSelfList;

    //////////////////////////////////////////////////////////////////////////
    //通用对象类事件回调,以便同步
    std::list<CLASS_EVENT_FUNCTOR_PTR> mtCommonClassCallBackList;
    //通用属性变动回调,以便同步
    std::list<PROPERTY_EVENT_FUNCTOR_PTR> mtCommonPropertyCallBackList;
    //通用表变动回调,以便同步
    std::list<RECORD_EVENT_FUNCTOR_PTR> mtCommonRecordCallBackList;

private:
    //属性的KEY，比如HP=1，会以这个建立KEY，那么可以快速查询所有HP=1的对象而不用遍历
    //     std::map<std::string,std::map<TData, NFList<NFGUID>>>
    //     std::map<"Scene", std::map<10, NFList<NFGUID>>>

private:
    std::vector<float> mvRandom;
    int mnRandomPos;

    NFGUID mnCurExeObject;
    NFINT64 nLastTime;

    NFISceneModule* m_pSceneModule;
    NFILogModule* m_pLogModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIUUIDModule* m_pUUIDModule;
};

#endif
