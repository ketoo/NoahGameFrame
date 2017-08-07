// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneAOIModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-05
//    @Module           :    NFCSceneAOIModule
//
// -------------------------------------------------------------------------

#include "NFCSceneAOIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCSceneAOIModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();

	m_pKernelModule->RegisterCommonClassEvent(this, &NFCSceneAOIModule::OnClassCommonEvent);
	m_pKernelModule->RegisterCommonPropertyEvent(this, &NFCSceneAOIModule::OnPropertyCommonEvent);
	m_pKernelModule->RegisterCommonRecordEvent(this, &NFCSceneAOIModule::OnRecordCommonEvent);

	//init all scene
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			int nSceneID = lexical_cast<int>(strIdList[i]);
			m_pKernelModule->CreateScene(nSceneID);
		}
	}

    return true;
}

bool NFCSceneAOIModule::AfterInit()
{
	
    return true;
}

bool NFCSceneAOIModule::BeforeShut()
{
	mtObjectEnterCallback.clear();
	mtObjectLeaveCallback.clear();
	mtPropertyEnterCallback.clear();
	mtRecordEnterCallback.clear();
	mtPropertySingleCallback.clear();
	mtRecordSingleCallback.clear();

	mtAfterEnterSceneCallback.clear();
	mtBeforeLeaveSceneCallback.clear();

    return true;
}

bool NFCSceneAOIModule::Shut()
{
    return true;
}

bool NFCSceneAOIModule::Execute()
{
    return true;
}

bool NFCSceneAOIModule::RequestEnterScene(const NFGUID & self, const int nSceneID, const int nGrupID, const int nType, const NFDataList & argList)
{
	if (nGrupID < 0)
	{
		return false;
	}

	const int nNowSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int nNowGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
	
	if (nNowSceneID == nSceneID
		&& nNowGroupID == nGrupID)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "in same scene and group but it not a clone scene", nSceneID);

		return false;
	}

	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (!pSceneInfo)
	{
		return false;
	}

	/*
	if (!pSceneInfo->ExistElement(nNewGroupID))
	{
		return false;
	}
	*/

	int nEnterConditionCode = EnterSceneCondition(self, nSceneID, nGrupID, nType, argList);
	if (nEnterConditionCode != 0)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "before enter condition code:", nEnterConditionCode);
		return false;
	}

	
	NFVector3 vRelivePos = GetRelivePosition(nSceneID, 0);
	if (!SwitchScene(self, nSceneID, nGrupID, nType, vRelivePos, 0.0f, argList))
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "SwitchScene failed", nSceneID);

		return false;
	}

	return true;
}

bool NFCSceneAOIModule::AddSeedData(const int nSceneID, const std::string & strSeedID, const std::string & strConfigID, const NFVector3 & vPos, const int nWeight)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->AddSeedObjectInfo(strSeedID, strConfigID, vPos, nWeight);
	}

	return false;
}

bool NFCSceneAOIModule::AddRelivePosition(const int nSceneID, const int nIndex, const NFVector3 & vPos)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->AddReliveInfo(nIndex, vPos);
	}

	return false;
}
NFVector3 NFCSceneAOIModule::GetRelivePosition(const int nSceneID, const int nIndex)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->GetReliveInfo(nIndex);
	}

	return NFVector3();
}
bool NFCSceneAOIModule::AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mtObjectEnterCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddObjectDataFinishedCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mtObjectDataFinishedCallBack.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR & cb)
{
	mtObjectLeaveCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mtPropertyEnterCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mtRecordEnterCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR & cb)
{
	mtPropertySingleCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR & cb)
{
	mtRecordSingleCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddEnterSceneConditionCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mtEnterSceneConditionCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddBeforeEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mtBeforeEnterSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddAfterEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mtAfterEnterSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddSwapSceneEventCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mtOnSwapSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddBeforeLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mtBeforeLeaveSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddAfterLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mtAfterLeaveSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::CreateSceneNPC(const int nSceneID, const int nGroupID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (!pSceneInfo)
	{
		return false;
	}

	//prepare monster for player
	//create monster before the player enter the scene, then we can send monster's data by one message pack
	//if you create monster after player enter scene, then send monster's data one by one
	NF_SHARE_PTR<SceneSeedResource> pResource = pSceneInfo->mtSceneResourceConfig.First();
	for (; pResource; pResource = pSceneInfo->mtSceneResourceConfig.Next())
	{
		int nWeight = m_pKernelModule->Random(0, 100);
		if (nWeight <= pResource->nWeight)
		{
			const std::string& strClassName = m_pElementModule->GetPropertyString(pResource->strConfigID, NFrame::IObject::ClassName());

			NFDataList arg;
			arg << NFrame::IObject::Position() << pResource->vSeedPos;
			arg << NFrame::NPC::SeedID() << pResource->strSeedID;

			m_pKernelModule->CreateObject(NFGUID(), nSceneID, nGroupID, strClassName, pResource->strConfigID, arg);
		}
	}

	return false;
}

bool NFCSceneAOIModule::DestroySceneNPC(const int nSceneID, const int nGroupID)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (pSceneInfo)
	{
		if (pSceneInfo->GetElement(nGroupID))
		{
			NFDataList xMonsterlistObject;
			if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, xMonsterlistObject, false))
			{
				for (int i = 0; i < xMonsterlistObject.GetCount(); ++i)
				{
					NFGUID ident = xMonsterlistObject.Object(i);
					m_pKernelModule->DestroyObject(ident);
				}
			}

			pSceneInfo->RemoveElement(nGroupID);

			return true;
		}
	}

	return false;
}

