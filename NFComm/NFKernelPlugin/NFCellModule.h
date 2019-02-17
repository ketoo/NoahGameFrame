/*
			This file is part of:
				NoahFrame
		https://github.com/ketoo/NoahGameFrame

	Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

class NFSceneCellInfo
    : public NFList<NFGUID>
{
public:

    NFSceneCellInfo(const int& sceneID, const int& groupID, const NFGUID& gridID)
    {
		mnSceneID = sceneID;
		mnGroupID = groupID;
		mGridID = gridID;

        for (int i = ECELL_TOP; i < ECELL_DIRECTION_MAXCOUNT; i++)
        {
            mAroundGrid[i] = NULL;
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
            mAroundGrid[i] = pGridArray[i];
        }
    }

    void Finale()
    {
    }

	NF_SHARE_PTR<NFSceneCellInfo> GetConnectGrid(ECELL_DIRECTION eDirection)
    {
        return mAroundGrid[eDirection];
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
		return mGridID;
	}
protected:
private:
    NF_SHARE_PTR<NFSceneCellInfo> mAroundGrid[ECELL_DIRECTION_MAXCOUNT];
	NFGUID mGridID;
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

	virtual const bool CreateGroupGrid(const NFGUID& self, const int& sceneID, const int& groupID);
	virtual const bool DestroyGroupGrid(const NFGUID& self, const int& sceneID, const int& groupID);

    // the event that a object are moving
    virtual const NFGUID OnObjectMove(const NFGUID& self, const int& sceneID, const int& groupID,
                                         const NFGUID& lastGrid, const int nX, const int nY, const int nZ);

    // the event that a object has entried
    virtual const NFGUID OnObjectEntry(const NFGUID& self, const int& sceneID, const int& groupID,
                                          const int nX, const int nY, const int nZ);

    // the event that a object has leaved
    virtual const NFGUID OnObjectLeave(const NFGUID& self, const int& sceneID, const int& groupID,
											const int nX, const int nY, const int nZ);

    //////////////////////////////////////////////////////////////////////////
    // computer a id of this grid by position
	virtual const NFGUID ComputerGridID(const int nX, const int nY, const int nZ);
	//////////////////////////////////////////////////////////////////////////
	// computer a id of this grid by position
	virtual const NFGUID ComputerGridID(const NFGUID& selfGrid, ECELL_DIRECTION eDirection);

    // get the step lenth each two grid
    virtual const NFGUID GetStepLenth(const NFGUID& selfGrid, const NFGUID& otherGrid);

    // get some grids that around this grid(not include self)
    virtual const int GetAroundGrid(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NF_SHARE_PTR<NFSceneCellInfo>* gridList,
                                    ECELL_AROUND eAround = ECELL_AROUND_9);


    // get some objects that around this grid(not include self)
    virtual const int GetAroundObject(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NFDataList& objectList,
                                      ECELL_AROUND eAround = ECELL_AROUND_9);


    // get a grid who connected it by direction
    virtual NF_SHARE_PTR<NFSceneCellInfo> GetConnectGrid(const int& sceneID, const int& groupID, const NFGUID& selfGrid, ECELL_DIRECTION eDirection);

    // get the pointer of this grid
    virtual NF_SHARE_PTR<NFSceneCellInfo> GetGridInfo(const int& sceneID, const int& groupID, const NFGUID& selfGrid);

protected:

	// get some grids that around this grid(not include self)
	virtual const int GetAroundGrid(NF_SHARE_PTR<NFSceneCellInfo> pGridInfo, NF_SHARE_PTR<NFSceneCellInfo>* gridList,
		ECELL_AROUND eAround = ECELL_AROUND_9);

	// get some objects that around this grid(not include self)
	virtual const int GetAroundObject(NF_SHARE_PTR<NFSceneCellInfo> pGridInfo, NFDataList& objectList,
		ECELL_AROUND eAround = ECELL_AROUND_9);

private:

    bool OnMoveIn(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromGrid, const NFGUID& toGrid);

    bool OnMoveOut(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromGrid, const NFGUID& toGrid);

private:
	const static int nGridWidth = 10;
	const static int nSceneWidth = 1000;

	typedef std::map<NFGUID, NF_SHARE_PTR<NFSceneCellInfo>> TMAP_GRID_INFO;
	typedef std::map<int, TMAP_GRID_INFO> TMAP_GROUP_INFO;
	typedef std::map<int, TMAP_GROUP_INFO> TMAP_SCENE_INFO;
	TMAP_SCENE_INFO mtGridInfoMap;

private:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
	NFIEventModule* m_pEventModule;
};

#endif
