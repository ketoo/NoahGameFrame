/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#ifndef NFI_SCENE_AOI_MODULE_H
#define NFI_SCENE_AOI_MODULE_H

#include <list>
#include <iostream>
#include <functional>
#include <algorithm>
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFIRecord.h"
#include "NFComm/NFCore/NFIProperty.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIModule.h"

typedef std::function<int(const NFDataList&, const NFDataList&)> OBJECT_ENTER_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<OBJECT_ENTER_EVENT_FUNCTOR> OBJECT_ENTER_EVENT_FUNCTOR_PTR;//ObjectEnterCallBack

typedef std::function<int(const NFDataList&, const NFDataList&)> OBJECT_LEAVE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<OBJECT_LEAVE_EVENT_FUNCTOR> OBJECT_LEAVE_EVENT_FUNCTOR_PTR;//ObjectLeaveCallBack

typedef std::function<int(const NFDataList&, const NFGUID&)> PROPERTY_ENTER_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<PROPERTY_ENTER_EVENT_FUNCTOR> PROPERTY_ENTER_EVENT_FUNCTOR_PTR;//AddPropertyEnterCallBack

typedef std::function<int(const NFDataList&, const NFGUID&)> RECORD_ENTER_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<RECORD_ENTER_EVENT_FUNCTOR> RECORD_ENTER_EVENT_FUNCTOR_PTR;//AddRecordEnterCallBack

typedef std::function<int(const NFGUID&, const std::string&, const NFData&, const NFData&, const NFDataList&)> PROPERTY_SINGLE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<PROPERTY_SINGLE_EVENT_FUNCTOR> PROPERTY_SINGLE_EVENT_FUNCTOR_PTR;//AddPropertyEventCallBack

typedef std::function<int(const NFGUID&, const std::string&, const RECORD_EVENT_DATA&, const NFData&, const NFData&, const NFDataList&)> RECORD_SINGLE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<RECORD_SINGLE_EVENT_FUNCTOR> RECORD_SINGLE_EVENT_FUNCTOR_PTR;//AddRecordEventCallBack

typedef std::function<int(const NFGUID&, const int, const int, const int, const NFDataList&)> SCENE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<SCENE_EVENT_FUNCTOR> SCENE_EVENT_FUNCTOR_PTR;

class NFSceneInfo;
class NFSceneGroupInfo;

