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

#include "NFCellModule.h"


NFCellModule::NFCellModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCellModule::~NFCellModule()
{
}


bool NFCellModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();


	return true;
}

bool NFCellModule::AfterInit()
{

	//init all scene
	NFList<NF_SHARE_PTR<NFSceneCellInfo>> gridList;

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
				groupInfo.insert(TMAP_GROUP_INFO::value_type(0, std::map<NFGUID, NF_SHARE_PTR<NFSceneCellInfo>>()));
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
					NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = NF_SHARE_PTR<NFSceneCellInfo>(NF_NEW NFSceneCellInfo(it->first, itGroup->first, gridID));

					itGroup->second.insert(TMAP_GRID_INFO::value_type(gridID, pGridInfo));
					gridList.Add(pGridInfo);
				}
			}
		}
	}

	//all grids must connect together
	NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = nullptr;
	for (gridList.First(pGridInfo); pGridInfo != nullptr; gridList.Next(pGridInfo))
	{
		NF_SHARE_PTR<NFSceneCellInfo> aroundGrid[ECELL_DIRECTION_MAXCOUNT] = { 0 };
		NFGUID gridID[ECELL_DIRECTION_MAXCOUNT];

		for (int i = 0; i <ECELL_DIRECTION_MAXCOUNT; ++i)
		{
			gridID[i] = ComputerGridID(pGridInfo->GetID(), (ECELL_DIRECTION)i);
			aroundGrid[i] = GetGridInfo(pGridInfo->GetSceneID(), pGridInfo->GetGroupID(), gridID[i]);
		}

		pGridInfo->Init(aroundGrid);

		pGridInfo = nullptr;
	}

	//prepare group grid pool to enhance the performance when requesting a new group 

	return false;
}

bool NFCellModule::BeforeShut()
{
	mtGridInfoMap.clear();

	return false;
}

bool NFCellModule::Shut()
{
	return false;
}

bool NFCellModule::Execute()
{
	return false;
}

const bool NFCellModule::CreateGroupGrid(const NFGUID & self, const int & sceneID, const int & groupID)
{
	//we would have a group grid pool
	TMAP_SCENE_INFO::iterator it = mtGridInfoMap.find(sceneID);
	if (it != mtGridInfoMap.end())
	{
		TMAP_GROUP_INFO::iterator itNewGroup = it->second.find(groupID);
		TMAP_GROUP_INFO::iterator itGroup0 = it->second.find(0);
		if (itNewGroup == it->second.end()
			&& itGroup0 != it->second.end())
		{
			TMAP_GRID_INFO::iterator itGrid0 = itGroup0->second.begin();
			for (itGrid0; itGrid0 != itGroup0->second.end(); itGrid0++)
			{
				NF_SHARE_PTR<NFSceneCellInfo> pGridInfo0 = itGrid0->second;
				//NF_SHARE_PTR<NFSceneCellInfo> pNewGridInfo = NF_SHARE_PTR<NFSceneCellInfo>(NF_NEW NFSceneCellInfo(pGridInfo0));
			}
		}
	}

	return false;
}

const bool NFCellModule::DestroyGroupGrid(const NFGUID & self, const int & sceneID, const int & groupID)
{
	return false;
}

const NFGUID NFCellModule::OnObjectMove(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& lastGrid, const int nX, const int nY, const int nZ)
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

const NFGUID NFCellModule::OnObjectEntry(const NFGUID& self, const int& sceneID, const int& groupID, const int nX, const int nY, const int nZ)
{
    NFGUID gridID = ComputerGridID(nX, nY, nZ);
    NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = GetGridInfo(sceneID, groupID, gridID);
    if (pGridInfo)
    {
        OnMoveIn(self, sceneID, groupID, gridID, gridID);
    }

    return gridID;
}

const NFGUID NFCellModule::OnObjectLeave(const NFGUID& self, const int& sceneID, const int& groupID, const int nX, const int nY, const int nZ)
{
	NFGUID gridID = ComputerGridID(nX, nY, nZ);
	NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = GetGridInfo(sceneID, groupID, gridID);
	if (pGridInfo)
	{
		OnMoveOut(self, sceneID, groupID, gridID, gridID);
	}

    return NFGUID();
}

const NFGUID NFCellModule::ComputerGridID(const int nX, const int nY, const int nZ)
{
    int nGridXIndex = nX / nGridWidth;
    int nGridZIndex = nZ / nGridWidth;

    return NFGUID(nGridXIndex, nGridZIndex);
}

