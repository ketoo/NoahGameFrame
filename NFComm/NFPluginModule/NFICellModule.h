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

#ifndef NFI_CELL_MODULE_H
#define NFI_CELL_MODULE_H

#include <list>
#include <iostream>
#include <algorithm>
#include <array>
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFGUID.h"

class NFSceneCellInfo;

enum ECELL_DIRECTION
{
    ECELL_TOP = 0,
    ECELL_DOWN = 1,
    ECELL_LEFT = 2,
    ECELL_RIGHT = 3,

    ECELL_LEFT_TOP = 4,
    ECELL_LEFT_DOWN = 5,
    ECELL_RIGHT_TOP = 6,
    ECELL_RIGHT_DOWN = 7,

    ECELL_DIRECTION_MAXCOUNT = 8,
};

enum ECELL_AROUND
{
    ECELL_SELF,
    ECELL_AROUND_9,
    ECELL_AROUND_16,
    ECELL_AROUND_25,
    ECELL_AROUND_ALL,
};

class NFICellModule
	: public NFIModule
{
public:
    virtual ~NFICellModule() {}
    // the event that a object are moving
    virtual const NFGUID OnObjectMove(const NFGUID& self, const int& sceneID, const int& groupID,
                                         const NFGUID& lastGrid, const int nX, const int nY, const int nZ) = 0;

    // the event that a object has entried
    virtual const NFGUID OnObjectEntry(const NFGUID& self, const int& sceneID, const int& groupID,
                                          const int nX, const int nY, const int nZ) = 0;

    // the event that a object has leaved
    virtual const NFGUID OnObjectLeave(const NFGUID& self, const int& sceneID, const int& groupID,
											const int nX, const int nY, const int nZ) = 0;

    //////////////////////////////////////////////////////////////////////////
    

protected:
private:
};

#endif
