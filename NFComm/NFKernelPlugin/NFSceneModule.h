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

#ifndef NF_SCENE_MODULE_H
#define NF_SCENE_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFICellModule.h"


// all object in this group
/*
if a object in the group of '0', them it can be see by all object in this scene.
*/
struct SceneSeedResource
{
	std::string seedID;
	std::string configID;
	NFVector3 vSeedPos;
	int nWeight;
};

struct PosSyncUnit
{
	NFGUID 	mover;
	NFVector3 pos;
	NFVector3 direction;
	NFVector3 orientation;
	int32_t status;
	int32_t type;
};

class NFSceneGroupInfo
{
public:
	NFSceneGroupInfo(const int sceneID, const int groupID, NF_SHARE_PTR<NFIPropertyManager> xPropertyManager, NF_SHARE_PTR<NFIRecordManager> xRecordManager)
	{
		this->groupID = groupID;
		this->mxPropertyManager = xPropertyManager;
		this->mxRecordManager = xRecordManager;
	}

	virtual ~NFSceneGroupInfo()
	{
	}

	bool Execute()
	{
		return true;
	}

	NFMapEx<NFGUID, int> mxPlayerList;
	NFMapEx<NFGUID, int> mxOtherList;
	int groupID;

	int sequence = 0;
	std::map<NFGUID, PosSyncUnit> mPlayerPosition;
	NF_SHARE_PTR<NFIPropertyManager> mxPropertyManager;
	NF_SHARE_PTR<NFIRecordManager> mxRecordManager;
};

// all group in this scene
class NFSceneInfo
	: public NFMapEx<int, NFSceneGroupInfo>
{
public:

	NFSceneInfo(const int sceneID)
	{
		this->groupIndex = -1;
		this->sceneID = sceneID;
		this->width = 512;
	}

	NFSceneInfo(const int sceneID, const int nWidth)
	{
		this->groupIndex = -1;
		this->sceneID = sceneID;
		this->width = nWidth;
	}

	virtual ~NFSceneInfo()
	{
		ClearAll();
	}

	int NewGroupID()
	{
		groupIndex += 1;
		return groupIndex;
	}

	int GetWidth()
	{
		return width;
	}

	bool AddObjectToGroup(const int groupID, const NFGUID& ident, bool bPlayer)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pInfo = GetElement(groupID);
		if (pInfo.get())
		{
			if (bPlayer)
			{
				return pInfo->mxPlayerList.AddElement(ident, NF_SHARE_PTR<int>(new int(0)));
			}
			else
			{
				return pInfo->mxOtherList.AddElement(ident, NF_SHARE_PTR<int>(new int(0)));
			}
		}

		return false;
	}

	bool RemoveObjectFromGroup(const int groupID, const NFGUID& ident, bool bPlayer)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pInfo = GetElement(groupID);
		if (pInfo)
		{
			if (bPlayer)
			{
				return pInfo->mxPlayerList.RemoveElement(ident);
			}
			else
			{
				return pInfo->mxOtherList.RemoveElement(ident);
			}
		}

		return false;
	}

	bool ExistObjectInGroup(const int groupID, const NFGUID& ident)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pInfo = GetElement(groupID);
		if (pInfo)
		{
			return pInfo->mxPlayerList.ExistElement(ident) || pInfo->mxOtherList.ExistElement(ident);
		}

		return false;
	}

	bool Execute()
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = First();
		while (pGroupInfo.get())
		{
			pGroupInfo->Execute();

			pGroupInfo = Next();
		}
		return true;
	}

	bool AddSeedObjectInfo(const std::string& seedID, const std::string& configID, const NFVector3& vPos, const int nWeight)
	{
		NF_SHARE_PTR<SceneSeedResource> pInfo = mtSceneResourceConfig.GetElement(seedID);
		if (!pInfo)
		{
			pInfo = NF_SHARE_PTR<SceneSeedResource>(new SceneSeedResource());
			pInfo->seedID = seedID;
			pInfo->configID = configID;
			pInfo->vSeedPos = vPos;
			pInfo->nWeight = nWeight;
			return mtSceneResourceConfig.AddElement(seedID, pInfo);
		}

		return true;
	}

	NF_SHARE_PTR<SceneSeedResource> GetSeedObjectInfo(const std::string& seedID)
	{
		return mtSceneResourceConfig.GetElement(seedID);
	}

	bool RemoveSeedObject(const std::string& seedID)
	{
		return true;
	}

	bool AddReliveInfo(const int nIndex, const NFVector3& vPos)
	{
		return mtSceneRelivePos.AddElement(nIndex, NF_SHARE_PTR<NFVector3>(NF_NEW NFVector3(vPos)));
	}

	const NFVector3& GetReliveInfo(const int nIndex)
	{
		if (mtSceneRelivePos.Count() > 0)
		{
			if (nIndex < 0)
			{
				int rd = rand() % mtSceneRelivePos.Count();
				return *(mtSceneRelivePos.GetElement(rd));
			}

			if (nIndex < mtSceneRelivePos.Count())
			{
				NF_SHARE_PTR<NFVector3> vPos = mtSceneRelivePos.GetElement(nIndex);
				if (vPos)
				{
					return *vPos;
				}
			}
		}

		return NFVector3::Zero();
	}

	bool AddTagInfo(const int nIndex, const NFVector3& vPos)
	{
		return mtTagPos.AddElement(nIndex, NF_SHARE_PTR<NFVector3>(NF_NEW NFVector3(vPos)));
	}

	const NFVector3& GetTagInfo(const int nIndex)
	{
		if (mtTagPos.Count() > 0)
		{
			if (nIndex < 0)
			{
				int rd = rand() % mtTagPos.Count();
				return *(mtTagPos.GetElement(rd));
			}

			if (nIndex < mtTagPos.Count())
			{
				NF_SHARE_PTR<NFVector3> vPos = mtTagPos.GetElement(nIndex);
				if (vPos)
				{
					return *vPos;
				}
			}
		}

		return NFVector3::Zero();
	}

	int groupIndex;
	int sceneID;
	int width;
	//seedID, seedInfo
	NFMapEx<std::string, SceneSeedResource > mtSceneResourceConfig;
	NFMapEx<int, NFVector3 > mtSceneRelivePos;
	NFMapEx<int, NFVector3 > mtTagPos;
};


