/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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

#include "NFSceneModule.h"
#include "NFComm/NFCore/NFPropertyManager.h"
#include "NFComm/NFCore/NFRecordManager.h"

bool NFSceneModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pCellModule = pPluginManager->FindModule<NFICellModule>();
	
	m_pKernelModule->RegisterCommonClassEvent(this, &NFSceneModule::OnClassCommonEvent);
	m_pKernelModule->RegisterCommonPropertyEvent(this, &NFSceneModule::OnPropertyCommonEvent);
	m_pKernelModule->RegisterCommonRecordEvent(this, &NFSceneModule::OnRecordCommonEvent);


    return true;
}

bool NFSceneModule::AfterInit()
{
	//init all scene, scene module cant create scene at Init() function as scene module depends NFIClassModule
	//and class module will load data at Init() function.
	//as a result, developer cant create game object at function AfterInit().
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			int sceneID = lexical_cast<int>(strId);
			m_pKernelModule->CreateScene(sceneID);
		}
	}
    return true;
}

bool NFSceneModule::BeforeShut()
{
	mvObjectEnterCallback.clear();
	mvObjectLeaveCallback.clear();
	mvPropertyEnterCallback.clear();
	mvRecordEnterCallback.clear();
	mvPropertySingleCallback.clear();
	mvRecordSingleCallback.clear();

	mvAfterEnterSceneCallback.clear();
	mvAfterEnterAndReadySceneCallback.clear();
	mvBeforeLeaveSceneCallback.clear();

    return true;
}

bool NFSceneModule::Shut()
{
    return true;
}

bool NFSceneModule::Execute()
{
    return true;
}

int NFSceneModule::RequestGroupScene(const int sceneID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		int nNewGroupID = pSceneInfo->NewGroupID();
		if (!pSceneInfo->GetElement(nNewGroupID))
		{
			NF_SHARE_PTR<NFIPropertyManager> pPropertyManager(NF_NEW NFPropertyManager(NFGUID(sceneID, nNewGroupID)));
			NF_SHARE_PTR<NFIRecordManager> pRecordManager(NF_NEW NFRecordManager(NFGUID(sceneID, nNewGroupID)));
			NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo(NF_NEW NFSceneGroupInfo(sceneID, nNewGroupID, pPropertyManager, pRecordManager));
			if (pGroupInfo)
			{
				NFGUID ident(sceneID, nNewGroupID);

				NF_SHARE_PTR<NFIPropertyManager> pStaticClassPropertyManager = m_pClassModule->GetClassPropertyManager(NFrame::Group::ThisName());
				NF_SHARE_PTR<NFIRecordManager> pStaticClassRecordManager = m_pClassModule->GetClassRecordManager(NFrame::Group::ThisName());
				if (pStaticClassPropertyManager && pStaticClassRecordManager)
				{
					NF_SHARE_PTR<NFIProperty> pStaticConfigPropertyInfo = pStaticClassPropertyManager->First();
					while (pStaticConfigPropertyInfo)
					{
						NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->AddProperty(ident, pStaticConfigPropertyInfo->GetKey(), pStaticConfigPropertyInfo->GetType());

						xProperty->SetPublic(pStaticConfigPropertyInfo->GetPublic());
						xProperty->SetPrivate(pStaticConfigPropertyInfo->GetPrivate());
						xProperty->SetSave(pStaticConfigPropertyInfo->GetSave());
						xProperty->SetCache(pStaticConfigPropertyInfo->GetCache());
						xProperty->SetRef(pStaticConfigPropertyInfo->GetRef());
						xProperty->SetUpload(pStaticConfigPropertyInfo->GetUpload());

						PROPERTY_EVENT_FUNCTOR functor = std::bind(&NFSceneModule::OnScenePropertyCommonEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
						PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
						pPropertyManager->RegisterCallback(pStaticConfigPropertyInfo->GetKey(), functorPtr);

						pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
					}

					NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
					while (pConfigRecordInfo)
					{
						NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->AddRecord(ident,
							pConfigRecordInfo->GetName(),
							pConfigRecordInfo->GetInitData(),
							pConfigRecordInfo->GetTag(),
							pConfigRecordInfo->GetRows());

						xRecord->SetPublic(pConfigRecordInfo->GetPublic());
						xRecord->SetPrivate(pConfigRecordInfo->GetPrivate());
						xRecord->SetSave(pConfigRecordInfo->GetSave());
						xRecord->SetCache(pConfigRecordInfo->GetCache());
						xRecord->SetUpload(pConfigRecordInfo->GetUpload());

						RECORD_EVENT_FUNCTOR functor = std::bind(&NFSceneModule::OnSceneRecordCommonEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
						RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));

						xRecord->AddRecordHook(functorPtr);

						pConfigRecordInfo = pStaticClassRecordManager->Next();
					}
				}

				pSceneInfo->AddElement(nNewGroupID, pGroupInfo);

				int sceneType = m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());
				SceneGroupCreatedEvent(NFGUID(), sceneID, nNewGroupID, sceneType, NFDataList::Empty());

				m_pCellModule->CreateGroupCell(sceneID, nNewGroupID);
				return nNewGroupID;
			}
		}
	}

	return 0;
}

