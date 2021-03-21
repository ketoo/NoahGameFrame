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


#include "NFSceneProcessModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

bool NFSceneProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pCellModule = pPluginManager->FindModule<NFICellModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	
    return true;
}

bool NFSceneProcessModule::Shut()
{
    return true;
}

bool NFSceneProcessModule::Execute()
{
    return true;
}

bool NFSceneProcessModule::AfterInit()
{
    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFSceneProcessModule::OnObjectClassEvent);

	m_pSceneModule->AddEnterSceneConditionCallBack(this, &NFSceneProcessModule::EnterSceneConditionEvent);

	m_pSceneModule->AddBeforeEnterSceneGroupCallBack(this, &NFSceneProcessModule::BeforeEnterSceneGroupEvent);
	m_pSceneModule->AddAfterEnterSceneGroupCallBack(this, &NFSceneProcessModule::AfterEnterSceneGroupEvent);
	m_pSceneModule->AddBeforeLeaveSceneGroupCallBack(this, &NFSceneProcessModule::BeforeLeaveSceneGroupEvent);
	m_pSceneModule->AddAfterLeaveSceneGroupCallBack(this, &NFSceneProcessModule::AfterLeaveSceneGroupEvent);
    //////////////////////////////////////////////////////////////////////////

    return true;
}

bool NFSceneProcessModule::ReadyExecute()
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			//load first
			LoadSceneResource(strId);
			//create second
			CreateSceneBaseGroup(strId);
		}
	}

	return true;
}

bool NFSceneProcessModule::RequestEnterScene(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFVector3& pos, const NFDataList & argList)
{
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
		if (groupID < 0)
		{
			int nNewGroupID = m_pKernelModule->RequestGroupScene(sceneID);
			if (nNewGroupID > 0)
			{
				return m_pSceneModule->RequestEnterScene(self, sceneID, nNewGroupID, type, pos, argList);
			}
		}
		else if (groupID > 0)
		{
			return m_pSceneModule->RequestEnterScene(self, sceneID, groupID, type, pos, argList);
		}
	}
	else if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{
		return m_pSceneModule->RequestEnterScene(self, sceneID, 1, type, pos, argList);
	}
	else
	{
		if (groupID > 0)
		{
			return m_pSceneModule->RequestEnterScene(self, sceneID, groupID, type, pos, argList);
		}
		else
		{
			int nNewGroupID = m_pKernelModule->RequestGroupScene(sceneID);
			if (nNewGroupID > 0)
			{
				return m_pSceneModule->RequestEnterScene(self, sceneID, nNewGroupID, type, pos, argList);
			}
		}
	}

	return false;
}

int NFSceneProcessModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	//change AI onwer

	return 0;
}

int NFSceneProcessModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{

	}
	else if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
		m_pKernelModule->ReleaseGroupScene(sceneID, groupID);

		return 0;
	}
	else
	{
		const NFGUID masterID = m_pSceneModule->GetPropertyObject(sceneID, groupID, NFrame::Group::MasterID());
		if (masterID.IsNull())
		{
			NFDataList varObjectList;
			if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, varObjectList, true) && varObjectList.GetCount() <= 0)
			{
				m_pKernelModule->ReleaseGroupScene(sceneID, groupID);

				m_pLogModule->LogInfo(self, "DestroyCloneSceneGroup " + std::to_string(groupID), __FUNCTION__ , __LINE__);
			}
		}
	}

	return 0;
}

int NFSceneProcessModule::OnObjectClassEvent(const NFGUID& self, const std::string& className, const CLASS_OBJECT_EVENT classEvent, const NFDataList& var)
{
    if (className == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_DESTROY == classEvent)
        {
			int sceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
			int groupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
			NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());

            if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
            {
            	//wa need to wait for this player for a moment if the player disconnected from server.
                m_pKernelModule->ReleaseGroupScene(sceneID, groupID);

                m_pLogModule->LogInfo(self, "DestroyCloneSceneGroup " + std::to_string(groupID), __FUNCTION__ , __LINE__);
            }
			else if (eSceneType == NFMsg::ESceneType::MULTI_CLONE_SCENE)
			{
				//wa need to wait for this player if the player disconnected from server.
				NFDataList varObjectList;
				if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, varObjectList, true) && varObjectList.GetCount() <= 0)
				{
					m_pKernelModule->ReleaseGroupScene(sceneID, groupID);

					m_pLogModule->LogInfo(self, "DestroyCloneSceneGroup " + std::to_string(groupID), __FUNCTION__ , __LINE__);
				}
			}
        }
    }

    return 0;
}

int NFSceneProcessModule::EnterSceneConditionEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	return 0;
}

int NFSceneProcessModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	//you can use object pool to enhance performance
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
		m_pSceneModule->CreateSceneNPC(sceneID, groupID, NFDataList::Empty());
	}
	else if (eSceneType == NFMsg::ESceneType::MULTI_CLONE_SCENE)
	{
		NFDataList varObjectList;
		if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, varObjectList, true) && varObjectList.GetCount() <= 0)
		{
			m_pSceneModule->CreateSceneNPC(sceneID, groupID, NFDataList::Empty());
		}
	}

	return 0;
}

int NFSceneProcessModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int sceneID, const int groupID, const int type, const NFDataList & argList)
{
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
	}
	else if (eSceneType == NFMsg::ESceneType::MULTI_CLONE_SCENE)
	{
	}
	else if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{
		
	}

	return 1;
}

bool NFSceneProcessModule::LoadSceneResource(const std::string& strSceneIDName)
{
    const std::string& strSceneFilePath = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::FilePath());
 
    rapidxml::file<> xFileSource(strSceneFilePath.c_str());
    rapidxml::xml_document<>  xFileDoc;
    xFileDoc.parse<0>(xFileSource.data());

	int sceneID = lexical_cast<int>(strSceneIDName);

    rapidxml::xml_node<>* pSeedFileRoot = xFileDoc.first_node();
    for (rapidxml::xml_node<>* pSeedFileNode = pSeedFileRoot->first_node(); pSeedFileNode; pSeedFileNode = pSeedFileNode->next_sibling())
    {
        std::string seedID = pSeedFileNode->first_attribute(NFrame::IObject::ID().c_str())->value();
		std::string configID = pSeedFileNode->first_attribute(NFrame::IObject::ConfigID().c_str())->value();
        float fSeedX = lexical_cast<float>(pSeedFileNode->first_attribute("X")->value());
        float fSeedY = lexical_cast<float>(pSeedFileNode->first_attribute("Y")->value());
        float fSeedZ = lexical_cast<float>(pSeedFileNode->first_attribute("Z")->value());
		int nWeight = lexical_cast<int>(pSeedFileNode->first_attribute("Weight")->value());
		m_pSceneModule->AddSeedData(sceneID, seedID, configID, NFVector3(fSeedX, fSeedY, fSeedZ), nWeight);

	}

	const std::string& strRelivePosition = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::RelivePos());
	NFDataList xRelivePositionList;
	xRelivePositionList.Split(strRelivePosition, ";");
	for (int i = 0; i < xRelivePositionList.GetCount(); ++i)
	{
		const std::string& strPos = xRelivePositionList.String(i);
		if (!strPos.empty())
		{
			NFDataList xPosition;
			xPosition.Split(strPos, ",");
			if (xPosition.GetCount() == 3)
			{
				float fPosX = lexical_cast<float>(xPosition.String(0));
				float fPosY = lexical_cast<float>(xPosition.String(1));
				float fPosZ = lexical_cast<float>(xPosition.String(2));
				m_pSceneModule->AddRelivePosition(sceneID, i, NFVector3(fPosX, fPosY, fPosZ));
			}
		}
	}

	const std::string& strTagPosition = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::RelivePosEx());
	NFDataList xTagPositionList;
	xTagPositionList.Split(strTagPosition, ";");
	for (int i = 0; i < xTagPositionList.GetCount(); ++i)
	{
		const std::string& strPos = xTagPositionList.String(i);
		if (!strPos.empty())
		{
			NFDataList xPosition;
			xPosition.Split(strPos, ",");
			if (xPosition.GetCount() == 3)
			{
				float fPosX = lexical_cast<float>(xPosition.String(0));
				float fPosY = lexical_cast<float>(xPosition.String(1));
				float fPosZ = lexical_cast<float>(xPosition.String(2));
				m_pSceneModule->AddTagPosition(sceneID, i, NFVector3(fPosX, fPosY, fPosZ));
			}
		}
	}


    return true;
}

bool NFSceneProcessModule::CreateSceneBaseGroup(const std::string & strSceneIDName)
{
	const int sceneID = lexical_cast<int>(strSceneIDName);
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::Type());
	const int nMaxGroup = m_pElementModule->GetPropertyInt32(std::to_string(sceneID), NFrame::Scene::MaxGroup());
	if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{
		//line 10
		for (int i = 0; i < 1; ++i)
		{
			int groupID = m_pKernelModule->RequestGroupScene(sceneID);
			if (groupID > 0)
			{
				m_pSceneModule->CreateSceneNPC(sceneID, groupID);
			}
			else
			{
				m_pLogModule->LogError("CreateSceneBaseGroup error " + strSceneIDName);
			}
		}

		return true;
	}
	return false;
}
