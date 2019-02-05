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

#ifndef NFI_GRID_MODULE_H
#define NFI_GRID_MODULE_H

#include <list>
#include <iostream>
#include <algorithm>
#include <array>
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFGUID.h"

class NFSceneGridInfo;

enum EGRID_DIRECTION
{
    EGRID_TOP,
    EGRID_DOWN,
    EGRID_LEFT,
    EGRID_RIGHT,

    EGRID_LEFT_TOP,
    EGRID_LEFT_DOWN,
    EGRID_RIGHT_TOP,
    EGRID_RIGHT_DOWN,

    EGRID_DIRECTION_MAXCOUNT,
};

enum EGRID_AROUND
{
    EGRID_SELF,
    EGRID_AROUND_9,
    EGRID_AROUND_16,
    EGRID_AROUND_25,
    EGRID_AROUND_ALL,
};

class NFIGridModule
	: public NFIModule
{
public:
    virtual ~NFIGridModule() {}
    // the event that a object are moving
    virtual const NFGUID OnObjectMove(const NFGUID& self, const int& sceneID,
                                         const NFGUID& lastGrid, const int nX, const int nY, const int nZ) = 0;

    // the event that a object has entried
    virtual const NFGUID OnObjectEntry(const NFGUID& self, const int& sceneID,
                                          const int nX, const int nY, const int nZ) = 0;

    // the event that a object has leaved
    virtual const NFGUID OnObjectLeave(const NFGUID& self, const int& sceneID,
                                          const NFGUID& lastGrid) = 0;

    //////////////////////////////////////////////////////////////////////////
    // computer a id of this grid by position
    //virtual const NFGUID ComputerGridID(const int nX, const int nY, const int nZ) = 0;

    // get the step lenth each two grid
    virtual const NFGUID GetStepLenth(const NFGUID& selfGrid, const NFGUID& otherGrid) = 0;

    // get some grids that around this grid
    virtual const int GetAroundGrid(const NFGUID& selfGrid, NFList<NFSceneGridInfo*>& gridList,
                                    EGRID_AROUND eAround = EGRID_AROUND_9) = 0;

    // get some objects that around this grid
    virtual const int GetAroundObject(const NFGUID& selfGrid, NFDataList& objectList,
                                      EGRID_AROUND eAround = EGRID_AROUND_9) = 0;

    // get a grid who connected it by direction
    virtual NFSceneGridInfo* GetConnectGrid(const NFGUID& selfGrid, EGRID_DIRECTION eDirection) = 0;

    // get the pointer of this grid
    virtual NFSceneGridInfo* GetGridInfo(const NFGUID& selfGrid) = 0;

protected:
private:
};

#endif