bool NFSceneModule::RequestEnterScene(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFVector3& pos, const NFDataList & argList)
{
	if (groupID < 0)
	{
		return false;
	}

	const int nNowSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int nNowGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
	
	if (nNowSceneID == sceneID
		&& nNowGroupID == groupID)
	{
		m_pLogModule->LogInfo(self, "in same scene and group but it not a clone scene " + std::to_string(sceneID));

		return false;
	}

	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
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

	int nEnterConditionCode = EnterSceneCondition(self, sceneID, groupID, type, argList);
	if (nEnterConditionCode != 0)
	{
		m_pLogModule->LogInfo(self, "before enter condition code: " + std::to_string(nEnterConditionCode), __FUNCTION__, __LINE__);
		return false;
	}

	if (!SwitchScene(self, sceneID, groupID, type, pos, 0.0f, argList))
	{
		m_pLogModule->LogInfo(self, "SwitchScene failed " + std::to_string(sceneID));

		return false;
	}

	return true;
}

bool NFSceneModule::ReleaseGroupScene(const int sceneID, const int groupID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		if (groupID > 0)
		{
			DestroySceneNPC(sceneID, groupID);

			m_pCellModule->DestroyGroupCell(sceneID, groupID);

			int sceneType = m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());
			SceneGroupDestroyedEvent(NFGUID(), sceneID, groupID, sceneType, NFDataList::Empty());

			pSceneInfo->RemoveElement(groupID);

			return true;
		}
	}

	return false;
}

bool NFSceneModule::LeaveSceneGroup(const NFGUID & self)
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (pObject)
	{
		int nOldSceneID = pObject->GetPropertyInt32(NFrame::Scene::SceneID());
		int nOldGroupID = pObject->GetPropertyInt32(NFrame::Scene::GroupID());
		if (nOldGroupID <= 0)
		{
			//m_pLogModule->LogError(self, "no this group == 0 " + std::to_string(nOldSceneID), __FUNCTION__, __LINE__);
			//return false;
		}

		NF_SHARE_PTR<NFSceneInfo> pOldSceneInfo = this->GetElement(nOldSceneID);
		if (!pOldSceneInfo)
		{
			m_pLogModule->LogError(self, "no this container " + std::to_string(nOldSceneID), __FUNCTION__, __LINE__);
			return false;
		}

		if (!pOldSceneInfo->GetElement(nOldGroupID))
		{
			m_pLogModule->LogError(self, "no this group " + std::to_string(nOldGroupID), __FUNCTION__, __LINE__);
			return false;
		}
		/////////

		const NFVector3& lastPos = m_pKernelModule->GetPropertyVector3(self, NFrame::IObject::Position());
		BeforeLeaveSceneGroup(self, nOldSceneID, nOldGroupID, 0, NFDataList::Empty());

		const NFGUID lastCell = m_pCellModule->ComputeCellID(lastPos);
		OnMoveCellEvent(self, nOldSceneID, nOldGroupID, lastCell, NFGUID());

		pOldSceneInfo->RemoveObjectFromGroup(nOldGroupID, self, true);

		//if (nTargetSceneID != nOldSceneID)
		{
			pObject->SetPropertyInt(NFrame::Scene::GroupID(), 0);
			/////////
			AfterLeaveSceneGroup(self, nOldSceneID, nOldGroupID, 0, NFDataList::Empty());
		}

		return true;
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);


	return false;
}

const std::vector<int>& NFSceneModule::GetGroups(const int sceneID)
{
	static std::vector<int> vec;
	vec.clear();

	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo =  pSceneInfo->First();
		while (pGroupInfo)
		{
			vec.push_back(pGroupInfo->groupID);

			pGroupInfo =  pSceneInfo->Next();
		}
	}

	return vec;
}

bool NFSceneModule::AddSeedData(const int sceneID, const std::string & seedID, const std::string & configID, const NFVector3 & vPos, const int nWeight)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->AddSeedObjectInfo(seedID, configID, vPos, nWeight);
	}

	return false;
}

const NFVector3& NFSceneModule::GetSeedPos(const int sceneID, const std::string& seedID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		auto seedDnata = pSceneInfo->GetSeedObjectInfo(seedID);
		if (seedDnata)
		{
			return seedDnata->vSeedPos;
		}
	}

	return NFVector3::Zero();
}

const int NFSceneModule::GetSeedPWeight(const int sceneID, const std::string& seedID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		auto seedDnata = pSceneInfo->GetSeedObjectInfo(seedID);
		if (seedDnata)
		{
			return seedDnata->nWeight;
		}
	}

	return 0;
}

bool NFSceneModule::AddRelivePosition(const int sceneID, const int nIndex, const NFVector3 & vPos)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->AddReliveInfo(nIndex, vPos);
	}

	return false;
}

const NFVector3& NFSceneModule::GetRelivePosition(const int sceneID, const int nIndex)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->GetReliveInfo(nIndex);
	}

	return NFVector3::Zero();
}

bool NFSceneModule::AddTagPosition(const int sceneID, const int nIndex, const NFVector3 & vPos)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->AddTagInfo(nIndex, vPos);
	}

	return false;
}

const NFVector3& NFSceneModule::GetTagPosition(const int sceneID, const int nIndex)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->GetTagInfo(nIndex);
	}

	return NFVector3::Zero();
}

bool NFSceneModule::AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mvObjectEnterCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddObjectDataFinishedCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mvObjectDataFinishedCallBack.push_back(cb);
	return true;
}

