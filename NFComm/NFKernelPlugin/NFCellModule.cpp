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
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCellModule::OnObjectEvent);
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCellModule::OnObjectEvent);

	//NF SYNC
	bool bCell = false;
	std::shared_ptr<NFIClass> xServerLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xServerLogicClass)
	{
		const std::vector<std::string>& strIdList = xServerLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			const int nCell = 1;//m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Cell());
			if (pPluginManager->GetAppID() == nServerID && nCell == 1)
			{
				bCell = true;
				break;
			}
		}
	}

	if (!bCell)
	{
		return true;
	}
	/*
	//init all scene
	NFList<NF_SHARE_PTR<NFSceneCellInfo>> cellList;

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			int nSceneID = lexical_cast<int>(strId);
			TMAP_SCENE_INFO::iterator it = mtCellInfoMap.find(nSceneID);
			if (it == mtCellInfoMap.end())
			{
				TMAP_GROUP_INFO groupInfo;
				groupInfo.insert(TMAP_GROUP_INFO::value_type(0, std::map<NFGUID, NF_SHARE_PTR<NFSceneCellInfo>>()));
				mtCellInfoMap.insert(TMAP_SCENE_INFO::value_type(nSceneID, groupInfo));
			}
		}

		TMAP_SCENE_INFO::iterator it = mtCellInfoMap.begin();
		for (; it != mtCellInfoMap.end(); it++)
		{
			//init all cell, start from position 0
			//the default group's id is 0
			int nSceneID = it->first;
			NFVector2 vLeftBot = m_pElementModule->GetPropertyVector2(std::to_string(nSceneID), NFrame::Scene::LeftBot());
			NFVector2 vRightTop = m_pElementModule->GetPropertyVector2(std::to_string(nSceneID), NFrame::Scene::RightTop());

			TMAP_GROUP_INFO::iterator itGroup = it->second.find(0);

			for (int nPosX = vLeftBot.X(); nPosX < vRightTop.X(); nPosX += nCellWidth)
			{
				for (int nPosY = vLeftBot.Y(); nPosY < vRightTop.Y(); nPosY += nCellWidth)
				{
					NFGUID gridID = ComputeCellID(nPosX, 0, nPosY);
					NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = NF_SHARE_PTR<NFSceneCellInfo>(NF_NEW NFSceneCellInfo(it->first, itGroup->first, gridID));

					itGroup->second.insert(TMAP_CELL_INFO::value_type(gridID, pCellInfo));
					cellList.Add(pCellInfo);
				}
			}
		}
	}

	//all grids must connect together
	NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = nullptr;
	for (cellList.First(pCellInfo); pCellInfo != nullptr; cellList.Next(pCellInfo))
	{
		NF_SHARE_PTR<NFSceneCellInfo> aroundGrid[ECELL_DIRECTION_MAXCOUNT] = { 0 };
		NFGUID cellID[ECELL_DIRECTION_MAXCOUNT];

		for (int i = 0; i <ECELL_DIRECTION_MAXCOUNT; ++i)
		{
			cellID[i] = ComputeCellID(pCellInfo->GetID(), (ECELL_DIRECTION)i);
			aroundGrid[i] = GetCellInfo(pCellInfo->GetSceneID(), pCellInfo->GetGroupID(), cellID[i]);
		}

		pCellInfo->Init(aroundGrid);

		pCellInfo = nullptr;
	}

	//prepare group cell pool to enhance the performance when requesting a new group 
	*/
	return true;
}