class NFSceneModule
    : public NFISceneModule
{
public:
	NFSceneModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~NFSceneModule()
    {
    }

    virtual bool Init();
    virtual bool AfterInit();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Execute();

	virtual int RequestGroupScene(const int sceneID);
	virtual bool RequestEnterScene(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFVector3& pos, const NFDataList& argList);
	virtual bool ReleaseGroupScene(const int sceneID, const int groupID);
	virtual bool LeaveSceneGroup(const NFGUID& self);
	virtual const std::vector<int>& GetGroups(const int sceneID);

	virtual bool AddSeedData(const int sceneID, const std::string& seedID, const std::string& configID, const NFVector3& vPos, const int nWeight);
	virtual const NFVector3& GetSeedPos(const int sceneID, const std::string& seedID);
	virtual const int GetSeedPWeight(const int sceneID, const std::string& seedID);

	virtual bool AddRelivePosition(const int sceneID, const int nIndex, const NFVector3& vPos);
	virtual const NFVector3& GetRelivePosition(const int sceneID, const int nIndex = -1);

	virtual bool AddTagPosition(const int sceneID, const int nIndex, const NFVector3& vPos);
	virtual const NFVector3& GetTagPosition(const int sceneID, const int nIndex= -1);

	virtual bool CreateSceneNPC(const int sceneID, const int groupID);
	virtual bool CreateSceneNPC(const int sceneID, const int groupID, const NFDataList& argList);
	virtual bool DestroySceneNPC(const int sceneID, const int groupID);

	virtual bool RemoveSwapSceneEventCallBack();

	//////////////////////////////////////////////////////////////////////////
    virtual bool SetPropertyInt(const int scene, const int group, const std::string& propertyName, const NFINT64 nValue);
    virtual bool SetPropertyFloat(const int scene, const int group, const std::string& propertyName, const double dValue);
    virtual bool SetPropertyString(const int scene, const int group, const std::string& propertyName, const std::string& value);
    virtual bool SetPropertyObject(const int scene, const int group, const std::string& propertyName, const NFGUID& objectValue);
	virtual bool SetPropertyVector2(const int scene, const int group, const std::string& propertyName, const NFVector2& value);
	virtual bool SetPropertyVector3(const int scene, const int group, const std::string& propertyName, const NFVector3& value);

    virtual NFINT64 GetPropertyInt(const int scene, const int group, const std::string& propertyName);
	virtual int GetPropertyInt32(const int scene, const int group, const std::string& propertyName);	//equal to (int)GetPropertyInt(...), to remove C4244 warning
    virtual double GetPropertyFloat(const int scene, const int group, const std::string& propertyName);
    virtual const std::string& GetPropertyString(const int scene, const int group, const std::string& propertyName);
    virtual const NFGUID& GetPropertyObject(const int scene, const int group, const std::string& propertyName);
	virtual const NFVector2& GetPropertyVector2(const int scene, const int group, const std::string& propertyName);
	virtual const NFVector3& GetPropertyVector3(const int scene, const int group, const std::string& propertyName);

    //////////////////////////////////////////////////////////////////////////
	virtual NF_SHARE_PTR<NFIPropertyManager> FindPropertyManager(const int scene, const int group);
	virtual NF_SHARE_PTR<NFIRecordManager> FindRecordManager(const int scene, const int group);
    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const int scene, const int group, const std::string& recordName);
    virtual bool ClearRecord(const int scene, const int group, const std::string& recordName);

    virtual bool SetRecordInt(const int scene, const int group, const std::string& recordName, const int row, const int col, const NFINT64 nValue);
    virtual bool SetRecordFloat(const int scene, const int group, const std::string& recordName, const int row, const int col, const double dwValue);
    virtual bool SetRecordString(const int scene, const int group, const std::string& recordName, const int row, const int col, const std::string& value);
    virtual bool SetRecordObject(const int scene, const int group, const std::string& recordName, const int row, const int col, const NFGUID& objectValue);
	virtual bool SetRecordVector2(const int scene, const int group, const std::string& recordName, const int row, const int col, const NFVector2& value);
	virtual bool SetRecordVector3(const int scene, const int group, const std::string& recordName, const int row, const int col, const NFVector3& value);

    virtual bool SetRecordInt(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value);
    virtual bool SetRecordFloat(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag, const double value);
    virtual bool SetRecordString(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag, const std::string& value);
    virtual bool SetRecordObject(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value);
	virtual bool SetRecordVector2(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value);
	virtual bool SetRecordVector3(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value);

    virtual NFINT64 GetRecordInt(const int scene, const int group, const std::string& recordName, const int row, const int col);
    virtual double GetRecordFloat(const int scene, const int group, const std::string& recordName, const int row, const int col);
    virtual const std::string& GetRecordString(const int scene, const int group, const std::string& recordName, const int row, const int col);
    virtual const NFGUID& GetRecordObject(const int scene, const int group, const std::string& recordName, const int row, const int col);
	virtual const NFVector2& GetRecordVector2(const int scene, const int group, const std::string& recordName, const int row, const int col);
	virtual const NFVector3& GetRecordVector3(const int scene, const int group, const std::string& recordName, const int row, const int col);

    virtual NFINT64 GetRecordInt(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag);
    virtual double GetRecordFloat(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag);
    virtual const std::string& GetRecordString(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag);
    virtual const NFGUID& GetRecordObject(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag);
	virtual const NFVector2& GetRecordVector2(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag);
	virtual const NFVector3& GetRecordVector3(const int scene, const int group, const std::string& recordName, const int row, const std::string& colTag);

    ////////////////////////////////////////////////////////////////
