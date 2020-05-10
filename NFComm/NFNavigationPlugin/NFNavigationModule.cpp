/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: Bluesky
   
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


#include "NFNavigationModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFNavigationModule::Init()
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			int sceneId = lexical_cast<int>(strIdList[i]);
			std::string navigationResPath = m_pElementModule->GetPropertyManager(strId)->GetPropertyString(NFrame::Scene::NavigationResPath());
			LoadNavigation(sceneId, navigationResPath);

			//const std::string& strId = strIdList[i];
			//NFINT64 sceneId = elementModule->GetPropertyManager(strId)->GetPropertyInt(NFrame::Scene::SceneID);// lexical_cast<std::string, INT16>());
			//std::string navigationResPath = elementModule->GetPropertyManager(strId)->GetPropertyString(NFrame::Scene::NavigationResPath());
			//loadNavigation(sceneId, navigationResPath);
			//std::ostringstream strLog;
			//strLog << "strId: (" << strId.c_str() << ") is destroyed!\n";
			//m_pLogModule->LogInfo(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
			/*elementModule->
			int nSceneID = lexical_cast<int>(strIdList[i]);*/
		}
	}
    return true;
}

bool NFNavigationModule::AfterInit()
{
    return true;
}

NF_SHARE_PTR<NFNavigationHandle> NFNavigationModule::LoadNavigation(NFINT64 scendId, string resPath)
{
	if (resPath == "")
		return NULL;

	std::unordered_map<NFINT64, NF_SHARE_PTR<NFNavigationHandle>>::iterator iter = m_Navhandles.find(scendId);
	if (iter != m_Navhandles.end())
	{
		return iter->second;
	}

	NF_SHARE_PTR<NFNavigationHandle> pNavigationHandle = NULL;
	pNavigationHandle = NFNavigationHandle::Create(resPath);
	m_Navhandles[scendId] = pNavigationHandle;
	return pNavigationHandle;
}

bool NFNavigationModule::ExistNavigation(NFINT64 scendId)
{
	return m_Navhandles.find(scendId) != m_Navhandles.end();
}

bool NFNavigationModule::RemoveNavigation(NFINT64 scendId)
{
	std::unordered_map<NFINT64, NF_SHARE_PTR<NFNavigationHandle>>::iterator iter = m_Navhandles.find(scendId);
	if (m_Navhandles.find(scendId) != m_Navhandles.end())
	{
		m_Navhandles.erase(iter);

		std::ostringstream strLog;
		strLog << "Navigation::removeNavigation: (" << scendId << ") is destroyed!\n";
		m_pLogModule->LogInfo(strLog, __FUNCTION__, __LINE__);
		return true;
	}

	return false;
}

NF_SHARE_PTR<NFNavigationHandle> NFNavigationModule::FindNavigation(NFINT64 scendId)
{
	std::unordered_map<NFINT64, NF_SHARE_PTR<NFNavigationHandle>>::iterator iter = m_Navhandles.find(scendId);
	if (m_Navhandles.find(scendId) != m_Navhandles.end())
	{
		if (iter->second == NULL)
		{
			return NULL;
		}
		return iter->second;
	}
	return NULL;
}

int NFNavigationModule::FindPath(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths)
{
	NF_SHARE_PTR<NFNavigationHandle> pNavMeshHandle = FindNavigation(scendId);
	if (pNavMeshHandle)
	{
		return pNavMeshHandle->FindStraightPath(start, end, paths);
	}
	return 0;
}

int NFNavigationModule::FindRandomPointAroundCircle(NFINT64 scendId, const NFVector3& centerPos, std::vector<NFVector3>& points, NFINT32 max_points, float maxRadius)
{
	NF_SHARE_PTR<NFNavigationHandle> pNavMeshHandle = FindNavigation(scendId);
	if (pNavMeshHandle)
	{
		return pNavMeshHandle->FindRandomPointAroundCircle(centerPos, points, max_points, maxRadius);
	}
	return 0;
}

int NFNavigationModule::Raycast(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec)
{
	NF_SHARE_PTR<NFNavigationHandle> pNavMeshHandle = FindNavigation(scendId);
	if (pNavMeshHandle)
	{
		return pNavMeshHandle->Raycast(start, end, hitPointVec);
	}
	return 0;
}

bool NFNavigationModule::BeforeShut()
{	
    return true;
}

bool NFNavigationModule::Shut()
{
    return true;
}

bool NFNavigationModule::Execute()
{
    return true;
}