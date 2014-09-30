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
#include "NFComm/NFCore/NFCDataList.h"
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

    ///////////////////////////////////////////////////////////////////////

    virtual bool FindHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName);

    virtual bool RemoveHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName);


    virtual NFIDENTID NewIdentID();
    virtual void SetIdentSerialID(int nSerialID);
    
    virtual void SetIdentID(NFINT32 nID);
    virtual NFINT32 GetIdentID();

    virtual int Command(const NFIDataList& var);

    virtual bool IsContainer(const NFIDENTID& self);

    virtual bool ExistContainer(const int nContainerIndex);

    virtual int Type(const NFIDENTID& self);

    virtual NFIObject* GetObject(const NFIDENTID& ident);

    virtual NFIObject* CreateObject(const NFIDENTID& self, const int nContainerID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFIDataList& arg);

    virtual bool DestroyObject(const NFIDENTID& self);

    virtual bool DestroyAll();

    virtual bool DestroySelf(const NFIDENTID& self);

    //////////////////////////////////////////////////////////////////////////
    virtual bool SetComponentEnable(const NFIDENTID& self, const std::string& strComponentName, const bool bEnable);
    virtual bool QueryComponentEnable(const NFIDENTID& self, const std::string& strComponentName);

    virtual bool FindProperty(const NFIDENTID& self, const std::string& strPropertyName);

    virtual bool SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const int nValue);
    virtual bool SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName,  const float fValue);
    virtual bool SetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName, const double dValue);
    virtual bool SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& objectValue);

    virtual int GetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName);
    virtual float GetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName);
    virtual double GetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const NFIDENTID& self, const std::string& strPropertyName);
    virtual NFIDENTID GetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName);

    //////////////////////////////////////////////////////////////////////////
    virtual NFIRecord* FindRecord(const NFIDENTID& self, const std::string& strRecordName);

    virtual bool ClearRecord(const NFIDENTID& self, const std::string& strRecordName);

    virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const int nValue);
    virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue);
    virtual bool SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& objectValue);
    
    virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const int value);
    virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value);
    virtual bool SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
    virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value);

    virtual int GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual float GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual double GetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);

    virtual int GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual float GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual double GetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);

    virtual bool SwitchScene(const NFIDENTID& self, const int nTargetSceneID, const int nTargetGroupID, const float fX, const float fY, const float fZ, const float fOrient, const NFIDataList& arg);

    virtual bool AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic ,  bool bPrivate ,  bool bSave, bool bView, int nIndex, const std::string& strScriptFunction);
    virtual bool AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& TData, const NFIDataList& varKey, const NFIDataList& varDesc, const NFIDataList& varTag, const NFIDataList& varRelatedRecord, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex);
    ////////////////////////////////////////////////////////////////

    virtual NFIDENTID CreateContainer(const int nContainerIndex, const std::string& strSceneConfigID);

    virtual bool DestroyContainer(const int nContainerIndex);

    virtual int GetOnLineCount();

    virtual int GetMaxOnLineCount();

    virtual int GetContainerOnLineCount(const int nContainerID);

    virtual int GetContainerOnLineCount(const int nContainerID, const int nGroupID);

    virtual int GetContainerOnLineList(const int nContainerID, NFIDataList& var);
    virtual int GetAllContainerObjectList(NFIDataList& var);

    virtual int RequestGroupScene(const int nContainerID);

    //virtual int AddObjectToGroup(const int nContainerID,const int nGroupID, const NFIDataList& var);

    virtual bool ReleaseGroupScene(const int nContainerID, const int nGroupID);

    virtual bool GetGroupObjectList(const int nContainerID, const int nGroupID, NFIDataList& list);

    virtual NFIDENTID GetGridID(const float fX, const float fY, const float fZ);

    virtual bool GetAroundGrid(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIDataList& list);

    virtual bool GetGridObjectList(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIDataList& list);

    virtual bool GetRangObjectList(const NFIDENTID& self, const int nContainerID, const int nGroupID, const float fRang, NFIDataList& list);

    virtual bool GetRangObjectList(const float fX, const float fY, const float fZ, const int nContainerID, const int nGroupID, const float fRang, NFIDataList& list);

    virtual int GetObjectByProperty(const int nContainerID, const std::string& strPropertyName, const NFIDataList& valueArgArg, NFIDataList& list);

    virtual void Random(int nStart, int nEnd, int nCount, NFIDataList& valueList);

    //////////////////////////////////////////////////////////////////////////
    virtual bool LogStack();

    virtual bool LogInfo(const NFIDENTID ident);

    virtual bool LogSelfInfo(const NFIDENTID ident);

protected:

    //ֻ������[�ű�]ģ��ע�ᣬ�ص�����ͬ���������¼�,���е�����󶼻�ص�
    virtual bool ResgisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb);

    //ֻ������[�ű�]ģ��ע�ᣬ�ص�����ͬ�����������¼�,���е������Զ���ص�
    virtual bool ResgisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    //ֻ������[�ű�]ģ��ע�ᣬ�ص�����ͬ����������¼�,���е������ص�
    virtual bool ResgisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb);
   
//     //ֻ������[�ű�]ģ��ע�ᣬ�ص�����,���е���������ص�
//     virtual bool ResgisterCommonHeartBeat(const HEART_BEAT_FUNCTOR_PTR& cb);
// 
//     //ֻ������[�ű�]ģ��ע�ᣬ�ص��¼�,���е��¼�����ص�
//     virtual bool ResgisterCommonEvent(const EVENT_PROCESS_FUNCTOR_PTR& cb);

protected:

    virtual bool AddEventCallBack(const NFIDENTID& self, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb);
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);

    virtual bool AddRecordCallBack(const NFIDENTID& self, const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb);
    virtual bool AddPropertyCallBack(const NFIDENTID& self, const std::string& strPropertyName, const PROPERTY_EVENT_FUNCTOR_PTR& cb);
    virtual bool AddHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIDataList& var, const float fTime, const int nCount);

protected:
    void InitRandom();

protected:

    int OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
    int OnPropertyCommonEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar);
    int OnRecordCommonEvent(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& arg);

//     int OnHeartBeatCommonCB(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var);
//     int OnEventCommonCB(const NFIDENTID& self, const int nEventID, const NFIDataList& var);

protected:

    std::list<NFIDENTID> mtDeleteSelfList;

    //////////////////////////////////////////////////////////////////////////
    //ͨ�ö������¼��ص�,�Ա�ͬ��
    std::list<CLASS_EVENT_FUNCTOR_PTR> mtCommonClassCallBackList;
    //ͨ�����Ա䶯�ص�,�Ա�ͬ��
    std::list<PROPERTY_EVENT_FUNCTOR_PTR> mtCommonPropertyCallBackList;
    //ͨ�ñ�䶯�ص�,�Ա�ͬ��
    std::list<RECORD_EVENT_FUNCTOR_PTR> mtCommonRecordCallBackList;
//     //ͨ�������ص�
//     std::list<HEART_BEAT_FUNCTOR_PTR> mtCommonHeartBeatCallBackList;
//     //ͨ���¼��ص�
//     std::list<EVENT_PROCESS_FUNCTOR_PTR> mtCommonEventCallBackList;
private:
	//���Ե�KEY������HP=1�������������KEY����ô���Կ��ٲ�ѯ����HP=1�Ķ�������ñ���
    //     std::map<std::string,std::map<TData, NFList<NFIDENTID>>>
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
