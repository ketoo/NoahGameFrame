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


#ifndef NF_GRID_MODULE_H
#define NF_GRID_MODULE_H

#include <iostream>
#include <assert.h>
#include <map>
#include "NFComm/NFPluginModule/NFIGridModule.h"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"

class NFSceneGridInfo
    : public NFList<NFGUID>
{
public:

    NFSceneGridInfo(const NFGUID& gridID)
    {
        mGridID = gridID;
        for (int i = EGRID_TOP; i < EGRID_DIRECTION_MAXCOUNT; i++)
        {
            mAroundGrid[i] = NULL;
        }
    }

    virtual ~NFSceneGridInfo()
    {
        // TODO
    }

    void Init(NFSceneGridInfo** pGridArray)
    {
        for (int i = EGRID_TOP; i < EGRID_DIRECTION_MAXCOUNT; i++)
        {
            mAroundGrid[i] = pGridArray[i];
        }
    }

    void Finale()
    {
    }

    NFSceneGridInfo* GetConnectGrid(EGRID_DIRECTION eDirection)
    {
        return mAroundGrid[eDirection];
    }

protected:
private:
    NFSceneGridInfo* mAroundGrid[EGRID_DIRECTION_MAXCOUNT];
	NFGUID mGridID;
};

class NFGridModule
    : public NFIGridModule
{
public:
	NFGridModule(NFIPluginManager* p);

    virtual ~NFGridModule();

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

    // the event that a object are moving
    virtual const NFGUID OnObjectMove(const NFGUID& self, const int& sceneID,
                                         const NFGUID& lastGrid, const int nX, const int nY, const int nZ);

    // the event that a object has entried
    virtual const NFGUID OnObjectEntry(const NFGUID& self, const int& sceneID,
                                          const int nX, const int nY, const int nZ);

    // the event that a object has leaved
    virtual const NFGUID OnObjectLeave(const NFGUID& self, const int& sceneID,
                                          const NFGUID& lastGrid);

    //////////////////////////////////////////////////////////////////////////
    // computer a id of this grid by position
    static const NFGUID ComputerGridID(const int nX, const int nY, const int nZ);

    // get the step lenth each two grid
    virtual const NFGUID GetStepLenth(const NFGUID& selfGrid, const NFGUID& otherGrid);

    // get some grids that around this grid
    virtual const int GetAroundGrid(const NFGUID& selfGrid, NFList<NFSceneGridInfo*>& gridList,
                                    EGRID_AROUND eAround = EGRID_AROUND_9);


    // get some objects that around this grid
    virtual const int GetAroundObject(const NFGUID& selfGrid, NFDataList& objectList,
                                      EGRID_AROUND eAround = EGRID_AROUND_9);


    // get a grid who connected it by direction
    virtual NFSceneGridInfo* GetConnectGrid(const NFGUID& selfGrid, EGRID_DIRECTION eDirection);

    // get the pointer of this grid
    virtual NFSceneGridInfo* GetGridInfo(const NFGUID& selfGrid);

protected:

	// get some grids that around this grid
	virtual const int GetAroundGrid(NFSceneGridInfo* pGridInfo, NFList<NFSceneGridInfo*>& gridList,
		EGRID_AROUND eAround = EGRID_AROUND_9);

	// get some objects that around this grid
	virtual const int GetAroundObject(NFSceneGridInfo* pGridInfo, NFList<NFSceneGridInfo*>& objectList,
		EGRID_AROUND eAround = EGRID_AROUND_9);

private:

    bool RegisterGrid(const NFGUID& grid);

    bool OnMoveIn(const NFGUID& self, const NFGUID& grid, const NFGUID& lastGrid);

    bool OnMoveOut(const NFGUID& self, const NFGUID& grid, const NFGUID& lastGrid);

private:
	const static int nGridWidth = 10;
	const static int nSceneWidth = 10000;

    typedef std::map<NFGUID, NFSceneGridInfo*> TMAP_GRID_INFO;
    TMAP_GRID_INFO mtGridInfoMap;

};

#endif
