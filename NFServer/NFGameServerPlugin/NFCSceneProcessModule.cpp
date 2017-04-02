// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCSceneProcessModule
//
// -------------------------------------------------------------------------

#include "NFCSceneProcessModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCSceneProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    return true;
}

bool NFCSceneProcessModule::Shut()
{
    return true;
}

bool NFCSceneProcessModule::Execute()
{
    return true;
}

bool NFCSceneProcessModule::AfterInit()
{
    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCSceneProcessModule::OnObjectClassEvent);

	m_pSceneAOIModule->AddEnterSceneConditionCallBack(this, &NFCSceneProcessModule::EnterSceneConditionEvent);

	m_pSceneAOIModule->AddBeforeEnterSceneGroupCallBack(this, &NFCSceneProcessModule::BeforeEnterSceneGroupEvent);
	m_pSceneAOIModule->AddAfterEnterSceneGroupCallBack(this, &NFCSceneProcessModule::AfterEnterSceneGroupEvent);
	m_pSceneAOIModule->AddBeforeLeaveSceneGroupCallBack(this, &NFCSceneProcessModule::BeforeLeaveSceneGroupEvent);
	m_pSceneAOIModule->AddAfterLeaveSceneGroupCallBack(this, &NFCSceneProcessModule::AfterLeaveSceneGroupEvent);
    //////////////////////////////////////////////////////////////////////////

    NF_SHARE_PTR<NFIClass> xLogicClass =  m_pClassModule->GetElement(NFrame::Scene::ThisName());
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
bool NFCSceneProcessModule::RequestEnterScene(const NFGUID & self, const int nSceneID, const int nType, const NFDataList & argList)
{
	return RequestEnterScene(self, nSceneID, -1, nType, argList);
}

bool NFCSceneProcessModule::RequestEnterScene(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	if (GetCloneSceneType(nSceneID) == E_SCENE_TYPE::SCENE_TYPE_SINGLE_CLONE_SCENE)
	{
		int nNewGroupID = m_pKernelModule->RequestGroupScene(nSceneID);
		if (nNewGroupID > 0)
		{
			return m_pSceneAOIModule->RequestEnterScene(self, nSceneID, nNewGroupID, nType, argList);
		}
	}
	else if (GetCloneSceneType(nSceneID) == E_SCENE_TYPE::SCENE_TYPE_MULTI_CLONE_SCENE)
	{
		if (nGroupID > 0)
		{
			return m_pSceneAOIModule->RequestEnterScene(self, nSceneID, nGroupID, nType, argList);
		}
		else
		{
			int nNewGroupID = m_pKernelModule->RequestGroupScene(nSceneID);
			if (nNewGroupID > 0)
			{
				return m_pSceneAOIModule->RequestEnterScene(self, nSceneID, nNewGroupID, nType, argList);
			}
		}
		
	}
	else if (GetCloneSceneType(nSceneID) == E_SCENE_TYPE::SCENE_TYPE_NORMAL)
	{
		return m_pSceneAOIModule->RequestEnterScene(self, nSceneID, nGroupID, nType, argList);
	}
	return false;
}

int NFCSceneProcessModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCSceneProcessModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	E_SCENE_TYPE eSceneType = GetCloneSceneType(nSceneID);
	if (eSceneType == E_SCENE_TYPE::SCENE_TYPE_SINGLE_CLONE_SCENE)
	{
		m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

		return 0;
	}
	else if (eSceneType == E_SCENE_TYPE::SCENE_TYPE_MULTI_CLONE_SCENE)
	{
		NFDataList varObjectList;
		if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, varObjectList, true) && varObjectList.GetCount() <= 0)
		{
			m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);
		}
	}

	return 0;
}

int NFCSceneProcessModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
			int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
			int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

            if (GetCloneSceneType(nSceneID) == SCENE_TYPE_SINGLE_CLONE_SCENE)
            {
                m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);
            }
			else if (GetCloneSceneType(nSceneID) == SCENE_TYPE_MULTI_CLONE_SCENE)
			{
				NFDataList varObjectList;
				if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, varObjectList, true) && varObjectList.GetCount() <= 0)
				{
					m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);
				}
			}
        }
    }

    return 0;
}

