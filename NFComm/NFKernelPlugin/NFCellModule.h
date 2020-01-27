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


#ifndef NF_CELL_MODULE_H
#define NF_CELL_MODULE_H

#include <iostream>
#include <assert.h>
#include <map>
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFICellModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

class NFSceneCellInfo
    : public NFList<NFGUID>
{
public:
	NFSceneCellInfo(const NFSceneCellInfo& cell)
	{
		mnSceneID = cell.mnSceneID;
		mnGroupID = cell.mnGroupID;
		mCellID = cell.mCellID;

		for (int i = ECELL_TOP; i < ECELL_DIRECTION_MAXCOUNT; i++)
		{
			mAroundCell[i] = NULL;
		}
	}

    NFSceneCellInfo(const int& sceneID, const int& groupID, const NFGUID& cellID)
    {
		mnSceneID = sceneID;
		mnGroupID = groupID;
		mCellID = cellID;

        for (int i = ECELL_TOP; i < ECELL_DIRECTION_MAXCOUNT; i++)
        {
            mAroundCell[i] = NULL;
        }
    }

    virtual ~NFSceneCellInfo()
    {
        // TODO
    }

    void Init(NF_SHARE_PTR<NFSceneCellInfo>* pGridArray)
    {
        for (int i = ECELL_TOP; i < ECELL_DIRECTION_MAXCOUNT; i++)
        {
            mAroundCell[i] = pGridArray[i];
        }
    }

    void Finale()
    {
    }

	NF_SHARE_PTR<NFSceneCellInfo> GetConnectCell(ECELL_DIRECTION eDirection)
    {
        return mAroundCell[eDirection];
    }

	const int GetSceneID()
	{
		return mnSceneID;
	}

	const int GetGroupID()
	{
		return mnGroupID;
	}
	const NFGUID GetID()
	{
		return mCellID;
	}
	const bool Exist(const NFGUID& id)
	{
		return this->Find(id);
	}

protected:
private:
    NF_SHARE_PTR<NFSceneCellInfo> mAroundCell[ECELL_DIRECTION_MAXCOUNT];
	NFGUID mCellID;
	int mnSceneID;
	int mnGroupID;
};

class NFCellModule
    : public NFICellModule
{
public:
	NFCellModule(NFIPluginManager* p);

    virtual ~NFCellModule();

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

	virtual const bool CreateGroupCell(const int& sceneID, const int& groupID);
	virtual const bool DestroyGroupCell(const int& sceneID, const int& groupID);

    // the event that a object are moving
    virtual const NFGUID OnObjectMove(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell, const NFGUID& toCell);

    // the event that a object has entried
    virtual const NFGUID OnObjectEntry(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& toCell);

    // the event that a object has leaved
    virtual const NFGUID OnObjectLeave(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell);

	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, ECELL_AROUND eAround = ECELL_AROUND_9);
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, const NFGUID& noSelf, ECELL_AROUND eAround = ECELL_AROUND_9);
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, const bool bPlayer, ECELL_AROUND eAround = ECELL_AROUND_9);
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, NFDataList& list, const bool bPlayer, const NFGUID& noSelf, ECELL_AROUND eAround = ECELL_AROUND_9);
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, const std::string& strClassName, NFDataList& list, ECELL_AROUND eAround = ECELL_AROUND_9);
	virtual bool GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3& pos, const std::string& strClassName, NFDataList& list, const NFGUID& noSelf, ECELL_AROUND eAround = ECELL_AROUND_9);
    
	//////////////////////////////////////////////////////////////////////////
    // computer a id of this grid by position
	virtual const NFGUID ComputeCellID(const int nX, const int nY, const int nZ);
	virtual const NFGUID ComputeCellID(const NFVector3& vec);
	//////////////////////////////////////////////////////////////////////////
	// computer a id of this grid by position
	virtual const NFGUID ComputeCellID(const NFGUID& selfGrid, ECELL_DIRECTION eDirection);

    // get the step lenth each two grid
    virtual const NFGUID GetStepLenth(const NFGUID& selfGrid, const NFGUID& otherGrid);

    // get some grids that around this grid(not include self)
    virtual const int GetAroundCell(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NF_SHARE_PTR<NFSceneCellInfo>* gridList,
                                    ECELL_AROUND eAround = ECELL_AROUND_9);


    // get some objects that around this grid(not include self)
    virtual const int GetAroundObject(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NFDataList& objectList,
                                      ECELL_AROUND eAround = ECELL_AROUND_9);


    // get a grid who connected it by direction
    virtual NF_SHARE_PTR<NFSceneCellInfo> GetConnectCell(const int& sceneID, const int& groupID, const NFGUID& selfGrid, ECELL_DIRECTION eDirection);

    // get the pointer of this grid
    virtual NF_SHARE_PTR<NFSceneCellInfo> GetCellInfo(const int& sceneID, const int& groupID, const NFGUID& selfGrid);

protected:

	// get some grids that around this grid(not include self)
	virtual const int GetAroundCell(NF_SHARE_PTR<NFSceneCellInfo> pGridInfo, NF_SHARE_PTR<NFSceneCellInfo>* gridList,
		ECELL_AROUND eAround = ECELL_AROUND_9);

	// get some objects that around this grid(not include self)
	virtual const int GetAroundObject(NF_SHARE_PTR<NFSceneCellInfo> pGridInfo, NFDataList& objectList,
		ECELL_AROUND eAround = ECELL_AROUND_9);


	virtual int AddMoveEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr);
	virtual int AddMoveInEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr);
	virtual int AddMoveOutEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr);

	int OnObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnPositionEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar);

private:

	bool OnMoveEvent(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell, const NFGUID& toCell);
	bool OnMoveInEvent(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& toCell);
	bool OnMoveOutEvent(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell);
	
	int BeforeLeaveSceneGroup(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterEnterSceneGroup(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

private:
	const static int nCellWidth = 10;

	//cell_id -> cell_data
	typedef std::map<NFGUID, NF_SHARE_PTR<NFSceneCellInfo>> TMAP_CELL_INFO;

	//groupd_id->cell info
	typedef std::map<int, TMAP_CELL_INFO> TMAP_GROUP_INFO;

	//scene_id->group info
	typedef std::map<int, TMAP_GROUP_INFO> TMAP_SCENE_INFO;
	TMAP_SCENE_INFO mtCellInfoMap;


	std::vector<CELL_MOVE_EVENT_FUNCTOR_PTR> mMoveEventHandler;
	std::vector<CELL_MOVE_EVENT_FUNCTOR_PTR> mMoveInEventHandler;
	std::vector<CELL_MOVE_EVENT_FUNCTOR_PTR> mMoveOutEventHandler;
private:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
	NFIEventModule* m_pEventModule;
};

#endif