bool NFSceneModule::AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR & cb)
{
	mvObjectLeaveCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mvPropertyEnterCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mvRecordEnterCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR & cb)
{
	mvPropertySingleCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR & cb)
{
	mvRecordSingleCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddEnterSceneConditionCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mvEnterSceneConditionCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddBeforeEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mvBeforeEnterSceneCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddAfterEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mvAfterEnterSceneCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddSwapSceneEventCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mvOnSwapSceneCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddBeforeLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mvBeforeLeaveSceneCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddAfterLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mvAfterLeaveSceneCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddSceneGroupCreatedCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{

	mvSceneGroupCreatedCallback.push_back(cb);
	return true;
}

bool NFSceneModule::AddSceneGroupDestroyedCallBack(const SCENE_EVENT_FUNCTOR_PTR & cb)
{
	mvSceneGroupDestroyedCallback.push_back(cb);
	return true;
}

bool NFSceneModule::CreateSceneNPC(const int sceneID, const int groupID)
{
	return CreateSceneNPC(sceneID, groupID, NFDataList::Empty());
}

bool NFSceneModule::CreateSceneNPC(const int sceneID, const int groupID, const NFDataList& argList)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (!pSceneInfo)
	{
		return false;
	}

	NF_SHARE_PTR<SceneSeedResource> pResource = pSceneInfo->mtSceneResourceConfig.First();
	for (; pResource; pResource = pSceneInfo->mtSceneResourceConfig.Next())
	{
		int nWeight = m_pKernelModule->Random(0, 100);
		if (nWeight <= pResource->nWeight)
		{
			NFDataList arg;
			arg << NFrame::IObject::Position() << pResource->vSeedPos;
			arg << NFrame::NPC::SeedID() << pResource->seedID;
			arg.Append(argList);

			m_pKernelModule->CreateObject(NFGUID(), sceneID, groupID, NFrame::NPC::ThisName(), pResource->configID, arg);
		}
	}

	return false;
}

bool NFSceneModule::DestroySceneNPC(const int sceneID, const int groupID)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(sceneID);
	if (pSceneInfo)
	{
		if (pSceneInfo->GetElement(groupID))
		{
			NFDataList xMonsterlistObject;
			if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, xMonsterlistObject, false))
			{
				for (int i = 0; i < xMonsterlistObject.GetCount(); ++i)
				{
					NFGUID ident = xMonsterlistObject.Object(i);
					m_pKernelModule->DestroyObject(ident);
				}
			}

			pSceneInfo->RemoveElement(groupID);

			return true;
		}
	}

	return false;
}

bool NFSceneModule::RemoveSwapSceneEventCallBack()
{
	mvOnSwapSceneCallback.clear();

	return true;
}

bool NFSceneModule::SetPropertyInt(const int scene, const int group, const std::string & propertyName, const NFINT64 nValue)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->SetPropertyInt(propertyName, nValue);
		}
	}

	return false;
}

bool NFSceneModule::SetPropertyFloat(const int scene, const int group, const std::string & propertyName, const double dValue)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->SetPropertyFloat(propertyName, dValue);
		}
	}

	return false;
}

bool NFSceneModule::SetPropertyString(const int scene, const int group, const std::string & propertyName, const std::string & value)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->SetPropertyString(propertyName, value);
		}
	}

	return false;
}

bool NFSceneModule::SetPropertyObject(const int scene, const int group, const std::string & propertyName, const NFGUID & objectValue)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->SetPropertyObject(propertyName, objectValue);
		}
	}

	return false;
}

bool NFSceneModule::SetPropertyVector2(const int scene, const int group, const std::string & propertyName, const NFVector2 & value)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->SetPropertyVector2(propertyName, value);
		}
	}

	return false;
}

bool NFSceneModule::SetPropertyVector3(const int scene, const int group, const std::string & propertyName, const NFVector3 & value)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->SetPropertyVector3(propertyName, value);
		}
	}

	return false;
}

NFINT64 NFSceneModule::GetPropertyInt(const int scene, const int group, const std::string & propertyName)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->GetPropertyInt(propertyName);
		}
	}

	return 0;
}

int NFSceneModule::GetPropertyInt32(const int scene, const int group, const std::string & propertyName)
{
	return (int)GetPropertyInt(scene, group, propertyName);
}

double NFSceneModule::GetPropertyFloat(const int scene, const int group, const std::string & propertyName)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->GetPropertyFloat(propertyName);
		}
	}

	return 0.0;
}

const std::string & NFSceneModule::GetPropertyString(const int scene, const int group, const std::string & propertyName)
{

	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->GetPropertyString(propertyName);
		}
	}

	static std::string emptyString("");
	return emptyString;
}

const NFGUID & NFSceneModule::GetPropertyObject(const int scene, const int group, const std::string & propertyName)
{

	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->GetPropertyObject(propertyName);
		}
	}

	return NULL_OBJECT;
}

const NFVector2 & NFSceneModule::GetPropertyVector2(const int scene, const int group, const std::string & propertyName)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->GetPropertyVector2(propertyName);
		}
	}

	return NFVector2::Zero();
}

const NFVector3 & NFSceneModule::GetPropertyVector3(const int scene, const int group, const std::string & propertyName)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager->GetPropertyVector3(propertyName);
		}
	}

	return NFVector3::Zero();
}

