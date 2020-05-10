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

    virtual bool ExistScene(const int nSceneID);

	virtual bool ExistObject(const NFGUID& ident);
    virtual bool ObjectReady(const NFGUID& ident);
	virtual bool ExistObject(const NFGUID& ident, const int nSceneID, const int nGroupID);

    virtual NF_SHARE_PTR<NFIObject> GetObject(const NFGUID& ident);
    virtual NF_SHARE_PTR<NFIObject> CreateObject(const NFGUID& self, const int nSceneID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFDataList& arg);

    virtual bool DestroyAll();
    virtual bool DestroySelf(const NFGUID& self);
    virtual bool DestroyObject(const NFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    virtual bool FindProperty(const NFGUID& self, const std::string& strPropertyName);

    virtual bool SetPropertyInt(const NFGUID& self, const std::string& strPropertyName, const NFINT64 nValue);
    virtual bool SetPropertyFloat(const NFGUID& self, const std::string& strPropertyName, const double dValue);
    virtual bool SetPropertyString(const NFGUID& self, const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const NFGUID& self, const std::string& strPropertyName, const NFGUID& objectValue);
	virtual bool SetPropertyVector2(const NFGUID& self, const std::string& strPropertyName, const NFVector2& value);
	virtual bool SetPropertyVector3(const NFGUID& self, const std::string& strPropertyName, const NFVector3& value);

    virtual NFINT64 GetPropertyInt(const NFGUID& self, const std::string& strPropertyName);
	virtual int GetPropertyInt32(const NFGUID& self, const std::string& strPropertyName);	//equal to (int)GetPropertyInt(...), to remove C4244 warning
    virtual double GetPropertyFloat(const NFGUID& self, const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const NFGUID& self, const std::string& strPropertyName);
    virtual const NFGUID& GetPropertyObject(const NFGUID& self, const std::string& strPropertyName);
	virtual const NFVector2& GetPropertyVector2(const NFGUID& self, const std::string& strPropertyName);
	virtual const NFVector3& GetPropertyVector3(const NFGUID& self, const std::string& strPropertyName);

    //////////////////////////////////////////////////////////////////////////
    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const NFGUID& self, const std::string& strRecordName);
    virtual bool ClearRecord(const NFGUID& self, const std::string& strRecordName);

    virtual bool SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue);
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue);
	virtual bool SetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& value);
	virtual bool SetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& value);

    virtual bool SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
    virtual bool SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value);
	virtual bool SetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value);
	virtual bool SetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value);

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual double GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual const NFVector2& GetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual const NFVector3& GetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual double GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const NFVector2& GetRecordVector2(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const NFVector3& GetRecordVector3(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);

    ////////////////////////////////////////////////////////////////
    virtual NFGUID CreateGUID();

    virtual bool CreateScene(const int nSceneID);
    virtual bool DestroyScene(const int nSceneID);

    virtual int GetOnLineCount();
    virtual int GetMaxOnLineCount();

    virtual int RequestGroupScene(const int nSceneID);
    virtual bool ReleaseGroupScene(const int nSceneID, const int nGroupID);
    virtual bool ExitGroupScene(const int nSceneID, const int nGroupID);

	virtual int GetGroupObjectList(const int nSceneID, const int nGroupID, const bool bPlayer, const NFGUID& noSelf);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList& list);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList& list, const NFGUID& noSelf);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList& list, const bool bPlayer);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, NFDataList& list, const bool bPlayer, const NFGUID& noSelf);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string& strClassName, NFDataList& list);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string& strClassName, NFDataList& list, const NFGUID& noSelf);
	
	virtual int GetObjectByProperty(const int nSceneID, const int nGroupID, const std::string& strPropertyName, const NFDataList& valueArgArg, NFDataList& list);

	virtual int Random(int nStart, int nEnd);//return [nStart, nEnd)
	virtual float Random();//return [0f, 1f)

    //////////////////////////////////////////////////////////////////////////
    virtual bool LogStack();
    virtual bool LogInfo(const NFGUID ident);
    virtual bool LogSelfInfo(const NFGUID ident);

    //////////////////////////////////////////////////////////////////////////

    virtual bool DoEvent(const NFGUID& self, const std::string& strClassName, CLASS_OBJECT_EVENT eEvent, const NFDataList& valueList);

protected:

    
    virtual bool RegisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb);
    virtual bool RegisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb);
    virtual bool RegisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb);

	virtual bool RegisterClassPropertyEvent(const std::string& strClassName, const PROPERTY_EVENT_FUNCTOR_PTR& cb);
	virtual bool RegisterClassRecordEvent(const std::string& strClassName, const RECORD_EVENT_FUNCTOR_PTR& cb);
protected:

    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);

    void InitRandom();

    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
    int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
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
