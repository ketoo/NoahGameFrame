// -------------------------------------------------------------------------
//    @FileName			:    NFISceneModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFISceneModule
//
// -------------------------------------------------------------------------

#ifndef NFI_CONTAINER_MODULE_H
#define NFI_CONTAINER_MODULE_H

#include <list>
#include <iostream>
#include <algorithm>
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"

// all object in this group
/*
if a object in the group of '0', them it can be see by all object in this scene.
*/
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
        mnGroupIndex = 0;
        mnSceneID = nSceneID;
        mnWidth = 512;
    }

    NFCSceneInfo(int nSceneID, int nWidth)
    {
        mnGroupIndex = 0;
        mnSceneID = nSceneID;
        mnWidth = nWidth;
    }

    virtual ~NFCSceneInfo()
    {
        ClearAll();
    }

    int NewGroupID()
    {
        return ++mnGroupIndex;
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

            }
            else
            {

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
protected:
private:
    int mnGroupIndex;
    int mnSceneID;
    int mnWidth;
};

typedef std::function<int(const NFIDataList&, const NFIDataList&)> OBJECT_ENTER_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<OBJECT_ENTER_EVENT_FUNCTOR> OBJECT_ENTER_EVENT_FUNCTOR_PTR;//ObjectEnterCallBack

typedef std::function<int(const NFIDataList&, const NFIDataList&)> OBJECT_LEAVE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<OBJECT_LEAVE_EVENT_FUNCTOR> OBJECT_LEAVE_EVENT_FUNCTOR_PTR;//ObjectLeaveCallBack

typedef std::function<int(const NFIDataList&, const NFIDataList&)> PROPERTY_ENTER_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<PROPERTY_ENTER_EVENT_FUNCTOR> PROPERTY_ENTER_EVENT_FUNCTOR_PTR;//AddPropertyEnterCallBack

typedef std::function<int(const NFIDataList&, const NFIDataList&)> RECORD_ENTER_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<RECORD_ENTER_EVENT_FUNCTOR> RECORD_ENTER_EVENT_FUNCTOR_PTR;//AddRecordEnterCallBack

typedef std::function<int(const NFIDataList&, const NFIDataList&)> PROPERTY_SINGLE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<PROPERTY_SINGLE_EVENT_FUNCTOR> PROPERTY_SINGLE_EVENT_FUNCTOR_PTR;//AddPropertyEventCallBack

typedef std::function<int(const NFIDataList&, const NFIDataList&)> RECORD_SINGLE_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<RECORD_SINGLE_EVENT_FUNCTOR> RECORD_SINGLE_EVENT_FUNCTOR_PTR;//AddRecordEventCallBack


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
	bool AddObjectEnterCallBack(BaseType* pBase, int (BaseType::*handler)(const NFIDataList&, const NFIDataList&))
	{
		OBJECT_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		OBJECT_ENTER_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_ENTER_EVENT_FUNCTOR(functor));
		return AddObjectEnterCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddObjectLeaveCallBack(BaseType* pBase, int (BaseType::*handler)(const NFIDataList&, const NFIDataList&))
	{
		OBJECT_LEAVE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		OBJECT_LEAVE_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_LEAVE_EVENT_FUNCTOR(functor));
		return AddObjectLeaveCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddPropertyEnterCallBack(BaseType* pBase, int (BaseType::*handler)(const NFIDataList&, const NFGUID&))
	{
		PROPERTY_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		PROPERTY_ENTER_EVENT_FUNCTOR_PTR functorPtr(new PROPERTY_ENTER_EVENT_FUNCTOR(functor));
		return AddPropertyEnterCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddRecordEnterCallBack(BaseType* pBase, int (BaseType::*handler)(const NFIDataList&, const NFGUID&))
	{
		RECORD_ENTER_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		RECORD_ENTER_EVENT_FUNCTOR_PTR functorPtr(new RECORD_ENTER_EVENT_FUNCTOR(functor));
		return AddRecordEnterCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddPropertyEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFIDataList::TData&, const NFIDataList::TData&, const NFIDataList&))
	{
		PROPERTY_SINGLE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		PROPERTY_SINGLE_EVENT_FUNCTOR_ptr functorPtr(new PROPERTY_SINGLE_EVENT_FUNCTOR(functor));
		return AddPropertyEventCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddRecordEventCallBack(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFIDataList::TData&, const NFIDataList::TData&, const NFIDataList&))
	{
		RECORD_SINGLE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		RECORD_SINGLE_EVENT_FUNCTOR_PTR functorPtr(new RECORD_SINGLE_EVENT_FUNCTOR(functor));
		return AddRecordEventCallBack(functorPtr);
	}

protected:
	virtual bool AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR& cb) = 0;
	virtual bool AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR& cb) = 0;
	virtual bool AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR& cb) = 0;
	virtual bool AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR& cb) = 0;

private:
};
#endif
