// -------------------------------------------------------------------------
//    @FileName			:    NFISceneAOIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFISceneAOIModule
//
// -------------------------------------------------------------------------

#ifndef NFI_SCENE_AOI_MODULE_H
#define NFI_SCENE_AOI_MODULE_H

#include <list>
#include <iostream>
#include <algorithm>
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFIRecord.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIModule.h"

// all object in this group
/*
if a object in the group of '0', them it can be see by all object in this scene.
*/
struct SceneSeedResource
{
	std::string strSeedID;
	std::string strConfigID;
	NFVector3 vSeedPos;
	int nWeight;
};

//SceneID,(SeedID,SeedData)
//NFMapEx<int, NFMapEx<std::string, SceneSeedResource>> mtSceneResourceConfig;

class NFCSceneGroupInfo
{
public:
    NFCSceneGroupInfo(int nSceneID, int nGroupID)
    {
        mnGroupID = nGroupID;
    }

    NFCSceneGroupInfo(int nSceneID, int nGroupID, int nWidth)
    {
        mnGroupID = nGroupID;
    }

    virtual ~NFCSceneGroupInfo()
    {
    }

    bool Execute()
    {
        return true;
    }

    NFMapEx<NFGUID, int> mxPlayerList;
    NFMapEx<NFGUID, int> mxOtherList;
    int mnGroupID;
};

// all group in this scene
class NFCSceneInfo
    : public NFMapEx<int, NFCSceneGroupInfo>
{
public:

    NFCSceneInfo(int nSceneID)
    {
        mnGroupIndex = -1;
        mnSceneID = nSceneID;
        mnWidth = 512;
    }

    NFCSceneInfo(int nSceneID, int nWidth)
    {
        mnGroupIndex = -1;
        mnSceneID = nSceneID;
        mnWidth = nWidth;
    }

    virtual ~NFCSceneInfo()
    {
        ClearAll();
    }

    int NewGroupID()
    {
		mnGroupIndex += 1;
		return mnGroupIndex;
    }

    int GetWidth()
    {
        return mnWidth;
    }

    bool AddObjectToGroup(const int nGroupID, const NFGUID& ident, bool bPlayer)
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pInfo = GetElement(nGroupID);
        if (pInfo.get())
        {
            if (bPlayer)
            {
                return pInfo->mxPlayerList.AddElement(ident, NF_SHARE_PTR<int>());
            }
            else
            {
                return pInfo->mxOtherList.AddElement(ident, NF_SHARE_PTR<int>());
            }
        }

        return false;
    }

    bool RemoveObjectFromGroup(const int nGroupID, const NFGUID& ident, bool bPlayer)
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pInfo = GetElement(nGroupID);
        if (pInfo.get())
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

	bool ExistObjectInGroup(const int nGroupID, const NFGUID& ident)
	{
		NF_SHARE_PTR<NFCSceneGroupInfo> pInfo = GetElement(nGroupID);
		if (pInfo)
		{
			return pInfo->mxPlayerList.ExistElement(ident) || pInfo->mxOtherList.ExistElement(ident);
		}

		return false;
	}

    bool Execute()
    {
        NF_SHARE_PTR<NFCSceneGroupInfo> pGroupInfo = First();
        while (pGroupInfo.get())
        {
            pGroupInfo->Execute();

            pGroupInfo = Next();
        }
        return true;
    }

	bool AddSeedObjectInfo(const std::string& strSeedID, const std::string& strConfigID, const NFVector3& vPos, const int nWeight)
	{
		NF_SHARE_PTR<SceneSeedResource> pInfo = mtSceneResourceConfig.GetElement(strSeedID);
		if (!pInfo)
		{
			pInfo = NF_SHARE_PTR<SceneSeedResource>(new SceneSeedResource());
			pInfo->strSeedID = strSeedID;
			pInfo->strConfigID = strConfigID;
			pInfo->vSeedPos = vPos;
			pInfo->nWeight = nWeight;
			return mtSceneResourceConfig.AddElement(strSeedID, pInfo);
		}

		return true;
	}

	bool RemoveSeedObject(const std::string& strSeedID)
	{
		return true;
	}
	
	bool AddReliveInfo(const int nIndex, const NFVector3& vPos)
	{
		return mtSceneRelivePos.AddElement(nIndex, NF_SHARE_PTR<NFVector3>(NF_NEW NFVector3(vPos)));
	}
	NFVector3 GetReliveInfo(const int nIndex)
	{
		NF_SHARE_PTR<NFVector3> vPos = mtSceneRelivePos.GetElement(nIndex);
		if (vPos)
		{
			return *vPos;
		}

		return NFVector3();
	}
    int mnGroupIndex;
    int mnSceneID;
    int mnWidth;
	//seedID, seedInfo
	NFMapEx<std::string, SceneSeedResource > mtSceneResourceConfig;
	NFMapEx<int, NFVector3 > mtSceneRelivePos;
};

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


class NFISceneAOIModule
    : public NFIModule,
  public NFMapEx<int, NFCSceneInfo>
{
public:
    virtual ~NFISceneAOIModule()
    {
        ClearAll();
    }

	template<typename BaseType>
	bool AddObjectEnterCallBack(BaseType* pBase, int (BaseType::*handler)(const NFDataList&, const NFDataList&))
	{
		OBJECT_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		OBJECT_ENTER_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_ENTER_EVENT_FUNCTOR(functor));
		return AddObjectEnterCallBack(functorPtr);
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

	virtual bool RequestEnterScene(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList) = 0;
	virtual bool AddSeedData(const int nSceneID, const std::string& strSeedID, const std::string& strConfigID, const NFVector3& vPos, const int nHeight) = 0;
	virtual bool AddRelivePosition(const int nSceneID, const int nIndex, const NFVector3& vPos) = 0;
	virtual NFVector3 GetRelivePosition(const int nSceneID, const int nIndex) = 0;

	virtual bool CreateSceneNPC(const int nSceneID, const int nGroupID) = 0;
	virtual bool DestroySceneNPC(const int nSceneID, const int nGroupID) = 0;

protected:
	virtual bool AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool AddEnterSceneConditionCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool AddBeforeEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddAfterEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddSwapSceneEventCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddBeforeLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddAfterLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb) = 0;

private:
};
#endif