NF_SHARE_PTR<NFIPropertyManager> NFSceneModule::FindPropertyManager(const int scene, const int group)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxPropertyManager;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRecordManager> NFSceneModule::FindRecordManager(const int scene, const int group)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxRecordManager;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRecord> NFSceneModule::FindRecord(const int scene, const int group, const std::string & recordName)
{
	NF_SHARE_PTR<NFSceneInfo> pSceneInfo = GetElement(scene);
	if (pSceneInfo)
	{
		NF_SHARE_PTR<NFSceneGroupInfo> pGroupInfo = pSceneInfo->GetElement(group);
		if (pGroupInfo)
		{
			return pGroupInfo->mxRecordManager->GetElement(recordName);
		}
	}

	return nullptr;
}

bool NFSceneModule::ClearRecord(const int scene, const int group, const std::string & recordName)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->Clear();
	}

	return false;
}

bool NFSceneModule::SetRecordInt(const int scene, const int group, const std::string & recordName, const int row, const int col, const NFINT64 nValue)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetInt(row, col, nValue);
	}

	return false;
}

bool NFSceneModule::SetRecordFloat(const int scene, const int group, const std::string & recordName, const int row, const int col, const double dwValue)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetFloat(row, col, dwValue);
	}

	return false;
}

bool NFSceneModule::SetRecordString(const int scene, const int group, const std::string & recordName, const int row, const int col, const std::string & value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetString(row, col, value);
	}

	return false;
}

bool NFSceneModule::SetRecordObject(const int scene, const int group, const std::string & recordName, const int row, const int col, const NFGUID & objectValue)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetObject(row, col, objectValue);
	}

	return false;
}

bool NFSceneModule::SetRecordVector2(const int scene, const int group, const std::string & recordName, const int row, const int col, const NFVector2 & value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetVector2(row, col, value);
	}

	return false;
}

bool NFSceneModule::SetRecordVector3(const int scene, const int group, const std::string & recordName, const int row, const int col, const NFVector3 & value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetVector3(row, col, value);
	}

	return false;
}

bool NFSceneModule::SetRecordInt(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag, const NFINT64 value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetInt(row, colTag, value);
	}

	return false;
}

bool NFSceneModule::SetRecordFloat(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag, const double value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetFloat(row, colTag, value);
	}

	return false;
}

bool NFSceneModule::SetRecordString(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag, const std::string & value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetString(row, colTag, value);
	}

	return false;
}

bool NFSceneModule::SetRecordObject(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag, const NFGUID & value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetObject(row, colTag, value);
	}

	return false;
}

bool NFSceneModule::SetRecordVector2(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag, const NFVector2 & value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetVector2(row, colTag, value);
	}

	return false;
}

bool NFSceneModule::SetRecordVector3(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag, const NFVector3 & value)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->SetVector3(row, colTag, value);
	}

	return false;
}

NFINT64 NFSceneModule::GetRecordInt(const int scene, const int group, const std::string & recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetInt(row, col);
	}

	return 0;
}

double NFSceneModule::GetRecordFloat(const int scene, const int group, const std::string & recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetFloat(row, col);
	}

	return 0.0;
}

const std::string & NFSceneModule::GetRecordString(const int scene, const int group, const std::string & recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetString(row, col);
	}

	return NULL_STR;
}

const NFGUID & NFSceneModule::GetRecordObject(const int scene, const int group, const std::string & recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetObject(row, col);
	}

	return NULL_OBJECT;
}

const NFVector2 & NFSceneModule::GetRecordVector2(const int scene, const int group, const std::string & recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetVector2(row, col);
	}

	return NFVector2::Zero();
}

const NFVector3 & NFSceneModule::GetRecordVector3(const int scene, const int group, const std::string & recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetVector3(row, col);
	}

	return NFVector3::Zero();
}

NFINT64 NFSceneModule::GetRecordInt(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetInt(row, colTag);
	}

	return 0;
}

double NFSceneModule::GetRecordFloat(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetFloat(row, colTag);
	}

	return 0.0;
}

const std::string & NFSceneModule::GetRecordString(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetString(row, colTag);
	}

	return NULL_STR;
}

const NFGUID & NFSceneModule::GetRecordObject(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetObject(row, colTag);
	}

	return NULL_OBJECT;
}

const NFVector2 & NFSceneModule::GetRecordVector2(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetVector2(row, colTag);
	}

	return NFVector2::Zero();
}

const NFVector3 & NFSceneModule::GetRecordVector3(const int scene, const int group, const std::string & recordName, const int row, const std::string & colTag)
{
	NF_SHARE_PTR<NFIRecord> xRecord = FindRecord(scene, group, recordName);
	if (xRecord)
	{
		return xRecord->GetVector3(row, colTag);
	}

	return NFVector3::Zero();
}

bool NFSceneModule::AddGroupPropertyCallBack(const std::string& name, const PROPERTY_EVENT_FUNCTOR_PTR & cb)
{
	if (mtGroupPropertyCallBackList.find(name) == mtGroupPropertyCallBackList.end())
	{
		std::list<PROPERTY_EVENT_FUNCTOR_PTR> xList;
		xList.push_back(cb);

		mtGroupPropertyCallBackList.insert(std::map< std::string, std::list<PROPERTY_EVENT_FUNCTOR_PTR>>::value_type(name, xList));

		return true;
	}


	std::map< std::string, std::list<PROPERTY_EVENT_FUNCTOR_PTR>>::iterator it = mtGroupPropertyCallBackList.find(name);
	it->second.push_back(cb);

	return true;
}

