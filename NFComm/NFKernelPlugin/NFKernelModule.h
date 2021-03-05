/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NF_KERNEL_MODULE_H
#define NF_KERNEL_MODULE_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFIRecord.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFICellModule.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"


class NFKernelModule
    : public NFIKernelModule,
  public NFMapEx<NFGUID, NFIObject>
{
public:
    NFKernelModule(NFIPluginManager* p);
    virtual ~NFKernelModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute();

    ///////////////////////////////////////////////////////////////////////

    virtual bool ExistScene(const int sceneID);

	virtual bool ExistObject(const NFGUID& ident);
    virtual bool ObjectReady(const NFGUID& ident);
	virtual bool ExistObject(const NFGUID& ident, const int sceneID, const int groupID);

    virtual NF_SHARE_PTR<NFIObject> GetObject(const NFGUID& ident);
    virtual NF_SHARE_PTR<NFIObject> CreateObject(const NFGUID& self, const int sceneID, const int groupID, const std::string& className, const std::string& configIndex, const NFDataList& arg);

    virtual bool DestroyAll();
    virtual bool DestroySelf(const NFGUID& self);
    virtual bool DestroyObject(const NFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    virtual bool FindProperty(const NFGUID& self, const std::string& propertyName);

    virtual bool SetPropertyInt(const NFGUID& self, const std::string& propertyName, const NFINT64 nValue, const NFINT64 reason = 0);
    virtual bool SetPropertyFloat(const NFGUID& self, const std::string& propertyName, const double dValue, const NFINT64 reason = 0);
    virtual bool SetPropertyString(const NFGUID& self, const std::string& propertyName, const std::string& value, const NFINT64 reason = 0);
    virtual bool SetPropertyObject(const NFGUID& self, const std::string& propertyName, const NFGUID& objectValue, const NFINT64 reason = 0);
	virtual bool SetPropertyVector2(const NFGUID& self, const std::string& propertyName, const NFVector2& value, const NFINT64 reason = 0);
	virtual bool SetPropertyVector3(const NFGUID& self, const std::string& propertyName, const NFVector3& value, const NFINT64 reason = 0);

    virtual NFINT64 GetPropertyInt(const NFGUID& self, const std::string& propertyName);
	virtual int GetPropertyInt32(const NFGUID& self, const std::string& propertyName);	//equal to (int)GetPropertyInt(...), to remove C4244 warning
    virtual double GetPropertyFloat(const NFGUID& self, const std::string& propertyName);
    virtual const std::string& GetPropertyString(const NFGUID& self, const std::string& propertyName);
    virtual const NFGUID& GetPropertyObject(const NFGUID& self, const std::string& propertyName);
	virtual const NFVector2& GetPropertyVector2(const NFGUID& self, const std::string& propertyName);
	virtual const NFVector3& GetPropertyVector3(const NFGUID& self, const std::string& propertyName);

    //////////////////////////////////////////////////////////////////////////
    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const NFGUID& self, const std::string& recordName);
    virtual bool ClearRecord(const NFGUID& self, const std::string& recordName);

    virtual bool SetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFINT64 nValue);
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const int col, const double dwValue);
    virtual bool SetRecordString(const NFGUID& self, const std::string& recordName, const int row, const int col, const std::string& value);
    virtual bool SetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFGUID& objectValue);
	virtual bool SetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFVector2& value);
	virtual bool SetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const int col, const NFVector3& value);

    virtual bool SetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value);
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const double value);
    virtual bool SetRecordString(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const std::string& value);
    virtual bool SetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value);
	virtual bool SetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value);
	virtual bool SetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value);

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const int col);
    virtual double GetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const int col);
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& recordName, const int row, const int col);
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const int col);
	virtual const NFVector2& GetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const int col);
	virtual const NFVector3& GetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const int col);

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
    virtual double GetRecordFloat(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
	virtual const NFVector2& GetRecordVector2(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);
	virtual const NFVector3& GetRecordVector3(const NFGUID& self, const std::string& recordName, const int row, const std::string& colTag);

    ////////////////////////////////////////////////////////////////
    virtual NFGUID CreateGUID();

    virtual bool CreateScene(const int sceneID);
    virtual bool DestroyScene(const int sceneID);

    virtual int GetOnLineCount();
    virtual int GetMaxOnLineCount();

    virtual int RequestGroupScene(const int sceneID);
    virtual bool ReleaseGroupScene(const int sceneID, const int groupID);
    virtual bool ExitGroupScene(const int sceneID, const int groupID);

	virtual int GetGroupObjectList(const int sceneID, const int groupID, const bool bPlayer, const NFGUID& noSelf);
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list);
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list, const NFGUID& noSelf);
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list, const bool bPlayer);
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, NFDataList& list, const bool bPlayer, const NFGUID& noSelf);
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, const std::string& className, NFDataList& list);
	virtual bool GetGroupObjectList(const int sceneID, const int groupID, const std::string& className, NFDataList& list, const NFGUID& noSelf);
	
	virtual int GetObjectByProperty(const int sceneID, const int groupID, const std::string& propertyName, const NFDataList& valueArgArg, NFDataList& list);

	virtual int Random(int nStart, int nEnd);//return [nStart, nEnd)
	virtual float Random();//return [0f, 1f)

    //////////////////////////////////////////////////////////////////////////
    virtual bool LogStack();
    virtual bool LogInfo(const NFGUID ident);
    virtual bool LogSelfInfo(const NFGUID ident);

    //////////////////////////////////////////////////////////////////////////

    virtual bool DoEvent(const NFGUID& self, const std::string& className, CLASS_OBJECT_EVENT eEvent, const NFDataList& valueList);