bool NFCellModule::BeforeShut()
{
	mtCellInfoMap.clear();

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

const bool NFCellModule::CreateGroupCell(const int & sceneID, const int & groupID)
{
	/*
	//we would have a group cell pool
	TMAP_SCENE_INFO::iterator it = mtCellInfoMap.find(sceneID);
	if (it != mtCellInfoMap.end())
	{
		TMAP_GROUP_INFO::iterator itNewGroup = it->second.find(groupID);
		TMAP_GROUP_INFO::iterator itGroup0 = it->second.find(0);
		if (itNewGroup == it->second.end()
			&& itGroup0 != it->second.end())
		{
			std::map<NFGUID, NF_SHARE_PTR<NFSceneCellInfo>> newGroup;

			NFList<NF_SHARE_PTR<NFSceneCellInfo>> cellList;

			TMAP_CELL_INFO::iterator itCell0 = itGroup0->second.begin();
			for (itCell0; itCell0 != itGroup0->second.end(); itCell0++)
			{
				NF_SHARE_PTR<NFSceneCellInfo> pCellInfo0 = itCell0->second;
				NF_SHARE_PTR<NFSceneCellInfo> pNewCellInfo = NF_SHARE_PTR<NFSceneCellInfo>(NF_NEW NFSceneCellInfo(*pCellInfo0));

				newGroup.insert(TMAP_CELL_INFO::value_type(pNewCellInfo->GetID(), pNewCellInfo));
				cellList.Add(pNewCellInfo);
			}

			it->second.insert(TMAP_GROUP_INFO::value_type(groupID, newGroup));

			//all grids must connect together
			NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = nullptr;
			for (cellList.First(pCellInfo); pCellInfo != nullptr; cellList.Next(pCellInfo))
			{
				NF_SHARE_PTR<NFSceneCellInfo> aroundCell[ECELL_DIRECTION_MAXCOUNT] = { 0 };
				NFGUID cellID[ECELL_DIRECTION_MAXCOUNT];

				for (int i = 0; i <ECELL_DIRECTION_MAXCOUNT; ++i)
				{
					cellID[i] = ComputeCellID(pCellInfo->GetID(), (ECELL_DIRECTION)i);
					aroundCell[i] = GetCellInfo(pCellInfo->GetSceneID(), pCellInfo->GetGroupID(), cellID[i]);
				}

				pCellInfo->Init(aroundCell);

				pCellInfo = nullptr;
			}

		}
	}
	*/
	return false;
}

const bool NFCellModule::DestroyGroupCell(const int & sceneID, const int & groupID)
{
	TMAP_SCENE_INFO::iterator it = mtCellInfoMap.find(sceneID);
	if (it != mtCellInfoMap.end())
	{
		TMAP_GROUP_INFO::iterator itGroup = it->second.find(groupID);
		if (itGroup != it->second.end())
		{
			it->second.erase(groupID);
		}
	}

	return true;
}

const NFGUID NFCellModule::OnObjectMove(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell, const NFGUID& toCell)
{
    if (toCell == toCell)
    {
        return toCell;
    }

	OnMoveEvent(self, sceneID, groupID, fromCell, toCell);

    return toCell;
}

const NFGUID NFCellModule::OnObjectEntry(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& toCell)
{
    NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = GetCellInfo(sceneID, groupID, toCell);
    if (pCellInfo)
    {
		OnMoveInEvent(self, sceneID, groupID, toCell);
    }

    return toCell;
}

const NFGUID NFCellModule::OnObjectLeave(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell)
{
	NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = GetCellInfo(sceneID, groupID, fromCell);
	if (pCellInfo)
	{
		OnMoveOutEvent(self, sceneID, groupID, fromCell);
	}

    return NFGUID();
}

bool NFCellModule::GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3 & pos, NFDataList & list, ECELL_AROUND eAround)
{
	return m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, list);
}

bool NFCellModule::GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3 & pos, NFDataList & list, const NFGUID & noSelf, ECELL_AROUND eAround)
{
	return m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, list, noSelf);
}

bool NFCellModule::GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3 & pos, NFDataList & list, const bool bPlayer, ECELL_AROUND eAround)
{
	return m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, list, bPlayer);
}

bool NFCellModule::GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3 & pos, NFDataList & list, const bool bPlayer, const NFGUID & noSelf, ECELL_AROUND eAround)
{
	return m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, list, bPlayer, noSelf);
}

bool NFCellModule::GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3 & pos, const std::string & strClassName, NFDataList & list, ECELL_AROUND eAround)
{
	return m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, strClassName, list);
}

bool NFCellModule::GetCellObjectList(const int nSceneID, const int nGroupID, const NFVector3 & pos, const std::string & strClassName, NFDataList & list, const NFGUID & noSelf, ECELL_AROUND eAround)
{
	return m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, strClassName, list, noSelf);
}

const NFGUID NFCellModule::ComputeCellID(const int nX, const int nY, const int nZ)
{
    int nGridXIndex = nX / nCellWidth;
    int nGridZIndex = nZ / nCellWidth;

    return NFGUID(nGridXIndex, nGridZIndex);
}

const NFGUID NFCellModule::ComputeCellID(const NFVector3 & vec)
{
	return ComputeCellID(vec.X(), vec.Y(), vec.Z());
}

const NFGUID NFCellModule::ComputeCellID(const NFGUID & selfGrid, ECELL_DIRECTION eDirection)
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

const int NFCellModule::GetAroundCell(const int& sceneID, const int& groupID, const NFGUID& selfGrid, NF_SHARE_PTR<NFSceneCellInfo>* gridList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
{
	NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = GetCellInfo(sceneID, groupID, selfGrid);
    return GetAroundCell(pGridInfo, gridList, eAround);
}

const int NFCellModule::GetAroundCell(NF_SHARE_PTR<NFSceneCellInfo> pGridInfo, NF_SHARE_PTR<NFSceneCellInfo>* gridList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
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
                NF_SHARE_PTR<NFSceneCellInfo> pInfo = pGridInfo->GetConnectCell((ECELL_DIRECTION)i);
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

const int NFCellModule::GetAroundObject(const int& sceneID, const int& groupID, const NFGUID& selfCell, NFDataList& objectList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
{
    NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = GetCellInfo(sceneID, groupID, selfCell);
    if (pCellInfo)
    {
        return GetAroundObject(pCellInfo, objectList, eAround);
    }
    return 0;
}

const int NFCellModule::GetAroundObject(NF_SHARE_PTR<NFSceneCellInfo> pCellInfo, NFDataList& objectList, ECELL_AROUND eAround /*= ECELL_AROUND_9 */)
{
    if (!pCellInfo)
    {
        return 0;
    }


	NF_SHARE_PTR<NFSceneCellInfo> aroundCell[ECELL_DIRECTION_MAXCOUNT];
    if (GetAroundCell(pCellInfo, aroundCell, eAround) > 0)
    {
        for (int i = 0; i < ECELL_DIRECTION_MAXCOUNT; i++)
        {
			NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = aroundCell[i];
            if (pCellInfo)
            {
                NFGUID ident;
                bool bRet = pCellInfo->First(ident);
                while (bRet)
                {
                    objectList.Add(ident);
                    bRet = pCellInfo->Next(ident);
                }
            }
        }
    }

    return objectList.GetCount();
}

int NFCellModule::AddMoveEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr)
{
	mMoveEventHandler.push_back(functorPtr);

	return 0;
}

int NFCellModule::AddMoveInEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr)
{
	mMoveInEventHandler.push_back(functorPtr);
	return 0;
}

int NFCellModule::AddMoveOutEventCallBack(CELL_MOVE_EVENT_FUNCTOR_PTR functorPtr)
{
	mMoveOutEventHandler.push_back(functorPtr);
	return 0;
}

int NFCellModule::OnObjectEvent(const NFGUID & self, const std::string & strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::IObject::Position(), this, &NFCellModule::OnPositionEvent);
	}

	return 0;
}

