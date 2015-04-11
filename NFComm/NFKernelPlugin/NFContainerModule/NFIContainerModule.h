// -------------------------------------------------------------------------
//    @FileName      :    NFISceneModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFISceneModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_CONTAINER_MODULE_H_
#define _NFI_CONTAINER_MODULE_H_

#include <list>
#include <iostream>
#include <algorithm>
#include "NFComm/NFKernelPlugin/NFGridModule/NFCGridModule.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFIdentID.h"

// all object in this group
/*
if a object in the group of '0', them it can be see by all object in this scene.
*/
class NFCContainerGroupInfo
    //: public NFList<NFIDENTID>
{
public:
    NFCContainerGroupInfo(int nSceneID, int nGroupID, int nWidth)
    {
        mnGroupID = nGroupID;
        //m_pGridModule = NF_SHARE_PTR<NFIGridModule>(NF_NEW NFCGridModule(nSceneID, nWidth));
    }

    virtual ~NFCContainerGroupInfo()
    {
    }

    //NF_SHARE_PTR<NFIGridModule> GetGridModule()
    //{
    //    return m_pGridModule;
    //}

    bool Execute(const float fLasFrametime, const float fStartedTime)
    {
        return true;
    }

    NFMapEx<NFIDENTID, int> mxPlayerList;
    NFMapEx<NFIDENTID, int> mxOtherList;
    int mnGroupID;
//private:
//    NF_SHARE_PTR<NFIGridModule> m_pGridModule;
};

// all group in this scene
class NFCContainerInfo
    : public NFMapEx<int, NFCContainerGroupInfo>
{
public:
    NFCContainerInfo(int nSceneID, int nWidth)
    {
        mnGroupIndex = 0;
        mnSceneID = nSceneID;
        mnWidth = nWidth;
    }

    virtual ~NFCContainerInfo()
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

    //void SetObjectSelf(const NFIDENTID& ident)
    //{
    //    mIdent = ident;
    //}

    //NFIDENTID GetObjectSelf()
    //{
    //    return mIdent;
    //}

    bool AddObjectToGroup(const int nGroupID, const NFIDENTID& ident, bool bPlayer)
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pInfo = GetElement(nGroupID);
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

    bool RemoveObjectFromGroup(const int nGroupID, const NFIDENTID& ident, bool bPlayer)
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pInfo = GetElement(nGroupID);
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

    bool Execute(const float fLasFrametime, const float fStartedTime)
    {
        NF_SHARE_PTR<NFCContainerGroupInfo> pGroupInfo = First();
        while (pGroupInfo.get())
        {
            pGroupInfo->Execute(fLasFrametime, fStartedTime);

            pGroupInfo = Next();
        }
        return true;
    }
protected:
private:
    //NFIDENTID mIdent;
    int mnGroupIndex;
    int mnSceneID;
    int mnWidth;
};

class NFIContainerModule
    : public NFMapEx<int, NFCContainerInfo>
{
public:
    virtual ~NFIContainerModule()
    {
        ClearAll();
    }

protected:
private:
};
#endif
