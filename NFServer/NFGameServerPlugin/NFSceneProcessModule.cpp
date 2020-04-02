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

bool NFSceneProcessModule::RequestEnterScene(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFVector3& pos, const NFDataList & argList)
{
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
		int nNewGroupID = m_pKernelModule->RequestGroupScene(nSceneID);
		if (nNewGroupID > 0)
		{
			return m_pSceneModule->RequestEnterScene(self, nSceneID, nNewGroupID, nType, pos, argList);
		}
	}
	else if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{
		const int nMaxGroup = m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::MaxGroup());
		const int nMaxPlayer = m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::MaxGroupPlayers());
		for (int i = 1; i <= nMaxGroup; ++i)
		{
			return m_pSceneModule->RequestEnterScene(self, nSceneID, 1, nType, pos, argList);
		}

		return false;
	}
	else
	{
		if (nGroupID > 0)
		{
			return m_pSceneModule->RequestEnterScene(self, nSceneID, nGroupID, nType, pos, argList);
		}
		else
		{
			int nNewGroupID = m_pKernelModule->RequestGroupScene(nSceneID);
			if (nNewGroupID > 0)
			{
				return m_pSceneModule->RequestEnterScene(self, nSceneID, nNewGroupID, nType, pos, argList);
			}
		}
	}

	return false;
}

int NFSceneProcessModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	//change AI onwer

	return 0;
}

int NFSceneProcessModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{

	}
	else if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
		m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

		return 0;
	}
	else
	{
		NFDataList varObjectList;
		if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, varObjectList, true) && varObjectList.GetCount() <= 0)
		{
			m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

			m_pLogModule->LogInfo(self, "DestroyCloneSceneGroup " + std::to_string(nGroupID), __FUNCTION__ , __LINE__);
		}
	}

	return 0;
}

int NFSceneProcessModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
			int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
			int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
			NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());

            if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
            {
                m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

                m_pLogModule->LogInfo(self, "DestroyCloneSceneGroup " + std::to_string(nGroupID), __FUNCTION__ , __LINE__);
            }
			else if (eSceneType == NFMsg::ESceneType::MULTI_CLONE_SCENE)
			{
				NFDataList varObjectList;
				if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, varObjectList, true) && varObjectList.GetCount() <= 0)
				{
					m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

					m_pLogModule->LogInfo(self, "DestroyCloneSceneGroup " + std::to_string(nGroupID), __FUNCTION__ , __LINE__);
				}
			}
        }
    }

    return 0;
}

int NFSceneProcessModule::EnterSceneConditionEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFSceneProcessModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	//you can use object pool to enhance performance
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
		m_pSceneModule->CreateSceneNPC(nSceneID, nGroupID, NFDataList::Empty());
	}
	else if (eSceneType  == NFMsg::ESceneType::MULTI_CLONE_SCENE)
	{
		NFDataList varObjectList;
		if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, varObjectList, true) && varObjectList.GetCount() <= 0)
		{
			m_pSceneModule->CreateSceneNPC(nSceneID, nGroupID, NFDataList::Empty());
		}
	}
	else if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{
	}

	return 0;
}

int NFSceneProcessModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
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

	int nSceneID = lexical_cast<int>(strSceneIDName);

    rapidxml::xml_node<>* pSeedFileRoot = xFileDoc.first_node();
    for (rapidxml::xml_node<>* pSeedFileNode = pSeedFileRoot->first_node(); pSeedFileNode; pSeedFileNode = pSeedFileNode->next_sibling())
    {
        std::string strSeedID = pSeedFileNode->first_attribute(NFrame::IObject::ID().c_str())->value();
		std::string strConfigID = pSeedFileNode->first_attribute(NFrame::IObject::ConfigID().c_str())->value();
        float fSeedX = lexical_cast<float>(pSeedFileNode->first_attribute("X")->value());
        float fSeedY = lexical_cast<float>(pSeedFileNode->first_attribute("Y")->value());
        float fSeedZ = lexical_cast<float>(pSeedFileNode->first_attribute("Z")->value());
		int nWeight = lexical_cast<int>(pSeedFileNode->first_attribute("Weight")->value());
		m_pSceneModule->AddSeedData(nSceneID, strSeedID, strConfigID, NFVector3(fSeedX, fSeedY, fSeedZ), nWeight);

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
				m_pSceneModule->AddRelivePosition(nSceneID, i, NFVector3(fPosX, fPosY, fPosZ));
			}
		}
	}
	/*
	const std::string& strTagPosition = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::TagPos());
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
				m_pSceneModule->AddTagPosition(nSceneID, i, NFVector3(fPosX, fPosY, fPosZ));
			}
		}
	}
	
	*/

    return true;
}

bool NFSceneProcessModule::CreateSceneBaseGroup(const std::string & strSceneIDName)
{
	const int nSceneID = lexical_cast<int>(strSceneIDName);
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	const int nMaxGroup = m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::MaxGroup());
	if (eSceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{
		//line 10
		for (int i = 0; i < 1; ++i)
		{
			int nGroupID = m_pKernelModule->RequestGroupScene(nSceneID);
			if (nGroupID > 0)
			{
				m_pSceneModule->CreateSceneNPC(nSceneID, nGroupID);
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
