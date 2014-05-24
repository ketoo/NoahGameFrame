// -------------------------------------------------------------------------
//    @FileName      :    NFCKernelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_KERNEL_MODULE_H_
#define _NFC_KERNEL_MODULE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <boost/random.hpp>
#include "NFComm/NFCore/NFIdentID.h"
#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFCValueList.h"
#include "NFComm/NFCore/NFCRecord.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"
#include "NFComm/NFKernelPlugin/NFContainerModule/NFCContainerModule.h"

class NFCKernelModule
    : public NFIKernelModule,
  public NFMap<NFIDENTID, NFIObject>
{
public:
    NFCKernelModule(NFIPluginManager* p);
    virtual ~NFCKernelModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual void OnReload(const char* strModuleName, NFILogicModule* pModule);

    ///////////////////////////////////////////////////////////////////////

    virtual bool FindHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName);

    virtual bool RemoveHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName);


    virtual NFIDENTID NewIdentID();
    virtual void SetIdentSerialID(int nSerialID);
    
    virtual void SetIdentID(NFINT32 nID);
    virtual NFINT32 GetIdentID();

    virtual int Command(const NFIValueList& var);

    virtual bool IsContainer(const NFIDENTID& self);

    virtual bool ExistContainer(const int nContainerIndex);

    virtual int Type(const NFIDENTID& self);

    virtual NFIObject* GetObject(const NFIDENTID& ident);

    virtual NFIObject* CreateObject(const NFIDENTID& self, const int nContainerID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFIValueList& arg);

    virtual bool DestroyObject(const NFIDENTID& self);

    virtual bool DestroyAll();

    virtual bool DestroySelf(const NFIDENTID& self);

    //////////////////////////////////////////////////////////////////////////

    virtual bool FindProperty(const NFIDENTID& self, const std::string& strPropertyName);

    virtual bool SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const int nValue);
    virtual bool SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName,  const float fValue);
    virtual bool SetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName, const double dValue);
    virtual bool SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& objectValue);

    virtual int QueryPropertyInt(const NFIDENTID& self, const std::string& strPropertyName);
    virtual float QueryPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName);
    virtual double QueryPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName);
    virtual const std::string& QueryPropertyString(const NFIDENTID& self, const std::string& strPropertyName);
    virtual NFIDENTID QueryPropertyObject(const NFIDENTID& self, const std::string& strPropertyName);

    virtual NFIRecord* FindRecord(const NFIDENTID& self, const std::string& strRecordName);

    virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const int nValue);
    virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue);
    virtual bool SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& objectValue);

    virtual int QueryRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual float QueryRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual double QueryRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& QueryRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual NFIDENTID QueryRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);

    virtual bool SwitchScene(const NFIDENTID& self, const int nTargetSceneID, const int nTargetGroupID, const float fX, const float fY, const float fZ, const float fOrient, const NFIValueList& arg);

    virtual void AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const VARIANT_TYPE varType, bool bPublic ,  bool bPrivate ,  bool bSave, int nIndex, const std::string& strScriptFunction);
    virtual void AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIValueList& varData, const NFIValueList& varKey, const NFIValueList& varDesc, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, int nIndex);
    ////////////////////////////////////////////////////////////////

    virtual NFIDENTID CreateContainer(const int nContainerIndex, const std::string& strSceneConfigID);

    virtual bool DestroyContainer(const int nContainerIndex);

    virtual int GetOnLineCount();

    virtual int GetMaxOnLineCount();

    virtual int GetContainerOnLineCount(const int nContainerID);

    virtual int GetContainerOnLineCount(const int nContainerID, const int nGroupID);

    virtual int GetContainerOnLineList(const int nContainerID, NFIValueList& var);
    virtual int GetAllContainerObjectList(NFIValueList& var);

    virtual int RequestGroupScene(const int nContainerID, const std::string& strSceneResource);

    //virtual int AddObjectToGroup(const int nContainerID,const int nGroupID, const NFIValueList& var);

    virtual bool ReleaseGroupScene(const int nContainerID, const int nGroupID);

    virtual bool GetGroupObjectList(const int nContainerID, const int nGroupID, NFIValueList& list);

    virtual NFIDENTID GetGridID(const float fX, const float fY, const float fZ);

    virtual bool GetAroundGrid(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIValueList& list);

    virtual bool GetGridObjectList(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIValueList& list);

    virtual bool GetRangObjectList(const NFIDENTID& self, const int nContainerID, const int nGroupID, const float fRang, NFIValueList& list);

    virtual bool GetRangObjectList(const float fX, const float fY, const float fZ, const int nContainerID, const int nGroupID, const float fRang, NFIValueList& list);

    virtual int GetObjectByProperty(const int nContainerID, const std::string& strPropertyName, const NFIValueList& valueArgArg, NFIValueList& list);

    virtual void Random(int nStart, int nEnd, int nCount, NFIValueList& valueList);

    //////////////////////////////////////////////////////////////////////////
    virtual bool LogStack();

    virtual bool LogInfo(const NFIDENTID ident);

    virtual bool LogSelfInfo(const NFIDENTID ident);

protected:

    //只能网络模块注册，回调用来同步对象类事件,所有的类对象都会回调
    virtual bool ResgisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb);

    //只能网络模块注册，回调用来同步对象属性事件,所有的类属性都会回调
    virtual bool ResgisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    //只能网络模块注册，回调用来同步对象类表事件,所有的类表都会回调
    virtual bool ResgisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb);
   
    //////////////////////////////////////////////////////////////////////////
    virtual bool AddEventCallBack(const NFIDENTID& self, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb);
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);

protected:
    void InitRandom();

protected:

    int OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);
    int OnPropertyCommonEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar);
    int OnRecordCommonEvent(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& arg);
protected:

    std::list<NFIDENTID> mtDeleteSelfList;

    //////////////////////////////////////////////////////////////////////////
    //通用对象类事件回调,以便同步
    std::list<CLASS_EVENT_FUNCTOR_PTR> mtCommonClassCallBackListEx;
    //通用属性变动回调,以便同步
    std::list<PROPERTY_EVENT_FUNCTOR_PTR> mtCommonPropertyCallBackListEx;
    //通用表变动回调,以便同步
    std::list<RECORD_EVENT_FUNCTOR_PTR> mtCommonRecordCallBackListEx;

private:
	//属性的KEY，比如HP=1，会以这个建立KEY，那么可以快速查询所有HP=1的对象而不用遍历
    //     std::map<std::string,std::map<VarData, NFList<NFIDENTID>>>
    //     std::map<"Scene", std::map<10, NFList<NFIDENTID>>>

private:
    std::vector<float> mvRandom;
    int mnRandomPos;

    NFUINT32 mnIdentID;
    NFIDENTID mnIdentIndex;
    NFIDENTID mnCurExeObject;
    float fLastTotal;

    NFIContainerModule* m_pContainerModule;

    NFILogModule* m_pLogModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif
