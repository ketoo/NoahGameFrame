// -------------------------------------------------------------------------
//    @FileName      :    NFISceneModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFISceneModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_CONTAINER_MODULE_H
#define _NFI_CONTAINER_MODULE_H

#include <list>
#include <iostream>
#include <algorithm>
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"

// all object in this group
/*
if a object in the group of '0', them it can be see by all object in this scene.
*/
class NFCSceneGroupInfo
    //: public NFList<NFGUID>
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
                return pInfo->mxPlayerList.AddElement(ident, NF_SHARE_PTR<int>()); // TODO:Map.second为空，使用的时候千万注意
            }
            else
            {
                return pInfo->mxOtherList.AddElement(ident, NF_SHARE_PTR<int>()); // TODO:Map.second为空，使用的时候千万注意
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

class NFISceneModule
    : public NFILogicModule,
	public NFMapEx<int, NFCSceneInfo>
{
public:
    virtual ~NFISceneModule()
    {
        ClearAll();
    }

protected:
private:
};
#endif