protected:

    
    virtual bool RegisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb);
    virtual bool RegisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb);
    virtual bool RegisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb);

	virtual bool RegisterClassPropertyEvent(const std::string& className, const PROPERTY_EVENT_FUNCTOR_PTR& cb);
	virtual bool RegisterClassRecordEvent(const std::string& className, const RECORD_EVENT_FUNCTOR_PTR& cb);
protected:

    virtual bool AddClassCallBack(const std::string& className, const CLASS_EVENT_FUNCTOR_PTR& cb);

    void InitRandom();

    int OnClassCommonEvent(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var);
    int OnPropertyCommonEvent(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar, const NFINT64 reason);
    int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

    void ProcessMemFree();

protected:

    std::list<NFGUID> mtDeleteSelfList;

    //////////////////////////////////////////////////////////////////////////
    
    std::list<CLASS_EVENT_FUNCTOR_PTR> mtCommonClassCallBackList;
    std::list<PROPERTY_EVENT_FUNCTOR_PTR> mtCommonPropertyCallBackList;
    std::list<RECORD_EVENT_FUNCTOR_PTR> mtCommonRecordCallBackList;

	std::map<std::string, std::list<PROPERTY_EVENT_FUNCTOR_PTR>> mtClassPropertyCallBackList;
	std::map<std::string, std::list<RECORD_EVENT_FUNCTOR_PTR>> mtClassRecordCallBackList;

private:
    std::vector<float> mvRandom;
	int nGUIDIndex;
    //int mnRandomPos;
	decltype(mvRandom.cbegin()) mxRandomItor;

    NFGUID mnCurExeObject;
    NFINT64 nLastTime;

	NFISceneModule* m_pSceneModule;
    NFILogModule* m_pLogModule;
    NFIClassModule* m_pClassModule;
    NFIElementModule* m_pElementModule;
	NFIScheduleModule* m_pScheduleModule;
	NFIEventModule* m_pEventModule;
    NFICellModule* m_pCellModule;
    NFIThreadPoolModule* m_pThreadPoolModule;

};

#endif