bool NFCSceneAOIModule::RemoveSwapSceneEventCallBack()
{
	mtOnSwapSceneCallback.clear();

	return true;
}

bool NFCSceneAOIModule::SwitchScene(const NFGUID& self, const int nTargetSceneID, const int nTargetGroupID, const int nType, const NFVector3 v, const float fOrient, const NFDataList& arg)
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (pObject)
	{
		int nOldSceneID = pObject->GetPropertyInt32(NFrame::Scene::SceneID());
		int nOldGroupID = pObject->GetPropertyInt32(NFrame::Scene::GroupID());

		NF_SHARE_PTR<NFCSceneInfo> pOldSceneInfo = this->GetElement(nOldSceneID);
		NF_SHARE_PTR<NFCSceneInfo> pNewSceneInfo = this->GetElement(nTargetSceneID);
		if (!pOldSceneInfo)
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this container", nOldSceneID);
			return false;
		}

		if (!pNewSceneInfo)
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this container", nTargetSceneID);
			return false;
		}

		if (!pNewSceneInfo->GetElement(nTargetGroupID))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this group", nTargetGroupID);
			return false;
		}
		/////////
		BeforeLeaveSceneGroup(self, nOldSceneID, nOldGroupID, nType, arg);

		pOldSceneInfo->RemoveObjectFromGroup(nOldGroupID, self, true);

		//if (nTargetSceneID != nOldSceneID)
		{
			pObject->SetPropertyInt(NFrame::Scene::GroupID(), 0);
			/////////
			AfterLeaveSceneGroup(self, nOldSceneID, nOldGroupID, nType, arg);

			pObject->SetPropertyInt(NFrame::Scene::SceneID(), nTargetSceneID);

			OnSwapSceneEvent(self, nTargetSceneID, nTargetGroupID, nType, arg);
		}

		pObject->SetPropertyVector3(NFrame::IObject::Position(), v);

		////////
		BeforeEnterSceneGroup(self, nTargetSceneID, nTargetGroupID, nType, arg);

		pNewSceneInfo->AddObjectToGroup(nTargetGroupID, self, true);
		pObject->SetPropertyInt(NFrame::Scene::GroupID(), nTargetGroupID);

		/////////
		AfterEnterSceneGroup(self, nTargetSceneID, nTargetGroupID, nType, arg);

		return true;
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

int NFCSceneAOIModule::OnPropertyCommonEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	const std::string& strClassName = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
	if (strClassName == NFrame::Player::ThisName())
	{
		//only player can change grupid and sceneid
		if (NFrame::Player::GroupID() == strPropertyName)
		{
			OnPlayerGroupEvent(self, strPropertyName, oldVar, newVar);
			return 0;
		}

		if (NFrame::Player::SceneID() == strPropertyName)
		{
			OnPlayerSceneEvent(self, strPropertyName, oldVar, newVar);
			return 0;
		}
	}

	NFDataList valueBroadCaseList;
	if (GetBroadCastObject(self, strPropertyName, false, valueBroadCaseList) <= 0)
	{
		return 0;
	}

	OnPropertyEvent(self, strPropertyName, oldVar, newVar, valueBroadCaseList);

	return 0;
}

