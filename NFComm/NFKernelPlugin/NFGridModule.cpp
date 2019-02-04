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

#include "NFGridModule.h"


NFGridModule::NFGridModule(NFIPluginManager* p)
{
}

NFGridModule::~NFGridModule()
{
}

const NFGUID NFGridModule::OnObjectMove(const NFGUID& self, const int& sceneID, const NFGUID& lastGrid, const int nX, const int nY, const int nZ)
{
    const NFGUID& currGrid = ComputerGridID(nX, nY, nZ);
    if (currGrid == lastGrid)
    {
        return lastGrid;
    }

    OnMoveOut(self, currGrid, lastGrid);
    OnMoveIn(self, currGrid, lastGrid);

    return currGrid;
}

const NFGUID NFGridModule::OnObjectEntry(const NFGUID& self, const int& sceneID, const int nX, const int nY, const int nZ)
{
    NFGUID gridID = ComputerGridID(nX, nY, nZ);
    NFSceneGridInfo* pGridInfo = GetGridInfo(gridID);
    if (pGridInfo)
    {
        OnMoveIn(self, gridID, gridID);
    }

    return gridID;
}

const NFGUID NFGridModule::OnObjectLeave(const NFGUID& self, const int& sceneID, const NFGUID& lastGrid)
{

    OnMoveOut(self, lastGrid, lastGrid);

    return NFGUID();
}

const NFGUID NFGridModule::ComputerGridID(const int nX, const int nY, const int nZ)
{
    int nGridXIndex = nX / nGridWidth;
    int nGridZIndex = nZ / nGridWidth;

    return NFGUID(nGridXIndex, nGridZIndex);
}

const NFGUID NFGridModule::GetStepLenth(const NFGUID& selfGrid, const NFGUID& otherGrid)
{
    return NFGUID(abs(otherGrid.nHead64 - selfGrid.nHead64), abs(otherGrid.nData64 - selfGrid.nData64));
}

const int NFGridModule::GetAroundGrid(const NFGUID& selfGrid, NFList<NFSceneGridInfo*>& gridList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    NFSceneGridInfo* pGridInfo = GetGridInfo(selfGrid);
    return GetAroundGrid(pGridInfo, gridList, eAround);
}

const int NFGridModule::GetAroundGrid(NFSceneGridInfo* pGridInfo, NFList<NFSceneGridInfo*>& gridList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    int nObjectCount = 0;

    if (!pGridInfo)
    {
        return nObjectCount;
    }

    gridList.Add(pGridInfo);

    nObjectCount += pGridInfo->Count();

    switch (eAround)
    {
        case EGRID_AROUND_9:
        {
            for (int i = 0; i < EGRID_DIRECTION_MAXCOUNT; i++)
            {
                NFSceneGridInfo* pInfo = pGridInfo->GetConnectGrid((EGRID_DIRECTION)i);
                if (pInfo)
                {
                    gridList.Add(pInfo);
                    nObjectCount += pInfo->Count();
                }
            }
        }
        break;
        case EGRID_AROUND_16:
            break;
        case EGRID_AROUND_25:
            break;
        case EGRID_AROUND_ALL:
            break;
        default:
            break;
    }

    return nObjectCount;
}

const int NFGridModule::GetAroundObject(const NFGUID& selfGrid, NFDataList& objectList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    NFSceneGridInfo* pGridInfo = GetGridInfo(selfGrid);
    if (pGridInfo)
    {
        return GetAroundObject(pGridInfo, objectList, eAround);
    }
    return 0;
}