int NFCSceneProcessModule::EnterSceneConditionEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCSceneProcessModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	//you can use object pool to enhance performance
	E_SCENE_TYPE eSceneType = GetCloneSceneType(nSceneID);
	if (eSceneType == E_SCENE_TYPE::SCENE_TYPE_SINGLE_CLONE_SCENE)
	{
		m_pSceneAOIModule->CreateSceneNPC(nSceneID, nGroupID);
	}
	else if (eSceneType  == E_SCENE_TYPE::SCENE_TYPE_MULTI_CLONE_SCENE)
	{
		NFDataList varObjectList;
		if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, varObjectList, true) && varObjectList.GetCount() <= 0)
		{
			m_pSceneAOIModule->CreateSceneNPC(nSceneID, nGroupID);
		}
	}

	return 0;
}

int NFCSceneProcessModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{


	return 1;
}

E_SCENE_TYPE NFCSceneProcessModule::GetCloneSceneType(const int nSceneID)
{
	std::string strSceneIDName = lexical_cast<std::string>(nSceneID);
    if (m_pElementModule->ExistElement(strSceneIDName))
    {
		int nClone = m_pElementModule->GetPropertyInt(strSceneIDName, NFrame::Scene::CanClone());
		int nMulti = m_pElementModule->GetPropertyInt(strSceneIDName, NFrame::Scene::Share());
		if (nClone > 0)
		{
			if (nMulti > 0)
			{
				return E_SCENE_TYPE::SCENE_TYPE_MULTI_CLONE_SCENE;
			}
			else
			{
				return E_SCENE_TYPE::SCENE_TYPE_SINGLE_CLONE_SCENE;
			}
		}
		else
		{
			return E_SCENE_TYPE::SCENE_TYPE_NORMAL;
		}
    }

    return E_SCENE_TYPE::SCENE_TYPE_ERROR;
}

bool NFCSceneProcessModule::LoadSceneResource(const std::string& strSceneIDName)
{
    const std::string& strSceneFilePath = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::FilePath());
 
    rapidxml::file<> xFileSource(strSceneFilePath.c_str());
    rapidxml::xml_document<>  xFileDoc;
    xFileDoc.parse<0>(xFileSource.data());

	int nSceneID = lexical_cast<float>(strSceneIDName);

    rapidxml::xml_node<>* pSeedFileRoot = xFileDoc.first_node();
    for (rapidxml::xml_node<>* pSeedFileNode = pSeedFileRoot->first_node(); pSeedFileNode; pSeedFileNode = pSeedFileNode->next_sibling())
    {
        std::string strSeedID = pSeedFileNode->first_attribute(NFrame::IObject::ID().c_str())->value();
		std::string strConfigID = pSeedFileNode->first_attribute(NFrame::IObject::ConfigID().c_str())->value();
        float fSeedX = lexical_cast<float>(pSeedFileNode->first_attribute(NFrame::IObject::X().c_str())->value());
        float fSeedY = lexical_cast<float>(pSeedFileNode->first_attribute(NFrame::IObject::Y().c_str())->value());
        float fSeedZ = lexical_cast<float>(pSeedFileNode->first_attribute(NFrame::IObject::Z().c_str())->value());
		int nWeight = lexical_cast<int>(pSeedFileNode->first_attribute("Weight")->value());
		m_pSceneAOIModule->AddSeedData(nSceneID, strSeedID, strConfigID, NFVector3(fSeedX, fSeedY, fSeedZ), nWeight);

		const std::string& strRelivePosition = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::RelivePos());
		NFDataList xPositionList;
		xPositionList.Split(strRelivePosition, ";");
		for (int i = 0; i < xPositionList.GetCount(); ++i)
		{
			const std::string& strPos = xPositionList.String(i);
			if (!strPos.empty())
			{
				NFDataList xPosition;
				xPosition.Split(strPos, ",");
				if (xPosition.GetCount() == 3)
				{
					float fPosX = lexical_cast<float>(xPosition.String(0));
					float fPosY = lexical_cast<float>(xPosition.String(1));
					float fPosZ = lexical_cast<float>(xPosition.String(2));
					m_pSceneAOIModule->AddRelivePosition(nSceneID, i, NFVector3(fPosX, fPosY, fPosZ));
				}
			}
		}
	}

    return true;
}

bool NFCSceneProcessModule::CreateSceneBaseGroup(const std::string & strSceneIDName)
{
	const int nSceneID = lexical_cast<int>(strSceneIDName);
	m_pKernelModule->RequestGroupScene(nSceneID);

	if (GetCloneSceneType(nSceneID) == SCENE_TYPE_NORMAL)
	{
		//line 10
		for (int i = 0; i < 10; ++i)
		{
			int nGroupID = m_pKernelModule->RequestGroupScene(nSceneID);
			m_pSceneAOIModule->CreateSceneNPC(nSceneID, nGroupID);
		}

		return true;
	}
	return false;
}