protected:
	//for scene && group
	virtual bool AddGroupPropertyCallBack(const std::string& name, const PROPERTY_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddGroupRecordCallBack(const std::string& name, const RECORD_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddGroupPropertyCommCallBack(const PROPERTY_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddGroupRecordCommCallBack(const RECORD_EVENT_FUNCTOR_PTR& cb);

	//for players
	virtual bool AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddObjectDataFinishedCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR& cb);

	virtual bool AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR& cb);

	virtual bool AddEnterSceneConditionCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);

	virtual bool AddBeforeEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddAfterEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddAfterEnterAndReadySceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddSwapSceneEventCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddBeforeLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddAfterLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);

	virtual bool AddSceneGroupCreatedCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddSceneGroupDestroyedCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
protected:
	bool SwitchScene(const NFGUID& self, const int nTargetSceneID, const int nTargetGroupID, const int type, const NFVector3 v, const float fOrient, const NFDataList& arg);

protected:
	//for scene && group
	int OnScenePropertyCommonEvent(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar, const NFINT64 reason);
	int OnSceneRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);


	//for players
	int OnPropertyCommonEvent(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar, const NFINT64 reason);
	int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);
	int OnClassCommonEvent(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var);

	int OnPlayerGroupEvent(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar);
	int OnPlayerSceneEvent(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar);
	
	int GetBroadCastObject(const NFGUID& self, const std::string& propertyName, const bool bTable, NFDataList& valueObject);

	int EnterSceneCondition(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);

	int BeforeLeaveSceneGroup(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);
	int AfterLeaveSceneGroup(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);
	int OnSwapSceneEvent(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);
	int BeforeEnterSceneGroup(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);
	int AfterEnterSceneGroup(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);
	int AfterEnterAndReadySceneGroup(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);

	int SceneGroupCreatedEvent(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);
	int SceneGroupDestroyedEvent(const NFGUID& self, const int sceneID, const int groupID, const int type, const NFDataList& argList);