bool NFSceneModule::AddGroupRecordCallBack(const std::string& name, const RECORD_EVENT_FUNCTOR_PTR & cb)
{
	if (mtGroupRecordCallBackList.find(name) == mtGroupRecordCallBackList.end())
	{
		std::list<RECORD_EVENT_FUNCTOR_PTR> xList;
		xList.push_back(cb);

		mtGroupRecordCallBackList.insert(std::map< std::string, std::list<RECORD_EVENT_FUNCTOR_PTR>>::value_type(name, xList));

		return true;
	}


	std::map< std::string, std::list<RECORD_EVENT_FUNCTOR_PTR>>::iterator it = mtGroupRecordCallBackList.find(name);
	it->second.push_back(cb);

	return true;
}

bool NFSceneModule::AddGroupPropertyCommCallBack(const PROPERTY_EVENT_FUNCTOR_PTR & cb)
{
	mtGroupPropertyCommCallBackList.push_back(cb);

	return true;
}

bool NFSceneModule::AddGroupRecordCommCallBack(const RECORD_EVENT_FUNCTOR_PTR & cb)
{
	mtGroupRecordCallCommBackList.push_back(cb);

	return true;
}

bool NFSceneModule::SwitchScene(const NFGUID& self, const int nTargetSceneID, const int nTargetGroupID, const int type, const NFVector3 v, const float fOrient, const NFDataList& arg)
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (pObject)
	{
		int nOldSceneID = pObject->GetPropertyInt32(NFrame::Scene::SceneID());
		int nOldGroupID = pObject->GetPropertyInt32(NFrame::Scene::GroupID());

		NF_SHARE_PTR<NFSceneInfo> pOldSceneInfo = this->GetElement(nOldSceneID);
		NF_SHARE_PTR<NFSceneInfo> pNewSceneInfo = this->GetElement(nTargetSceneID);
		if (!pOldSceneInfo)
		{
			m_pLogModule->LogError(self, "no this container " + std::to_string(nOldSceneID), __FUNCTION__, __LINE__);
			return false;
		}

		if (!pNewSceneInfo)
		{
			m_pLogModule->LogError(self, "no this container " + std::to_string(nTargetSceneID), __FUNCTION__, __LINE__);
			return false;
		}

		if (!pNewSceneInfo->GetElement(nTargetGroupID))
		{
			m_pLogModule->LogError(self, "no this group " + std::to_string(nTargetGroupID), __FUNCTION__, __LINE__);
			return false;
		}
		/////////

		const NFVector3& lastPos = m_pKernelModule->GetPropertyVector3(self, NFrame::IObject::Position());
		BeforeLeaveSceneGroup(self, nOldSceneID, nOldGroupID, type, arg);

		const NFGUID lastCell = m_pCellModule->ComputeCellID(lastPos);
		OnMoveCellEvent(self, nOldSceneID, nOldGroupID, lastCell, NFGUID());

		pOldSceneInfo->RemoveObjectFromGroup(nOldGroupID, self, true);

		//if (nTargetSceneID != nOldSceneID)
		{
			pObject->SetPropertyInt(NFrame::Scene::GroupID(), 0);
			/////////
			AfterLeaveSceneGroup(self, nOldSceneID, nOldGroupID, type, arg);

			pObject->SetPropertyInt(NFrame::Scene::SceneID(), nTargetSceneID);

			OnSwapSceneEvent(self, nTargetSceneID, nTargetGroupID, type, arg);
		}


		////////
		BeforeEnterSceneGroup(self, nTargetSceneID, nTargetGroupID, type, arg);

		pNewSceneInfo->AddObjectToGroup(nTargetGroupID, self, true);
		pObject->SetPropertyInt(NFrame::Scene::GroupID(), nTargetGroupID);


		const NFGUID newCell = m_pCellModule->ComputeCellID(v);
		OnMoveCellEvent(self, nTargetGroupID, nTargetGroupID, NFGUID(), newCell);

		pObject->SetPropertyVector3(NFrame::IObject::MoveTo(), v);

		/////////
		AfterEnterSceneGroup(self, nTargetSceneID, nTargetGroupID, type, arg);

		AfterEnterAndReadySceneGroup(self, nTargetSceneID, nTargetGroupID, type, arg);

		return true;
	}

	m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

	return false;
}

int NFSceneModule::OnScenePropertyCommonEvent(const NFGUID & self, const std::string & propertyName, const NFData & oldVar, const NFData & newVar, const NFINT64 reason)
{
	auto itList = mtGroupPropertyCommCallBackList.begin();
	for (; itList != mtGroupPropertyCommCallBackList.end(); itList++)
	{
		PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *itList;
		PROPERTY_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, propertyName, oldVar, newVar, reason);
	}

	auto it = mtGroupPropertyCallBackList.find(propertyName);
	if (it != mtGroupPropertyCallBackList.end())
	{
		auto itList = it->second.begin();
		for (; itList != it->second.end(); itList++)
		{
			PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *itList;
			PROPERTY_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(self, propertyName, oldVar, newVar, reason);
		}
	}

	return 0;
}