int NFCellModule::OnPositionEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());
	const NFVector3& oldVec = oldVar.GetVector3();
	const NFVector3& newVec = newVar.GetVector3();

	NFGUID fromCell = ComputeCellID(oldVec);
	NFGUID toCell =	ComputeCellID(newVec);
	OnObjectMove(self, sceneID, groupID, fromCell, toCell);
	return 0;
}

NF_SHARE_PTR<NFSceneCellInfo> NFCellModule::GetConnectCell(const int& sceneID, const int& groupID, const NFGUID& selfGrid, ECELL_DIRECTION eDirection)
{
	NF_SHARE_PTR<NFSceneCellInfo> pGridInfo = GetCellInfo(sceneID, groupID, selfGrid);
    if (pGridInfo)
    {
        return pGridInfo->GetConnectCell(eDirection);
    }

    return NULL;
}

NF_SHARE_PTR<NFSceneCellInfo> NFCellModule::GetCellInfo(const int& sceneID, const int& groupID, const NFGUID& selfGrid)
{
    TMAP_SCENE_INFO::iterator it = mtCellInfoMap.find(sceneID);
    if (it != mtCellInfoMap.end())
    {
		TMAP_GROUP_INFO::iterator itGroup = it->second.find(groupID);
		if (itGroup != it->second.end())
		{
			TMAP_CELL_INFO::iterator itCell = itGroup->second.find(selfGrid);
			if (itCell != itGroup->second.end())
			{
				return itCell->second;
			}
		}
    }

    return NULL;
}

bool NFCellModule::OnMoveEvent(const NFGUID& self, const int& sceneID, const int& groupID, const NFGUID& fromCell, const NFGUID& toCell)
{
	if (groupID <= 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = GetCellInfo(sceneID, groupID, toCell);
	if (pCellInfo)
	{
		pCellInfo->Add(self);
	}

	NFGUID lastCell = fromCell;

	if (!fromCell.IsNull())
	{
		NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = GetCellInfo(sceneID, groupID, fromCell);
		if (pCellInfo)
		{
			if (!pCellInfo->Exist(self))
			{
				lastCell = NFGUID();
			}
		}
		else
		{
			lastCell = NFGUID();
		}
	}

	//BC
	for (int i = 0; i < mMoveEventHandler.size(); ++i)
	{
		CELL_MOVE_EVENT_FUNCTOR_PTR pFunPtr = mMoveEventHandler[i];
		CELL_MOVE_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, sceneID, groupID, lastCell, toCell);
	}
    return true;
}

bool NFCellModule::OnMoveInEvent(const NFGUID & self, const int & sceneID, const int & groupID, const NFGUID & toCell)
{
	if (groupID <= 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = GetCellInfo(sceneID, groupID, toCell);
	if (pCellInfo)
	{
		pCellInfo->Add(self);
	}

	//BC
	for (int i = 0; i < mMoveInEventHandler.size(); ++i)
	{
		CELL_MOVE_EVENT_FUNCTOR_PTR pFunPtr = mMoveEventHandler[i];
		CELL_MOVE_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, sceneID, groupID, NFGUID(), toCell);
	}

	return true;
}

bool NFCellModule::OnMoveOutEvent(const NFGUID & self, const int & sceneID, const int & groupID, const NFGUID & fromCell)
{
	if (groupID <= 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFSceneCellInfo> pCellInfo = GetCellInfo(sceneID, groupID, fromCell);
	if (pCellInfo)
	{
		pCellInfo->Remove(self);
	}

	//BC
	for (int i = 0; i < mMoveOutEventHandler.size(); ++i)
	{
		CELL_MOVE_EVENT_FUNCTOR_PTR pFunPtr = mMoveEventHandler[i];
		CELL_MOVE_EVENT_FUNCTOR* pFun = pFunPtr.get();
		pFun->operator()(self, sceneID, groupID, fromCell, NFGUID());
	}


	return true;
}

int NFCellModule::BeforeLeaveSceneGroup(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCellModule::AfterEnterSceneGroup(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}