class NFISceneModule
    : public NFIModule,
  public NFMapEx<int, NFSceneInfo>
{
public:
    virtual ~NFISceneModule()
    {
        ClearAll();
    }

	/////////////these interfaces below are for scene & group//////////////////
	template<typename BaseType>
	bool AddGroupPropertyCallBack(const std::string& strPropertyName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFData&, const NFData&))
	{
		PROPERTY_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
		return AddGroupPropertyCallBack(strPropertyName, functorPtr);
	}

	template<typename BaseType>
	bool AddGroupRecordCallBack(const std::string& strRecordName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&))
	{
		RECORD_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
		return AddGroupRecordCallBack(strRecordName, functorPtr);
	}

	template<typename BaseType>
	bool AddGroupPropertyCommCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFData&, const NFData&))
	{
		PROPERTY_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
		return AddGroupPropertyCommCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddGroupRecordCommCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFData&, const NFData&))
	{
		RECORD_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
		return AddGroupRecordCommCallBack(functorPtr);
	}

	/////////////these interfaces below are for player//////////////////

	template<typename BaseType>
	bool AddObjectEnterCallBack(BaseType* pBase, int (BaseType::*handler)(const NFDataList&, const NFDataList&))
	{
		OBJECT_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		OBJECT_ENTER_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_ENTER_EVENT_FUNCTOR(functor));
		return AddObjectEnterCallBack(functorPtr);
	}
	template<typename BaseType>
	bool AddObjectDataFinishedCallBack(BaseType* pBase, int (BaseType::*handler)(const NFDataList&, const NFDataList&))
	{
		OBJECT_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		OBJECT_ENTER_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_ENTER_EVENT_FUNCTOR(functor));
		return AddObjectDataFinishedCallBack(functorPtr);
	}
	template<typename BaseType>
	bool AddObjectLeaveCallBack(BaseType* pBase, int (BaseType::*handler)(const NFDataList&, const NFDataList&))
	{
		OBJECT_LEAVE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		OBJECT_LEAVE_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_LEAVE_EVENT_FUNCTOR(functor));
		return AddObjectLeaveCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddPropertyEnterCallBack(BaseType* pBase, int (BaseType::*handler)(const NFDataList&, const NFGUID&))
	{
		PROPERTY_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		PROPERTY_ENTER_EVENT_FUNCTOR_PTR functorPtr(new PROPERTY_ENTER_EVENT_FUNCTOR(functor));
		return AddPropertyEnterCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddRecordEnterCallBack(BaseType* pBase, int (BaseType::*handler)(const NFDataList&, const NFGUID&))
	{
		RECORD_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		RECORD_ENTER_EVENT_FUNCTOR_PTR functorPtr(new RECORD_ENTER_EVENT_FUNCTOR(functor));
		return AddRecordEnterCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddPropertyEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFData&, const NFData&, const NFDataList&))
	{
		PROPERTY_SINGLE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		PROPERTY_SINGLE_EVENT_FUNCTOR_PTR functorPtr(new PROPERTY_SINGLE_EVENT_FUNCTOR(functor));
		return AddPropertyEventCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddRecordEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const RECORD_EVENT_DATA&, const NFData&, const NFData&, const NFDataList&))
	{
		RECORD_SINGLE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
		RECORD_SINGLE_EVENT_FUNCTOR_PTR functorPtr(new RECORD_SINGLE_EVENT_FUNCTOR(functor));
		return AddRecordEventCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddEnterSceneConditionCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddEnterSceneConditionCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddBeforeEnterSceneGroupCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddBeforeEnterSceneGroupCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddAfterEnterSceneGroupCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddAfterEnterSceneGroupCallBack(functorPtr);
	}


	template<typename BaseType>
	bool AddSwapSceneEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddSwapSceneEventCallBack(functorPtr);
	}

	virtual bool RemoveSwapSceneEventCallBack() = 0;

	template<typename BaseType>
	bool AddBeforeLeaveSceneGroupCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddBeforeLeaveSceneGroupCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddAfterLeaveSceneGroupCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddAfterLeaveSceneGroupCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddSceneGroupCreatedCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddSceneGroupCreatedCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddSceneGroupDestroyedCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const int, const int, const NFDataList&))
	{
		SCENE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		SCENE_EVENT_FUNCTOR_PTR functorPtr(new SCENE_EVENT_FUNCTOR(functor));
		return AddSceneGroupDestroyedCallBack(functorPtr);
	}


	virtual int RequestGroupScene(const int nSceneID) = 0;
	virtual bool RequestEnterScene(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFVector3& pos, const NFDataList& argList) = 0;
	virtual bool ReleaseGroupScene(const int nSceneID, const int nGroupID) = 0;
	virtual bool LeaveSceneGroup(const NFGUID& self) = 0;

	virtual bool AddSeedData(const int nSceneID, const std::string& strSeedID, const std::string& strConfigID, const NFVector3& vPos, const int nHeight) = 0;
	virtual bool AddRelivePosition(const int nSceneID, const int nIndex, const NFVector3& vPos) = 0;
	virtual const NFVector3& GetRelivePosition(const int nSceneID, const int nIndex, const bool bRoll = true) = 0;
	virtual bool AddTagPosition(const int nSceneID, const int nIndex, const NFVector3& vPos) = 0;
	virtual const NFVector3& GetTagPosition(const int nSceneID, const int nIndex, const bool bRoll = true) = 0;

	virtual bool CreateSceneNPC(const int nSceneID, const int nGroupID) = 0;
	virtual bool CreateSceneNPC(const int nSceneID, const int nGroupID, const NFDataList& argList) = 0;
	virtual bool DestroySceneNPC(const int nSceneID, const int nGroupID) = 0;

	/////////////the interfaces below are for scene & group/////////////////////////////
	virtual bool SetPropertyInt(const int scene, const int group, const std::string& strPropertyName, const NFINT64 nValue) = 0;
	virtual bool SetPropertyFloat(const int scene, const int group, const std::string& strPropertyName, const double dValue) = 0;
	virtual bool SetPropertyString(const int scene, const int group, const std::string& strPropertyName, const std::string& strValue) = 0;
	virtual bool SetPropertyObject(const int scene, const int group, const std::string& strPropertyName, const NFGUID& objectValue) = 0;
	virtual bool SetPropertyVector2(const int scene, const int group, const std::string& strPropertyName, const NFVector2& value) = 0;
	virtual bool SetPropertyVector3(const int scene, const int group, const std::string& strPropertyName, const NFVector3& value) = 0;

	virtual NFINT64 GetPropertyInt(const int scene, const int group, const std::string& strPropertyName) = 0;
	virtual int GetPropertyInt32(const int scene, const int group, const std::string& strPropertyName) = 0;	//equal to (int)GetPropertyInt(...), to remove C4244 warning
	virtual double GetPropertyFloat(const int scene, const int group, const std::string& strPropertyName) = 0;
	virtual const std::string& GetPropertyString(const int scene, const int group, const std::string& strPropertyName) = 0;
	virtual const NFGUID& GetPropertyObject(const int scene, const int group, const std::string& strPropertyName) = 0;
	virtual const NFVector2& GetPropertyVector2(const int scene, const int group, const std::string& strPropertyName) = 0;
	virtual const NFVector3& GetPropertyVector3(const int scene, const int group, const std::string& strPropertyName) = 0;

	virtual NF_SHARE_PTR<NFIPropertyManager> FindPropertyManager(const int scene, const int group) = 0;
	virtual NF_SHARE_PTR<NFIRecordManager> FindRecordManager(const int scene, const int group) = 0;
	virtual NF_SHARE_PTR<NFIRecord> FindRecord(const int scene, const int group, const std::string& strRecordName) = 0;
	virtual bool ClearRecord(const int scene, const int group, const std::string& strRecordName) = 0;

	virtual bool SetRecordInt(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue) = 0;
	virtual bool SetRecordFloat(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue) = 0;
	virtual bool SetRecordString(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;
	virtual bool SetRecordObject(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue) = 0;
	virtual bool SetRecordVector2(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& value) = 0;
	virtual bool SetRecordVector3(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& value) = 0;

	virtual bool SetRecordInt(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value) = 0;
	virtual bool SetRecordFloat(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value) = 0;
	virtual bool SetRecordString(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value) = 0;
	virtual bool SetRecordObject(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value) = 0;
	virtual bool SetRecordVector2(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value) = 0;
	virtual bool SetRecordVector3(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value) = 0;

	virtual NFINT64 GetRecordInt(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual double GetRecordFloat(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual const std::string& GetRecordString(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual const NFGUID& GetRecordObject(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual const NFVector2& GetRecordVector2(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual const NFVector3& GetRecordVector3(const int scene, const int group, const std::string& strRecordName, const int nRow, const int nCol) = 0;

	virtual NFINT64 GetRecordInt(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual double GetRecordFloat(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual const std::string& GetRecordString(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual const NFGUID& GetRecordObject(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual const NFVector2& GetRecordVector2(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual const NFVector3& GetRecordVector3(const int scene, const int group, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;

	////////////////////////////////////////////////////////////////
protected:
	//for scne && group
	virtual bool AddGroupRecordCallBack(const std::string& strName, const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddGroupPropertyCallBack(const std::string& strName, const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddGroupRecordCommCallBack(const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddGroupPropertyCommCallBack(const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;

	//for players
	virtual bool AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddObjectDataFinishedCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool AddEnterSceneConditionCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool AddBeforeEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddAfterEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddSwapSceneEventCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddBeforeLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddAfterLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool AddSceneGroupCreatedCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddSceneGroupDestroyedCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;

private:
};
#endif