int NFSceneModule::OnSceneRecordCommonEvent(const NFGUID & self, const RECORD_EVENT_DATA & eventData, const NFData & oldVar, const NFData & newVar)
{
	auto itList = mtGroupRecordCallCommBackList.begin();
	for (; itList != mtGroupRecordCallCommBackList.end(); itList++)
	{
		RECORD_EVENT_FUNCTOR_PTR& pFunPtr = *itList;
		RECORD_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, eventData, oldVar, newVar);
	}

	auto it = mtGroupRecordCallBackList.find(eventData.recordName);
	if (it != mtGroupRecordCallBackList.end())
	{
		auto itList = it->second.begin();
		for (; itList != it->second.end(); itList++)
		{
			RECORD_EVENT_FUNCTOR_PTR& pFunPtr = *itList;
			RECORD_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(self, eventData, oldVar, newVar);
		}
	}

	return 0;
}

int NFSceneModule::OnPropertyCommonEvent(const NFGUID & self, const std::string & propertyName, const NFData & oldVar, const NFData & newVar, const NFINT64 reason)
{
	const std::string& className = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
	if (className == NFrame::Player::ThisName())
	{
		//only player can change grupid and sceneid
		if (NFrame::Player::GroupID() == propertyName)
		{
			OnPlayerGroupEvent(self, propertyName, oldVar, newVar);
			return 0;
		}

		if (NFrame::Player::SceneID() == propertyName)
		{
			OnPlayerSceneEvent(self, propertyName, oldVar, newVar);
			return 0;
		}
	}

	NFDataList valueBroadCaseList;
	if (GetBroadCastObject(self, propertyName, false, valueBroadCaseList) <= 0)
	{
		return 0;
	}

	OnPropertyEvent(self, propertyName, oldVar, newVar, valueBroadCaseList, reason);

	return 0;
}

int NFSceneModule::OnRecordCommonEvent(const NFGUID & self, const RECORD_EVENT_DATA & eventData, const NFData & oldVar, const NFData & newVar)
{
	const std::string& recordName = eventData.recordName;

	int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());

	if (nObjectGroupID < 0)
	{
		return 0;
	}
	
	NFDataList valueBroadCaseList;
	GetBroadCastObject(self, recordName, true, valueBroadCaseList);

	OnRecordEvent(self, recordName, eventData, oldVar, newVar, valueBroadCaseList);

	return 0;
}

int NFSceneModule::OnClassCommonEvent(const NFGUID & self, const std::string & className, const CLASS_OBJECT_EVENT classEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == classEvent)
	{
		const int nObjectSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
		const int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::GroupID());

		if (nObjectGroupID < 0 || nObjectSceneID <= 0)
		{
			return 0;
		}

		const NFVector3& pos = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::Position());
		NFDataList valueAllPlayrNoSelfList;
		m_pCellModule->GetCellObjectList(nObjectSceneID, nObjectGroupID, pos, valueAllPlayrNoSelfList, true, self);

		//tell other people that you want to leave from this scene or this group
		//every one want to know you want to leave notmater you are a monster maybe you are a player
		OnObjectListLeave(valueAllPlayrNoSelfList, NFDataList() << self);
	}

	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == classEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == classEvent)
	{
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == classEvent)
	{
		NFDataList selfVar;
		selfVar << self;

		if (className == NFrame::Player::ThisName())
		{
			//tell youself<client>, u want to enter this scene or this group
			OnObjectListEnter(selfVar, selfVar);

			//tell youself<client>, u want to broad your properties and records to youself
			OnPropertyEnter(selfVar, self);
			OnRecordEnter(selfVar, self);

			OnObjectListEnterFinished(selfVar, selfVar);
		}
		else
		{
			const int nObjectSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
			const int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::GroupID());

			if (nObjectGroupID < 0 || nObjectSceneID <= 0)
			{
				return 0;
			}
			const NFVector3& pos = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::Position());
			NFDataList valueAllPlayrObjectList;
			m_pCellModule->GetCellObjectList(nObjectSceneID, nObjectGroupID, pos, valueAllPlayrObjectList, true);

			//monster or others need to tell all player
			OnObjectListEnter(valueAllPlayrObjectList, selfVar);
			OnPropertyEnter(valueAllPlayrObjectList, self);
			OnRecordEnter(valueAllPlayrObjectList, self);

			OnObjectListEnterFinished(valueAllPlayrObjectList, selfVar);

		}
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == classEvent)
	{
	}

	return 0;
}

