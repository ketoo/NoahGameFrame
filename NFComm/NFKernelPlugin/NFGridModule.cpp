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
	pPluginManager = p;
}

NFGridModule::~NFGridModule()
{
}


bool NFGridModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();


	return true;
}

bool NFGridModule::AfterInit()
{

	//init all scene
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			int nSceneID = lexical_cast<int>(strId);
			TMAP_SCENE_INFO::iterator it = mtGridInfoMap.find(nSceneID);
			if (it == mtGridInfoMap.end())
			{
				TMAP_GROUP_INFO groupInfo;
				groupInfo.insert(TMAP_GROUP_INFO::value_type(0, std::map<NFGUID, NF_SHARE_PTR<NFSceneGridInfo>>()));
				mtGridInfoMap.insert(TMAP_SCENE_INFO::value_type(nSceneID, groupInfo));
			}
		}


		TMAP_SCENE_INFO::iterator it = mtGridInfoMap.begin();
		for (; it != mtGridInfoMap.end(); it++)
		{
			// init all grid, start from position 0
			//the default group's id is 0
			TMAP_GROUP_INFO::iterator itGroup = it->second.find(0);

			for (int nWidthPos = 0; nWidthPos < nSceneWidth; nWidthPos += nGridWidth)
			{
				for (int nHeightPos = 0; nHeightPos < nSceneWidth; nHeightPos += nGridWidth)
				{
					NFGUID gridID = ComputerGridID(nWidthPos, 0, nHeightPos);
					itGroup->second.insert(TMAP_GRID_INFO::value_type(gridID, NF_SHARE_PTR<NFSceneGridInfo>(NF_NEW NFSceneGridInfo(it->first, itGroup->first, gridID))));
				}
			}
		}
	}

	//all grids  connect together
	TMAP_SCENE_INFO::iterator it = mtGridInfoMap.begin();
	for (it; it != mtGridInfoMap.end(); it++)
	{
		//std::map<NFGUID, NF_SHARE_PTR<NFSceneGridInfo>>& gridMap = it->second;

		NFSceneGridInfo* aroundGrid[EGRID_DIRECTION_MAXCOUNT] = { 0 };

		for (int i = 0; i < EGRID_DIRECTION_MAXCOUNT; i++)
		{
			//aroundGrid[i] = GetConnectGrid(it->first, 0, (EGRID_DIRECTION)i);
		}

		//it->second->Init(aroundGrid);
	}

	return false;
}

bool NFGridModule::BeforeShut()
{
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

const bool NFGridModule::RequestGroupGrid(const NFGUID & self, const int & sceneID, const int & groupID)
{
	return false;
}

const NFGUID NFGridModule::OnObjectMove(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& lastGrid, const int nX, const int nY, const int nZ)
{
    const NFGUID& currGrid = ComputerGridID(nX, nY, nZ);
    if (currGrid == lastGrid)
    {
        return lastGrid;
    }

    OnMoveOut(self, sceneID, groupID, lastGrid, currGrid);
    OnMoveIn(self, sceneID, groupID, lastGrid, currGrid);

    return currGrid;
}

const NFGUID NFGridModule::OnObjectEntry(const NFGUID& self, const int& sceneID, const int& groupID, const int nX, const int nY, const int nZ)
{
    NFGUID gridID = ComputerGridID(nX, nY, nZ);
    NF_SHARE_PTR<NFSceneGridInfo> pGridInfo = GetGridInfo(sceneID, groupID, gridID);
    if (pGridInfo)
    {
        OnMoveIn(self, sceneID, groupID, gridID, gridID);
    }

    return gridID;
}

const NFGUID NFGridModule::OnObjectLeave(const NFGUID& self, const int& sceneID, const int& groupID, const int nX, const int nY, const int nZ)
{
	NFGUID gridID = ComputerGridID(nX, nY, nZ);
	NF_SHARE_PTR<NFSceneGridInfo> pGridInfo = GetGridInfo(sceneID, groupID, gridID);
	if (pGridInfo)
	{
		OnMoveOut(self, sceneID, groupID, gridID, gridID);
	}

    return NFGUID();
}

const NFGUID NFGridModule::ComputerGridID(const int nX, const int nY, const int nZ)
{
    int nGridXIndex = nX / nGridWidth;
    int nGridZIndex = nZ / nGridWidth;

    return NFGUID(nGridXIndex, nGridZIndex);
}

const NFGUID NFGridModule::ComputerGridID(const NFGUID & selfGrid, EGRID_DIRECTION eDirection)
{
	switch (eDirection)
	{
	case EGRID_TOP:
		return NFGUID(selfGrid.nHead64, selfGrid.nData64 + 1);
		break;
	case EGRID_DOWN:
		return NFGUID(selfGrid.nHead64, selfGrid.nData64 - 1);
		break;
	case EGRID_LEFT:
		return NFGUID(selfGrid.nHead64 - 1, selfGrid.nData64);
		break;
	case EGRID_RIGHT:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64);
		break;
	case EGRID_LEFT_TOP:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64 + 1);
		break;
	case EGRID_LEFT_DOWN:
		return NFGUID(selfGrid.nHead64 - 1, selfGrid.nData64 - 1);
		break;
	case EGRID_RIGHT_TOP:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64 + 1);
		break;
	case EGRID_RIGHT_DOWN:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64 - 1);
		break;
	default:
		break;
	}
	return NFGUID();
}