int NFCSceneAOIModule::OnRecordCommonEvent(const NFGUID & self, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar)
{
	const std::string& strRecordName = xEventData.strRecordName;
	const int nOpType = xEventData.nOpType;
	const int nRow = xEventData.nRow;
	const int nCol = xEventData.nCol;

	int nObjectContainerID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());

	if (nObjectGroupID < 0)
	{
		return 0;
	}
	
	NFDataList valueBroadCaseList;
	GetBroadCastObject(self, strRecordName, true, valueBroadCaseList);

	OnRecordEvent(self, strRecordName, xEventData, oldVar, newVar, valueBroadCaseList);

	return 0;
}

int NFCSceneAOIModule::OnClassCommonEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		const int nObjectSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
		const int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::GroupID());

		if (nObjectGroupID < 0 || nObjectSceneID <= 0)
		{
			return 0;
		}

		NFDataList valueAllPlayrNoSelfList;
		m_pKernelModule->GetGroupObjectList(nObjectSceneID, nObjectGroupID, valueAllPlayrNoSelfList, true, self);

		//tell other people that you want to leave from this scene or this group
		//every one want to know you want to leave notmater you are a monster maybe you are a player
		OnObjectListLeave(valueAllPlayrNoSelfList, NFDataList() << self);
	}

	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		if (strClassName == NFrame::Player::ThisName())
		{
			//tell youself<client>, u want to enter this scene or this group
			OnObjectListEnter(NFDataList() << self, NFDataList() << self);

			//tell youself<client>, u want to broad your properties and records to youself
			OnPropertyEnter(NFDataList() << self, self);
			OnRecordEnter(NFDataList() << self, self);

			OnObjectListEnterFinished(NFDataList() << self, NFDataList() << self);
		}
		else
		{
			const int nObjectSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
			const int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::GroupID());

			if (nObjectGroupID < 0 || nObjectSceneID <= 0)
			{
				return 0;
			}

			NFDataList valueAllPlayrObjectList;
			m_pKernelModule->GetGroupObjectList(nObjectSceneID, nObjectGroupID, valueAllPlayrObjectList, true);

			//monster or others need to tell all player
			OnObjectListEnter(valueAllPlayrObjectList, NFDataList() << self);
			OnPropertyEnter(valueAllPlayrObjectList, self);

			OnObjectListEnterFinished(valueAllPlayrObjectList, NFDataList() << self);

		}
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
	}

	return 0;
}

