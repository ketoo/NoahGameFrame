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
#include "../NFGridModule/NFCGridModule.h"
#include "NFComm/NFCore/NFIdentID.h"
#include "NFComm/NFCore/NFCValueList.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFIObject.h"

// all object in this group
/*
if a object in the group of '0', them it can be see by all object in this scene.
*/
class NFCContainerGroupInfo
    : public NFList<NFIDENTID>
{
public:
    NFCContainerGroupInfo(int nSceneID, int nWidth)
    {
        //m_pGridModule = new NFCGridModule(nSceneID, nWidth);
    }

    virtual ~NFCContainerGroupInfo()
    {
        //if (m_pGridModule)
        //{
        //    delete m_pGridModule;
        //    m_pGridModule = NULL;
        //}
    }

    NFIGridModule* GetGridModule()
    {
        return NULL;
        //return m_pGridModule;
    }

    bool Execute(const float fLasFrametime, const float fStartedTime)
    {
        return true;
    }

private:
    //NFIGridModule* m_pGridModule;


};

// all group in this scene
class NFCContainerInfo
    : public NFMap<int, NFCContainerGroupInfo>
{
public:
    NFCContainerInfo(int nSceneID, int nWidth)
    {
        mnGroupIndex = 0;
        mnSceneID = nSceneID;
        mnWidth = nWidth;
    }

    int NewGroupID()
    {
        return ++mnGroupIndex;
    }

    int GetWidth()
    {
        return mnWidth;
    }

    void SetObjectSelf(const NFIDENTID& ident)
    {
        mIdent = ident;
    }

    NFIDENTID GetObjectSelf()
    {
        return mIdent;
    }

    bool AddObjectToGroup(const int nGroupID, const NFIDENTID& ident)
    {
        NFCContainerGroupInfo* pInfo = GetElement(nGroupID);
        if (pInfo)
        {
            return pInfo->Add(ident);
        }

        return false;
    }

    bool RemoveObjectFromGroup(const int nGroupID, const NFIDENTID& ident)
    {
        NFCContainerGroupInfo* pInfo = GetElement(nGroupID);
        if (pInfo)
        {
            return pInfo->Remove(ident);
        }

        return false;
    }

    bool Execute(const float fLasFrametime, const float fStartedTime)
    {
        NFCContainerGroupInfo* pGroupInfo = First();
        while (pGroupInfo)
        {
            pGroupInfo->Execute(fLasFrametime, fStartedTime);

            pGroupInfo = Next();
        }
        return true;
    }
protected:
private:
    NFIDENTID mIdent;
    int mnGroupIndex;
    int mnSceneID;
    int mnWidth;
};

class NFIContainerModule
    : public NFMap<int, NFCContainerInfo>
{
public:

protected:
private:
};
#endif