int NFSceneModule::OnPlayerGroupEvent(const NFGUID & self, const std::string & propertyName, const NFData & oldVar, const NFData & newVar)
{
	//this event only happened in the same scene
	const int sceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
	const NFVector3 position = m_pKernelModule->GetPropertyVector3(self, NFrame::IObject::Position());
	int nOldGroupID = oldVar.GetInt32();
	int nNewGroupID = newVar.GetInt32();

	//maybe form 0, maybe not, only three stuation
	//example1: 0 -> 1 ==> new_group > 0 && old_group <= 0
	//example2: 1 -> 2 ==> new_group > 0 && old_group > 0
	//example3: 5 -> 0 ==> new_group <= 0 && old_group > 0


	NFDataList selfVar;
	selfVar << self;

	if (nNewGroupID > 0)
	{
		if (nOldGroupID > 0)
		{
			//example2: 1 -> 2 ==> new_group > 0 && old_group > 0
			//step1: leave
			NFDataList valueAllOldNPCListNoSelf;
			NFDataList valueAllOldPlayerListNoSelf;
			m_pCellModule->GetCellObjectList(sceneID, nOldGroupID, position, valueAllOldNPCListNoSelf, false, self);
			m_pCellModule->GetCellObjectList(sceneID, nOldGroupID, position, valueAllOldPlayerListNoSelf, true, self);

			OnObjectListLeave(valueAllOldPlayerListNoSelf, selfVar);
			OnObjectListLeave(selfVar, valueAllOldPlayerListNoSelf);
			OnObjectListLeave(selfVar, valueAllOldNPCListNoSelf);
		}
		else
		{
			//example1: 0 -> 1 == > new_group > 0 && old_group <= 0
			//only use step2 that enough
		}

		//step2: enter
		NFDataList valueAllNewNPCListNoSelf;
		NFDataList valueAllNewPlayerListNoSelf;

		m_pCellModule->GetCellObjectList(sceneID, nNewGroupID, position, valueAllNewNPCListNoSelf, false, self);
		m_pCellModule->GetCellObjectList(sceneID, nNewGroupID, position, valueAllNewPlayerListNoSelf, true, self);

		OnObjectListEnter(valueAllNewPlayerListNoSelf, selfVar);
		OnObjectListEnter(selfVar, valueAllNewPlayerListNoSelf);
		OnObjectListEnter(selfVar, valueAllNewNPCListNoSelf);

		//bc others data to u
		NFDataList identOldVar;
		identOldVar.Add(NFGUID());

		for (int i = 0; i < valueAllNewNPCListNoSelf.GetCount(); i++)
		{
			NFGUID identOld = valueAllNewNPCListNoSelf.Object(i);
			identOldVar.SetObject(0, identOld);

			OnPropertyEnter(selfVar, identOld);
			OnRecordEnter(selfVar, identOld);

			OnObjectListEnterFinished(selfVar, identOldVar);
		}

		//bc others data to u
		for (int i = 0; i < valueAllNewPlayerListNoSelf.GetCount(); i++)
		{
			NFGUID identOld = valueAllNewPlayerListNoSelf.Object(i);
			identOldVar.SetObject(0, identOld);

			OnPropertyEnter(selfVar, identOld);
			OnRecordEnter(selfVar, identOld);

			OnObjectListEnterFinished(selfVar, identOldVar);
		}

		//bc u data to others
		OnPropertyEnter(valueAllNewPlayerListNoSelf, self);
		OnRecordEnter(valueAllNewPlayerListNoSelf, self);

		OnObjectListEnterFinished(valueAllNewPlayerListNoSelf, selfVar);
	}
	else
	{
		if (nOldGroupID > 0)
		{
			//example3: 5 -> 0 ==> new_group <= 0 && old_group > 0
			//step1: leave
			NFDataList valueAllOldNPCListNoSelf;
			NFDataList valueAllOldPlayerListNoSelf;
			m_pCellModule->GetCellObjectList(sceneID, nOldGroupID, position, valueAllOldNPCListNoSelf, false, self);
			m_pCellModule->GetCellObjectList(sceneID, nOldGroupID, position, valueAllOldPlayerListNoSelf, true, self);

			OnObjectListLeave(valueAllOldPlayerListNoSelf, selfVar);
			OnObjectListLeave(selfVar, valueAllOldPlayerListNoSelf);
			OnObjectListLeave(selfVar, valueAllOldNPCListNoSelf);
		}
	}

	return 0;
}

int NFSceneModule::OnPlayerSceneEvent(const NFGUID & self, const std::string & propertyName, const NFData & oldVar, const NFData & newVar)
{
	//no more player in this group of this scene at the same time
	//so now only one player(that you) in this group of this scene
	//BTW, most of time, we dont create monsters in the group 0
	//so no one at this group but u
	
	//therefore u just need post the new scene id to you self
	OnPropertyEvent(self, propertyName, oldVar, newVar, NFDataList() << self, 0);
	return 0;
}

int NFSceneModule::GetBroadCastObject(const NFGUID & self, const std::string & propertyName, const bool bTable, NFDataList & valueObject)
{
	const int nObjectContainerID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::SceneID());
	const int nObjectGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::IObject::GroupID());
	const NFVector3& position = m_pKernelModule->GetPropertyVector3(self, NFrame::IObject::Position());

	const std::string& className = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
	NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = m_pClassModule->GetClassRecordManager(className);
	NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = m_pClassModule->GetClassPropertyManager(className);

	NF_SHARE_PTR<NFIRecord> pRecord(NULL);
	NF_SHARE_PTR<NFIProperty> pProperty(NULL);
	if (bTable)
	{
		if (NULL == pClassRecordManager)
		{
			return -1;
		}

		pRecord = pClassRecordManager->GetElement(propertyName);
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
		pProperty = pClassPropertyManager->GetElement(propertyName);
		if (NULL == pProperty)
		{
			return -1;
		}
	}

	if (bTable)
	{
		//upload property can not board to itself
		//if (pRecord->GetUpload())
		{
			if (pRecord->GetPublic())
			{
				m_pCellModule->GetCellObjectList(nObjectContainerID, nObjectGroupID, position, valueObject, true, self);
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
		//if (pProperty->GetUpload())
		{
			if (pProperty->GetPublic())
			{
				m_pCellModule->GetCellObjectList(nObjectContainerID, nObjectGroupID, position, valueObject, true, self);
			}

			if (pProperty->GetPrivate())
			{
				valueObject.Add(self);
			}
		}
	}

	return valueObject.GetCount();
}

int NFSceneModule::EnterSceneCondition(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvEnterSceneConditionCallback.begin();
	for (; it != mvEnterSceneConditionCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		const int nReason = pFunc->operator()(self, sceneID, groupID, type, argList);
		if(nReason != 0) return nReason;
	}
	return 0;
}

int NFSceneModule::AfterEnterSceneGroup(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvAfterEnterSceneCallback.begin();
	for (; it != mvAfterEnterSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}

	return 0;
}

int NFSceneModule::SceneGroupCreatedEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvSceneGroupCreatedCallback.begin();
	for (; it != mvSceneGroupCreatedCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}

	return 0;
}