int NFCSceneAOIModule::OnPlayerGroupEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	//this event only happened in the same scene
	int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
	int nOldGroupID = oldVar.GetInt32();
	int nNewGroupID = newVar.GetInt32();

	//maybe form 0, maybe not, only three stuation
	//example1: 0 -> 1 ==> new_group > 0 && old_group <= 0
	//example2: 1 -> 2 ==> new_group > 0 && old_group > 0
	//example3: 5 -> 0 ==> new_group <= 0 && old_group > 0
	if (nNewGroupID > 0)
	{
		if (nOldGroupID > 0)
		{
			//example2: 1 -> 2 ==> new_group > 0 && old_group > 0
			//step1: leave
			NFDataList valueAllOldNPCListNoSelf;
			NFDataList valueAllOldPlayerListNoSelf;
			m_pKernelModule->GetGroupObjectList(nSceneID, nOldGroupID, valueAllOldNPCListNoSelf, false, self);
			m_pKernelModule->GetGroupObjectList(nSceneID, nOldGroupID, valueAllOldPlayerListNoSelf, true, self);

			OnObjectListLeave(valueAllOldPlayerListNoSelf, NFDataList() << self);
			OnObjectListLeave(NFDataList() << self, valueAllOldPlayerListNoSelf);
			OnObjectListLeave(NFDataList() << self, valueAllOldNPCListNoSelf);
		}
		else
		{
			//example1: 0 -> 1 == > new_group > 0 && old_group <= 0
			//only use step2 that enough
		}

		//step2: enter
		NFDataList valueAllNewNPCListNoSelf;
		NFDataList valueAllNewPlayerListNoSelf;

		m_pKernelModule->GetGroupObjectList(nSceneID, nNewGroupID, valueAllNewNPCListNoSelf, false, self);
		m_pKernelModule->GetGroupObjectList(nSceneID, nNewGroupID, valueAllNewPlayerListNoSelf, true, self);

		OnObjectListEnter(valueAllNewPlayerListNoSelf, NFDataList() << self);
		OnObjectListEnter(NFDataList() << self, valueAllNewPlayerListNoSelf);
		OnObjectListEnter(NFDataList() << self, valueAllNewNPCListNoSelf);

		//bc others data to u
		for (int i = 0; i < valueAllNewNPCListNoSelf.GetCount(); i++)
		{
			NFGUID identOld = valueAllNewNPCListNoSelf.Object(i);

			OnPropertyEnter(NFDataList() << self, identOld);
			OnRecordEnter(NFDataList() << self, identOld);

			OnObjectListEnterFinished(NFDataList() << self, NFDataList() << identOld);
		}

		//bc others data to u
		for (int i = 0; i < valueAllNewPlayerListNoSelf.GetCount(); i++)
		{
			NFGUID identOld = valueAllNewPlayerListNoSelf.Object(i);

			OnPropertyEnter(NFDataList() << self, identOld);
			OnRecordEnter(NFDataList() << self, identOld);

			OnObjectListEnterFinished(NFDataList() << self, NFDataList() << identOld);
		}

		//bc u data to others
		OnPropertyEnter(valueAllNewPlayerListNoSelf, self);
		OnRecordEnter(valueAllNewPlayerListNoSelf, self);

		OnObjectListEnterFinished(valueAllNewPlayerListNoSelf, NFDataList() << self);
	}
	else
	{
		if (nOldGroupID > 0)
		{
			//example3: 5 -> 0 ==> new_group <= 0 && old_group > 0
			//step1: leave
			NFDataList valueAllOldNPCListNoSelf;
			NFDataList valueAllOldPlayerListNoSelf;
			m_pKernelModule->GetGroupObjectList(nSceneID, nOldGroupID, valueAllOldNPCListNoSelf, false, self);
			m_pKernelModule->GetGroupObjectList(nSceneID, nOldGroupID, valueAllOldPlayerListNoSelf, true, self);

			OnObjectListLeave(valueAllOldPlayerListNoSelf, NFDataList() << self);
			OnObjectListLeave(NFDataList() << self, valueAllOldPlayerListNoSelf);
			OnObjectListLeave(NFDataList() << self, valueAllOldNPCListNoSelf);
		}
	}

	return 0;
}

int NFCSceneAOIModule::OnPlayerSceneEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	//no more player in this group of this scene at the same time
	//so now only one player(that you) in this group of this scene
	//BTW, most of time, we dont create monsters in the group 0
	//so no one at this group but u
	
	//therefore u just need post the new scene id to you self
	OnPropertyEvent(self, strPropertyName, oldVar, newVar, NFDataList() << self);
	return 0;
}

int NFCSceneAOIModule::GetBroadCastObject(const NFGUID & self, const std::string & strPropertyName, const bool bTable, NFDataList & valueObject)
{
	int nObjectContainerID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
	int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::GroupID());

	const std::string& strClassName = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
	NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = m_pClassModule->GetClassRecordManager(strClassName);
	NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = m_pClassModule->GetClassPropertyManager(strClassName);

	NF_SHARE_PTR<NFIRecord> pRecord(NULL);
	NF_SHARE_PTR<NFIProperty> pProperty(NULL);
	if (bTable)
	{
		if (NULL == pClassRecordManager)
		{
			return -1;
		}

		pRecord = pClassRecordManager->GetElement(strPropertyName);
		if (NULL == pRecord)
		{
			return -1;
		}
	}
	else
	{
		if (NULL == pClassPropertyManager)
		{
			return -1;
		}
		pProperty = pClassPropertyManager->GetElement(strPropertyName);
		if (NULL == pProperty)
		{
			return -1;
		}
	}

	if (bTable)
	{
		//upload property can not board to itself
		if (!pRecord->GetUpload())
		{
			if (pRecord->GetPublic())
			{
				m_pKernelModule->GetGroupObjectList(nObjectContainerID, nObjectGroupID, valueObject, true, self);
			}

			if (pRecord->GetPrivate())
			{
				valueObject.Add(self);
			}
		}
	}
	else
	{
		//upload property can not board to itself
		if (!pProperty->GetUpload())
		{
			if (pProperty->GetPublic())
			{
				m_pKernelModule->GetGroupObjectList(nObjectContainerID, nObjectGroupID, valueObject, true, self);
			}

			if (pProperty->GetPrivate())
			{
				valueObject.Add(self);
			}
		}
	}

	return valueObject.GetCount();
}