const NFGUID NFCellModule::ComputerGridID(const NFGUID & selfGrid, ECELL_DIRECTION eDirection)
{
	switch (eDirection)
	{
	case ECELL_TOP:
		return NFGUID(selfGrid.nHead64, selfGrid.nData64 + 1);
		break;
	case ECELL_DOWN:
		return NFGUID(selfGrid.nHead64, selfGrid.nData64 - 1);
		break;
	case ECELL_LEFT:
		return NFGUID(selfGrid.nHead64 - 1, selfGrid.nData64);
		break;
	case ECELL_RIGHT:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64);
		break;
	case ECELL_LEFT_TOP:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64 + 1);
		break;
	case ECELL_LEFT_DOWN:
		return NFGUID(selfGrid.nHead64 - 1, selfGrid.nData64 - 1);
		break;
	case ECELL_RIGHT_TOP:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64 + 1);
		break;
	case ECELL_RIGHT_DOWN:
		return NFGUID(selfGrid.nHead64 + 1, selfGrid.nData64 - 1);
		break;
	default:
		break;
	}
	return NFGUID();
}

const NFGUID NFCellModule::GetStepLenth(const NFGUID& selfGrid, const NFGUID& otherGrid)
{
    return NFGUID(std::abs(otherGrid.nHead64 - selfGrid.nHead64), std::abs(otherGrid.nData64 - selfGrid.nData64));
}

const int NFCellModule::GetAroundGrid(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NF_SHARE_PTR<NFSceneCellInfo>* gridList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
{
	NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = GetGridInfo(sceneID, groupID, selfGrid);
    return GetAroundGrid(pGridInfo, gridList, eAround);
}

const int NFCellModule::GetAroundGrid(NF_SHARE_PTR<NFSceneCellInfo> pGridInfo, NF_SHARE_PTR<NFSceneCellInfo>* gridList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
{
    int nObjectCount = 0;

    if (!pGridInfo)
    {
        return nObjectCount;
    }

    gridList[ECELL_DIRECTION_MAXCOUNT] = pGridInfo;

    nObjectCount += pGridInfo->Count();

    switch (eAround)
    {
        case ECELL_AROUND_9:
        {
            for (int i = 0; i < ECELL_DIRECTION_MAXCOUNT; i++)
            {
                NF_SHARE_PTR<NFSceneCellInfo> pInfo = pGridInfo->GetConnectGrid((ECELL_DIRECTION)i);
                if (pInfo)
                {
					gridList[i] = pInfo;
                    nObjectCount += pInfo->Count();
                }
            }
        }
        break;
        case ECELL_AROUND_16:
            break;
        case ECELL_AROUND_25:
            break;
        case ECELL_AROUND_ALL:
            break;
        default:
            break;
    }

    return nObjectCount;
}

const int NFCellModule::GetAroundObject(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NFDataList& objectList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
{
    NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = GetGridInfo(sceneID, groupID, selfGrid);
    if (pGridInfo)
    {
        return GetAroundObject(pGridInfo, objectList, eAround);
    }
    return 0;
}

const int NFCellModule::GetAroundObject(NF_SHARE_PTR<NFSceneCellInfo> pGridInfo, NFDataList& objectList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
{
    if (!pGridInfo)
    {
        return 0;
    }


	NF_SHARE_PTR<NFSceneCellInfo> aroundGrid[ECELL_DIRECTION_MAXCOUNT];
    if (GetAroundGrid(pGridInfo, aroundGrid, eAround) > 0)
    {
        for (int i = 0; i < ECELL_DIRECTION_MAXCOUNT; i++)
        {
			NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = aroundGrid[i];
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

NF_SHARE_PTR<NFSceneCellInfo> NFCellModule::GetConnectGrid(const int& sceneID, const int& groupID, const NFGUID& selfGrid, ECELL_DIRECTION eDirection)
{
	NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = GetGridInfo(sceneID, groupID, selfGrid);
    if (pGridInfo)
    {
        return pGridInfo->GetConnectGrid(eDirection);
    }

    return NULL;
}

NF_SHARE_PTR<NFSceneCellInfo> NFCellModule::GetGridInfo(const int& sceneID, const int& groupID, const NFGUID& selfGrid)
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

bool NFCellModule::OnMoveIn(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromGrid, const NFGUID& toGrid)
{
	NF_SHARE_PTR<NFSceneCellInfo> pGrigInfo = GetGridInfo(sceneID, groupID, toGrid);
	if (pGrigInfo)
	{
		pGrigInfo->Add(self);
	}

    return false;
}

bool NFCellModule::OnMoveOut(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromGrid, const NFGUID& toGrid)
{
	NF_SHARE_PTR<NFSceneCellInfo> pGrigInfo = GetGridInfo(sceneID, groupID, fromGrid);
	if (pGrigInfo)
	{
		return pGrigInfo->Remove(self);
	}

	return false;
}