int NFSceneModule::SceneGroupDestroyedEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvSceneGroupDestroyedCallback.begin();
	for (; it != mvSceneGroupDestroyedCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}

	return 0;
}

int NFSceneModule::BeforeLeaveSceneGroup(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvBeforeLeaveSceneCallback.begin();
	for (; it != mvBeforeLeaveSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}

	return 0;
}

int NFSceneModule::AfterLeaveSceneGroup(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvAfterLeaveSceneCallback.begin();
	for (; it != mvAfterLeaveSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}

	return 0;
}

int NFSceneModule::OnSwapSceneEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvOnSwapSceneCallback.begin();
	for (; it != mvOnSwapSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}
	return 0;
}

int NFSceneModule::BeforeEnterSceneGroup(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{

	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvBeforeEnterSceneCallback.begin();
	for (; it != mvBeforeEnterSceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}
	return 0;
}

int NFSceneModule::OnObjectListEnter(const NFDataList & self, const NFDataList & argVar)
{
	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mvObjectEnterCallback.begin();
	for (; it != mvObjectEnterCallback.end(); it++)
	{
		OBJECT_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFSceneModule::OnObjectListEnterFinished(const NFDataList & self, const NFDataList & argVar)
{
	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mvObjectDataFinishedCallBack.begin();
	for (; it != mvObjectDataFinishedCallBack.end(); it++)
	{
		OBJECT_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFSceneModule::OnObjectListLeave(const NFDataList & self, const NFDataList & argVar)
{
	std::vector<OBJECT_LEAVE_EVENT_FUNCTOR_PTR>::iterator it = mvObjectLeaveCallback.begin();
	for (; it != mvObjectLeaveCallback.end(); it++)
	{
		OBJECT_LEAVE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_LEAVE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFSceneModule::OnPropertyEnter(const NFDataList & argVar, const NFGUID & self)
{
	std::vector<PROPERTY_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mvPropertyEnterCallback.begin();
	for (; it != mvPropertyEnterCallback.end(); it++)
	{
		PROPERTY_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(argVar, self);
	}

	return 0;
}

int NFSceneModule::OnRecordEnter(const NFDataList & argVar, const NFGUID & self)
{
	std::vector<RECORD_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mvRecordEnterCallback.begin();
	for (; it != mvRecordEnterCallback.end(); it++)
	{
		RECORD_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		RECORD_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(argVar, self);
	}

	return 0;
}

int NFSceneModule::OnPropertyEvent(const NFGUID & self, const std::string & propertyName, const NFData & oldVar, const NFData & newVar, const NFDataList& argVar, const NFINT64 reason)
{
	std::vector<PROPERTY_SINGLE_EVENT_FUNCTOR_PTR>::iterator it = mvPropertySingleCallback.begin();
	for (; it != mvPropertySingleCallback.end(); it++)
	{
		PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_SINGLE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, propertyName, oldVar, newVar, argVar, reason);
	}

	return 0;
}

int NFSceneModule::OnRecordEvent(const NFGUID & self, const std::string& propertyName, const RECORD_EVENT_DATA & eventData, const NFData & oldVar, const NFData & newVar, const NFDataList& argVar)
{
	std::vector<RECORD_SINGLE_EVENT_FUNCTOR_PTR>::iterator it = mvRecordSingleCallback.begin();
	for (; it != mvRecordSingleCallback.end(); it++)
	{
		RECORD_SINGLE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		RECORD_SINGLE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, propertyName, eventData, oldVar, newVar, argVar);
	}

	return 0;
}

int NFSceneModule::OnMoveCellEvent(const NFGUID & self, const int & scene, const int & group, const NFGUID & fromCell, const NFGUID & toCell)
{
	if (fromCell.IsNull())
	{
		//enter a group
		m_pCellModule->OnObjectEntry(self, scene, group, toCell);
	}
	else if (toCell.IsNull())
	{
		//leave a group
		m_pCellModule->OnObjectLeave(self, scene, group, fromCell);
	}
	else
	{
		//move between two groups
		m_pCellModule->OnObjectMove(self, scene, group, fromCell, toCell);
	}

	return 0;
}

bool NFSceneModule::AddAfterEnterAndReadySceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR &cb)
{
	mvAfterEnterAndReadySceneCallback.push_back(cb);
	return true;
}

int NFSceneModule::AfterEnterAndReadySceneGroup(const NFGUID &self, const int sceneID, const int groupID, const int type, const NFDataList &argList)
{
	std::vector<SCENE_EVENT_FUNCTOR_PTR>::iterator it = mvAfterEnterAndReadySceneCallback.begin();
	for (; it != mvAfterEnterAndReadySceneCallback.end(); it++)
	{
		SCENE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		SCENE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, sceneID, groupID, type, argList);
	}

	return 0;
}