const int NFGridModule::GetAroundObject(NFSceneGridInfo* pGridInfo, NFDataList& objectList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    if (!pGridInfo)
    {
        return 0;
    }

    NFList<NFSceneGridInfo*> gridList;
    if (GetAroundGrid(pGridInfo, gridList, eAround) > 0)
    {
        for (int i = 0; i < gridList.Count(); i++)
        {
            NFSceneGridInfo* pGridInfo = (NFSceneGridInfo*)(gridList.Get(i));
            if (pGridInfo)
            {
                NFGUID ident;
                bool bRet = pGridInfo->First(ident);
                while (bRet)
                {
                    objectList.Add(ident);
                    bRet = pGridInfo->Next(ident);
                }
            }
        }
    }

    return objectList.GetCount();
}

NFSceneGridInfo* NFGridModule::GetConnectGrid(const NFGUID& selfGrid, EGRID_DIRECTION eDirection)
{
    NFSceneGridInfo* pGridInfo = GetGridInfo(selfGrid);
    if (pGridInfo)
    {
        return pGridInfo->GetConnectGrid(eDirection);
    }

    return NULL;
}

NFSceneGridInfo* NFGridModule::GetGridInfo(const NFGUID& selfGrid)
{
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.find(selfGrid);
    if (it != mtGridInfoMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool NFGridModule::Init()
{
    // init all grid
    for (int nWidthPos = 0; nWidthPos < nSceneWidth; nWidthPos += nGridWidth)
    {
        for (int nHeightPos = 0; nHeightPos < nSceneWidth; nHeightPos += nGridWidth)
        {
            NFGUID gridID = ComputerGridID(nWidthPos, 0, nHeightPos);
            if (!GetGridInfo(gridID))
            {
                RegisterGrid(gridID);
            }
        }
    }
    //     for (int nWidthPos = -mnSceneWidth + 1; nWidthPos < mnSceneWidth; nWidthPos++)
    //     {
    //         for (int nHeightPos = -mnSceneWidth + 1; nHeightPos < mnSceneWidth; nHeightPos++)
    //         {
    //             NFGUID gridID = ComputerGridID(nWidthPos, 0, nHeightPos);
    //             if (!GetGridInfo(gridID))
    //             {
    //                 RegisterGrid(gridID);
    //             }
    //         }
    //     }

    // connect all grid
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.begin();
    for (it; it != mtGridInfoMap.end(); it++)
    {
        NFSceneGridInfo* aroundGrid[EGRID_DIRECTION_MAXCOUNT] = { 0 };

        for (int  i = 0; i < EGRID_DIRECTION_MAXCOUNT; i++)
        {
            aroundGrid[i] = GetConnectGrid(it->first, (EGRID_DIRECTION)i);
        }

        it->second->Init(aroundGrid);
    }

    return true;
}

bool NFGridModule::AfterInit()
{
	return false;
}

bool NFGridModule::BeforeShut()
{
	TMAP_GRID_INFO::iterator it = mtGridInfoMap.begin();
	for (it; it != mtGridInfoMap.end(); it++)
	{
		if (NULL != it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	mtGridInfoMap.clear();

	return false;
}

bool NFGridModule::Shut()
{
	return false;
}

bool NFGridModule::Execute()
{
	return false;
}

bool NFGridModule::RegisterGrid(const NFGUID& grid)
{
    if (!GetGridInfo(grid))
    {
        NFSceneGridInfo* pInfo = NF_NEW NFSceneGridInfo(grid);
        mtGridInfoMap.insert(TMAP_GRID_INFO::value_type(grid, pInfo));

        return true;
    }

    return false;
}

bool NFGridModule::OnMoveIn(const NFGUID& self, const NFGUID& grid, const NFGUID& lastGrid)
{
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.find(grid);
    if (it != mtGridInfoMap.end())
    {
        return it->second->Add(self);
    }

    return false;
}

bool NFGridModule::OnMoveOut(const NFGUID& self, const NFGUID& grid, const NFGUID& lastGrid)
{
    //grid为现在的格子
    TMAP_GRID_INFO::iterator it = mtGridInfoMap.find(grid);
    if (it != mtGridInfoMap.end())
    {
        return it->second->Remove(self);
    }

    return true;
}

