// -------------------------------------------------------------------------
//    @FileName         :    NFCNavigationModule.cpp
//    @Author           :    Bluesky
//    @Date             :    2016-06-22
//    @Module           :    NFCNavigationModule
//
// -------------------------------------------------------------------------

#include "NFCNavigationModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCNavigationModule::Init()
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
			//NFINT64 sceneId = m_pElementModule->GetPropertyManager(strId)->GetPropertyInt(NFrame::Scene::SceneID);// lexical_cast<std::string, INT16>());
			//std::string navigationResPath = m_pElementModule->GetPropertyManager(strId)->GetPropertyString(NFrame::Scene::NavigationResPath());
			//loadNavigation(sceneId, navigationResPath);
			//std::ostringstream strLog;
			//strLog << "strId: (" << strId.c_str() << ") is destroyed!\n";
			//m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
			/*m_pElementModule->
			int nSceneID = lexical_cast<int>(strIdList[i]);*/
		}
	}
    return true;
}

bool NFCNavigationModule::AfterInit()
{
    return true;
}

NF_SHARE_PTR<NFCNavigationHandle> NFCNavigationModule::LoadNavigation(NFINT64 scendId, string resPath)
{
	if (resPath == "")
		return NULL;

	std::unordered_map<NFINT64, NF_SHARE_PTR<NFCNavigationHandle>>::iterator iter = m_Navhandles.find(scendId);
	if (iter != m_Navhandles.end())
	{
		return iter->second;
	}

	NF_SHARE_PTR<NFCNavigationHandle> pNavigationHandle = NULL;
	pNavigationHandle = NFCNavigationHandle::Create(resPath);
	m_Navhandles[scendId] = pNavigationHandle;
	return pNavigationHandle;
}

bool NFCNavigationModule::ExistNavigation(NFINT64 scendId)
{
	return m_Navhandles.find(scendId) != m_Navhandles.end();
}

bool NFCNavigationModule::RemoveNavigation(NFINT64 scendId)
{
	std::unordered_map<NFINT64, NF_SHARE_PTR<NFCNavigationHandle>>::iterator iter = m_Navhandles.find(scendId);
	if (m_Navhandles.find(scendId) != m_Navhandles.end())
	{
		m_Navhandles.erase(iter);

		std::ostringstream strLog;
		strLog << "Navigation::removeNavigation: (" << scendId << ") is destroyed!\n";
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
		return true;
	}

	return false;
}

NF_SHARE_PTR<NFCNavigationHandle> NFCNavigationModule::FindNavigation(NFINT64 scendId)
{
	std::unordered_map<NFINT64, NF_SHARE_PTR<NFCNavigationHandle>>::iterator iter = m_Navhandles.find(scendId);
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

int NFCNavigationModule::FindPath(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& paths)
{
	NF_SHARE_PTR<NFCNavigationHandle> pNavMeshHandle = FindNavigation(scendId);
	if (pNavMeshHandle)
	{
		return pNavMeshHandle->FindStraightPath(start, end, paths);
	}
	return 0;
}

int NFCNavigationModule::FindRandomPointAroundCircle(NFINT64 scendId, const NFVector3& centerPos, std::vector<NFVector3>& points, NFINT32 max_points, float maxRadius)
{
	NF_SHARE_PTR<NFCNavigationHandle> pNavMeshHandle = FindNavigation(scendId);
	if (pNavMeshHandle)
	{
		return pNavMeshHandle->FindRandomPointAroundCircle(centerPos, points, max_points, maxRadius);
	}
	return 0;
}

int NFCNavigationModule::Raycast(NFINT64 scendId, const NFVector3& start, const NFVector3& end, std::vector<NFVector3>& hitPointVec)
{
	NF_SHARE_PTR<NFCNavigationHandle> pNavMeshHandle = FindNavigation(scendId);
	if (pNavMeshHandle)
	{
		return pNavMeshHandle->Raycast(start, end, hitPointVec);
	}
	return 0;
}

bool NFCNavigationModule::BeforeShut()
{	
    return true;
}

bool NFCNavigationModule::Shut()
{
    return true;
}

bool NFCNavigationModule::Execute()
{
    return true;
}