protected:
	////////////////interface for broadcast event//////////////////////////////////


	//broad the data of argvar to self
	int OnObjectListEnter(const NFDataList& self, const NFDataList& argVar);
	int OnObjectListEnterFinished(const NFDataList& self, const NFDataList& argVar);

	int OnObjectListLeave(const NFDataList& self, const NFDataList& argVar);

	//broad the data of self to argvar 
	int OnPropertyEnter(const NFDataList& argVar, const NFGUID& self);
	int OnRecordEnter(const NFDataList& argVar, const NFGUID& self);

	int OnPropertyEvent(const NFGUID& self, const std::string& propertyName, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar, const NFINT64 reason);
	int OnRecordEvent(const NFGUID& self, const std::string& recordName, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar);
	
	////////////////interface for broadcast event///////////////////////////////////
	int OnMoveCellEvent(const NFGUID& self, const int& scene, const int& group, const NFGUID& fromCell, const NFGUID& toCell);

private:

	//for scene & group
	std::list<PROPERTY_EVENT_FUNCTOR_PTR> mtGroupPropertyCommCallBackList;
	std::list<RECORD_EVENT_FUNCTOR_PTR> mtGroupRecordCallCommBackList;
	std::map<std::string, std::list<PROPERTY_EVENT_FUNCTOR_PTR>> mtGroupPropertyCallBackList;
	std::map<std::string, std::list<RECORD_EVENT_FUNCTOR_PTR>> mtGroupRecordCallBackList;

	//for players & objects
	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR> mvObjectEnterCallback;
	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR> mvObjectDataFinishedCallBack;
	std::vector<OBJECT_LEAVE_EVENT_FUNCTOR_PTR> mvObjectLeaveCallback;

	std::vector<PROPERTY_ENTER_EVENT_FUNCTOR_PTR> mvPropertyEnterCallback;
	std::vector<RECORD_ENTER_EVENT_FUNCTOR_PTR> mvRecordEnterCallback;

	std::vector<PROPERTY_SINGLE_EVENT_FUNCTOR_PTR> mvPropertySingleCallback;
	std::vector<RECORD_SINGLE_EVENT_FUNCTOR_PTR> mvRecordSingleCallback;

	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvEnterSceneConditionCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvBeforeEnterSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvAfterEnterSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvAfterEnterAndReadySceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvOnSwapSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvBeforeLeaveSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvAfterLeaveSceneCallback;

	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvSceneGroupCreatedCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mvSceneGroupDestroyedCallback;

private:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
	NFIEventModule* m_pEventModule;
	NFICellModule* m_pCellModule;
};

#endif