int NFCSceneAOIModule::EnterSceneCondition(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mtEnterSceneConditionCallback.begin();
	for (; it != mtEnterSceneConditionCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		const int nReason = pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
		if (nReason != 0)
		{
			return nReason;
		}
	}
	return 0;
}

int NFCSceneAOIModule::AfterEnterSceneGroup(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mtAfterEnterSceneCallback.begin();
	for (; it != mtAfterEnterSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}

	return 0;
}

int NFCSceneAOIModule::BeforeLeaveSceneGroup(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mtBeforeLeaveSceneCallback.begin();
	for (; it != mtBeforeLeaveSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}

	return 0;
}

int NFCSceneAOIModule::AfterLeaveSceneGroup(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mtAfterLeaveSceneCallback.begin();
	for (; it != mtAfterLeaveSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}

	return 0;
}

int NFCSceneAOIModule::OnSwapSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mtOnSwapSceneCallback.begin();
	for (; it != mtOnSwapSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}
	return 0;
}

int NFCSceneAOIModule::BeforeEnterSceneGroup(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mtBeforeEnterSceneCallback.begin();
	for (; it != mtBeforeEnterSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}
	return 0;
}

int NFCSceneAOIModule::OnObjectListEnter(const NFDataList & self, const NFDataList & argVar)
{
	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mtObjectEnterCallback.begin();
	for (; it != mtObjectEnterCallback.end(); it++)
	{
		OBJECT_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFCSceneAOIModule::OnObjectListEnterFinished(const NFDataList & self, const NFDataList & argVar)
{
	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mtObjectDataFinishedCallBack.begin();
	for (; it != mtObjectDataFinishedCallBack.end(); it++)
	{
		OBJECT_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFCSceneAOIModule::OnObjectListLeave(const NFDataList & self, const NFDataList & argVar)
{
	std::vector<OBJECT_LEAVE_EVENT_FUNCTOR_PTR>::iterator it = mtObjectLeaveCallback.begin();
	for (; it != mtObjectLeaveCallback.end(); it++)
	{
		OBJECT_LEAVE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_LEAVE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFCSceneAOIModule::OnPropertyEnter(const NFDataList & argVar, const NFGUID & self)
{
	std::vector<PROPERTY_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mtPropertyEnterCallback.begin();
	for (; it != mtPropertyEnterCallback.end(); it++)
	{
		PROPERTY_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(argVar, self);
	}

	return 0;
}

int NFCSceneAOIModule::OnRecordEnter(const NFDataList & argVar, const NFGUID & self)
{
	std::vector<RECORD_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mtRecordEnterCallback.begin();
	for (; it != mtRecordEnterCallback.end(); it++)
	{
		RECORD_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		RECORD_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(argVar, self);
	}

	return 0;
}

int NFCSceneAOIModule::OnPropertyEvent(const NFGUID & self, const std::string & strProperty, const NFData & oldVar, const NFData & newVar, const NFDataList& argVar)
{
	std::vector<PROPERTY_SINGLE_EVENT_FUNCTOR_PTR>::iterator it = mtPropertySingleCallback.begin();
	for (; it != mtPropertySingleCallback.end(); it++)
	{
		PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_SINGLE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, strProperty, oldVar, newVar, argVar);
	}

	return 0;
}

int NFCSceneAOIModule::OnRecordEvent(const NFGUID & self, const std::string& strProperty, const RECORD_EVENT_DATA & xEventData, const NFData & oldVar, const NFData & newVar, const NFDataList& argVar)
{
	std::vector<RECORD_SINGLE_EVENT_FUNCTOR_PTR>::iterator it = mtRecordSingleCallback.begin();
	for (; it != mtRecordSingleCallback.end(); it++)
	{
		RECORD_SINGLE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		RECORD_SINGLE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, strProperty, xEventData, oldVar, newVar, argVar);
	}

	return 0;
}
