/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "NFHateModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFHateModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
	m_pFriendModule = pPluginManager->FindModule<NFIFriendModule>();

	return true;
}

bool NFHateModule::Shut()
{
	return true;
}

bool NFHateModule::AfterInit()
{
	//m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFHateModule::OnClassObjectEvent);
	//m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFHateModule::OnClassObjectEvent);
	return true;
}

bool NFHateModule::BeforeShut()
{
	return true;
}

bool NFHateModule::Execute()
{
	return true;
}

bool NFHateModule::ClearHateObjects(const NFGUID& self)
{
	mtObjectHateMap.RemoveElement(self);

	return true;
}

bool NFHateModule::AddHate(const NFGUID& self, const NFGUID& other, const int nValue)
{
	const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

	NF_SHARE_PTR<SceneHateData> pSceneHateData = mtObjectHateMap.GetElement(NFGUID(sceneID, groupID));
	if (pSceneHateData)
	{
		NF_SHARE_PTR<ObjectHateData> pObjectHateData = pSceneHateData->hateValue.GetElement(other);
		if (!pObjectHateData)
		{
			pObjectHateData = NF_SHARE_PTR<ObjectHateData>(NF_NEW ObjectHateData());
			pSceneHateData->hateValue.AddElement(self, pObjectHateData);
		}

		std::map<NFGUID, int>::iterator it = pObjectHateData->hateValue.find(other);
		if (it == pObjectHateData->hateValue.end())
		{
			pObjectHateData->hateValue.insert(std::map<NFGUID, int>::value_type(other, nValue));
		}
		else
		{
			it->second += nValue;
		}
	}

    return true;
}

bool NFHateModule::EndSpring(const NFGUID & other)
{
	const int sceneID = m_pKernelModule->GetPropertyInt(other, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(other, NFrame::IObject::GroupID());

	NF_SHARE_PTR<SceneHateData> pSceneHateData = mtObjectHateMap.GetElement(NFGUID(sceneID, groupID));
	if (pSceneHateData)
	{
		NF_SHARE_PTR<ObjectHateData> pHateData = pSceneHateData->hateValue.First();
		while (pHateData)
		{
			std::map<NFGUID, int>::iterator it = pHateData->hateValue.find(other);
			if (it != pHateData->hateValue.end())
			{
				pHateData->hateValue.erase(it);
			}

			pHateData = pSceneHateData->hateValue.Next();
		}
	}

	return true;
}

bool NFHateModule::OnSpring(const NFGUID & self, const NFGUID & other)
{
	//根据职业,等级,血量,防御
	//战斗状态只打仇恨列表内的人，巡逻,休闲状态才重新找对象打
	if (m_pFriendModule->IsEnemy(self, other))
	{
		const int hp = m_pKernelModule->GetPropertyInt(other, NFrame::NPC::HP());
		int initialValue = 100;
		if (hp > 0 && hp < 1000)
		{
			initialValue += (1000 - hp) / 2;
		}

		AddHate(self, other, initialValue);
	}
	return true;
}

bool NFHateModule::SetHateValue(const NFGUID& self, const NFGUID& other, const int nValue)
{
	const int sceneID = m_pKernelModule->GetPropertyInt(other, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(other, NFrame::IObject::GroupID());

	NF_SHARE_PTR<SceneHateData> pSceneHateData = mtObjectHateMap.GetElement(NFGUID(sceneID, groupID));
	if (pSceneHateData)
	{
		NF_SHARE_PTR<ObjectHateData> pHateData = pSceneHateData->hateValue.GetElement(self);
		if (pHateData)
		{
			std::map<NFGUID, int>::iterator it = pHateData->hateValue.find(other);
			if (it == pHateData->hateValue.end())
			{
				pHateData->hateValue.insert(std::map<NFGUID, int>::value_type(other, nValue));
			}
			else
			{
				if (nValue > 0)
				{
					it->second = nValue;
				}
				else
				{
					pHateData->hateValue.erase(it);
				}
			}
		}
	}

    return true;
}

int NFHateModule::OnClassObjectEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{

	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		const std::string& className = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
		if (className == NFrame::NPC::ThisName())
		{
			EndSpring(self);
		}
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
		const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

		const std::string& className = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
		const NFVector3& position = m_pKernelModule->GetPropertyVector3(self, NFrame::IObject::Position());

		if (className == NFrame::NPC::ThisName())
		{
			const int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
			if (nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
			{
				//all enemy
				const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
				const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());
				NFDataList objectList;
				if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, objectList))
				{
					for (int i = 0; i < objectList.GetCount(); ++i)
					{
						NFGUID id = objectList.Object(i);
						OnSpring(self, id);
						OnSpring(id, self);
					}
				}
			}
			else if (nNPCType == NFMsg::ENPCType::ENPCTYPE_NORMAL)
			{
			}

			m_pKernelModule->AddPropertyCallBack(self, NFrame::NPC::HP(), this, &NFHateModule::OnNPCHPEvent);
		}
		else if (className == NFrame::Player::ThisName())
		{
			//all players

			NFDataList objectList;
			if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, objectList, false))
			{
				for (int i = 0; i < objectList.GetCount(); ++i)
				{
					NFGUID id = objectList.Object(i);

					const std::string className = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
					if (className == NFrame::NPC::ThisName())
					{
						const int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
						if (nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
						{
							OnSpring(self, id);
						}
					}
				}
			}
		}
	}

	return 0;
}

int NFHateModule::OnPositionEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	return 0;
}

int NFHateModule::OnNPCHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	const NFGUID& lastAttacker = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::LastAttacker());
	if (newVar.GetInt() <= 0)
	{
		ClearHateObjects(self);
	}
	else
	{
		int tempHP = oldVar.GetInt() - newVar.GetInt();
		if (tempHP > 0)
		{
			AddHate(self, lastAttacker, tempHP);
		}
	}

	return 0;
}

bool NFHateModule::CopyHateList(const NFGUID& self, const NFGUID& other)
{
    return true;
}

NFGUID NFHateModule::QueryMaxHateObject(const NFGUID& self)
{
	const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

	NF_SHARE_PTR<SceneHateData> pSceneHateData = mtObjectHateMap.GetElement(NFGUID(sceneID, groupID));
	if (pSceneHateData)
	{
		NF_SHARE_PTR<ObjectHateData> pHateData = pSceneHateData->hateValue.GetElement(self);
		if (pHateData)
		{
			int maxHateValue = 0;
			NFGUID enemy;

			std::map<NFGUID, int>::iterator it = pHateData->hateValue.begin();
			for (; it != pHateData->hateValue.end(); it++)
			{
				if (it->second > maxHateValue)
				{
					maxHateValue = it->second;
					enemy = it->first;
				}
			}

			return enemy;
		}
	}
 
	return NULL_OBJECT;
}

int NFHateModule::QueryHateValue(const NFGUID& self, const NFGUID& other)
{
	const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

	NF_SHARE_PTR<SceneHateData> pSceneHateData = mtObjectHateMap.GetElement(NFGUID(sceneID, groupID));
	if (pSceneHateData)
	{
		NF_SHARE_PTR<ObjectHateData> pHateData = pSceneHateData->hateValue.GetElement(self);
		if (pHateData)
		{
			std::map<NFGUID, int>::iterator it = pHateData->hateValue.find(other);
			if (it != pHateData->hateValue.end())
			{
				return it->second;
			}
		}
	}

    return -1;
}