const NFGUID NFGridModule::GetStepLenth(const NFGUID& selfGrid, const NFGUID& otherGrid)
{
    return NFGUID(std::abs(otherGrid.nHead64 - selfGrid.nHead64), std::abs(otherGrid.nData64 - selfGrid.nData64));
}

const int NFGridModule::GetAroundGrid(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NFList<NF_SHARE_PTR<NFSceneGridInfo>>& gridList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
	NF_SHARE_PTR<NFSceneGridInfo> pGridInfo = GetGridInfo(sceneID, groupID, selfGrid);
    return GetAroundGrid(pGridInfo, gridList, eAround);
}

const int NFGridModule::GetAroundGrid(NF_SHARE_PTR<NFSceneGridInfo> pGridInfo, NFList<NF_SHARE_PTR<NFSceneGridInfo>>& gridList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
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
				NF_SHARE_PTR<NFSceneGridInfo> pInfo = pGridInfo->GetConnectGrid((EGRID_DIRECTION)i);
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

const int NFGridModule::GetAroundObject(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NFDataList& objectList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    NF_SHARE_PTR<NFSceneGridInfo> pGridInfo = GetGridInfo(sceneID, groupID, selfGrid);
    if (pGridInfo)
    {
        return GetAroundObject(pGridInfo, objectList, eAround);
    }
    return 0;
}

const int NFGridModule::GetAroundObject(NF_SHARE_PTR<NFSceneGridInfo> pGridInfo, NFDataList& objectList, EGRID_AROUND eAround /*= EGRID_AROUND_9 */)
{
    if (!pGridInfo)
    {
        return 0;
    }

    NFList<NF_SHARE_PTR<NFSceneGridInfo>> gridList;
    if (GetAroundGrid(pGridInfo, gridList, eAround) > 0)
    {
        for (int i = 0; i < gridList.Count(); i++)
        {
			NF_SHARE_PTR<NFSceneGridInfo> pGridInfo = nullptr;
            if (gridList.Get(i, pGridInfo))
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

NF_SHARE_PTR<NFSceneGridInfo> NFGridModule::GetConnectGrid(const int& sceneID, const int& groupID, const NFGUID& selfGrid, EGRID_DIRECTION eDirection)
{
	NF_SHARE_PTR<NFSceneGridInfo> pGridInfo = GetGridInfo(sceneID, groupID, selfGrid);
    if (pGridInfo)
    {
        //return pGridInfo->GetConnectGrid(sceneID, eDirection);
    }

    return NULL;
}

NF_SHARE_PTR<NFSceneGridInfo> NFGridModule::GetGridInfo(const int& sceneID, const int& groupID, const NFGUID& selfGrid)
{
    TMAP_SCENE_INFO::iterator it = mtGridInfoMap.find(sceneID);
    if (it != mtGridInfoMap.end())
    {
		TMAP_GROUP_INFO::iterator itGroup = it->second.find(groupID);
		if (itGroup != it->second.end())
		{
			TMAP_GRID_INFO::iterator itGrid = itGroup->second.find(selfGrid);
			if (itGrid != itGroup->second.end())
			{
				return itGrid->second;
			}
		}
    }

    return NULL;
}

bool NFGridModule::OnMoveIn(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromGrid, const NFGUID& toGrid)
{
	NF_SHARE_PTR<NFSceneGridInfo> pGrigInfo = GetGridInfo(sceneID, groupID, toGrid);
	if (pGrigInfo)
	{
		pGrigInfo->Add(self);
	}

    return false;
}

bool NFGridModule::OnMoveOut(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromGrid, const NFGUID& toGrid)
{
	NF_SHARE_PTR<NFSceneGridInfo> pGrigInfo = GetGridInfo(sceneID, groupID, fromGrid);
	if (pGrigInfo)
	{
		return pGrigInfo->Remove(self);
	}

	return false;
}

