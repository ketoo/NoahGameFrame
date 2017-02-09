// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCSceneProcessModule
//
// -------------------------------------------------------------------------

#include "NFCSceneProcessModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCSceneProcessModule::Init()
{
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
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCSceneProcessModule::OnObjectClassEvent);

	m_pSceneAOIModule->AddEnterSceneConditionCallBack(this, &NFCSceneProcessModule::EnterSceneConditionEvent);

	m_pSceneAOIModule->AddBeforeEnterSceneGroupCallBack(this, &NFCSceneProcessModule::BeforeEnterSceneGroupEvent);
	m_pSceneAOIModule->AddAfterEnterSceneGroupCallBack(this, &NFCSceneProcessModule::AfterEnterSceneGroupEvent);
	m_pSceneAOIModule->AddBeforeLeaveSceneGroupCallBack(this, &NFCSceneProcessModule::BeforeLeaveSceneGroupEvent);
	m_pSceneAOIModule->AddAfterLeaveSceneGroupCallBack(this, &NFCSceneProcessModule::AfterLeaveSceneGroupEvent);
    //////////////////////////////////////////////////////////////////////////

    NF_SHARE_PTR<NFIClass> pLogicClass =  m_pClassModule->GetElement(NFrame::Scene::ThisName());
    if (pLogicClass)
    {
        NFList<std::string>& strIdList = pLogicClass->GetIdList();

        std::string strId;
        bool bRet = strIdList.First(strId);
        while (bRet)
        {
			CreateSceneBaseGroup(strId);
            LoadSceneResource(strId);

            bRet = strIdList.Next(strId);
        }
    }

    return true;
}

int NFCSceneProcessModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	return 0;
}

int NFCSceneProcessModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	return 0;
}

int NFCSceneProcessModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
            
            int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
            if (GetCloneSceneType(nSceneID) == SCENE_TYPE_CLONE_SCENE)
            {
                int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

                //m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);

            }
        }
    }

    return 0;
}

int NFCSceneProcessModule::EnterSceneConditionEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	return 0;
}

int NFCSceneProcessModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	//m_pSceneAOIModule->CreateSceneObject(nSceneID, nGroupID);

	return 0;
}

int NFCSceneProcessModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	if (GetCloneSceneType(nSceneID) == SCENE_TYPE_CLONE_SCENE)
	{
		return 0;
	}

	return 1;
}

E_SCENE_TYPE NFCSceneProcessModule::GetCloneSceneType(const int nSceneID)
{
	std::string strSceneIDName = lexical_cast<std::string>(nSceneID);
    if (m_pElementModule->ExistElement(strSceneIDName))
    {
        return (E_SCENE_TYPE)m_pElementModule->GetPropertyInt(strSceneIDName, NFrame::Scene::CanClone());
    }

    return SCENE_TYPE_ERROR;
}

bool NFCSceneProcessModule::LoadSceneResource(const std::string& strSceneIDName)
{
    const std::string& strSceneFilePath = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::FilePath());
 
    rapidxml::file<> xFileSource(strSceneFilePath.c_str());
    rapidxml::xml_document<>  xFileDoc;
    xFileDoc.parse<0>(xFileSource.data());

    rapidxml::xml_node<>* pSeedFileRoot = xFileDoc.first_node();
    for (rapidxml::xml_node<>* pSeedFileNode = pSeedFileRoot->first_node(); pSeedFileNode; pSeedFileNode = pSeedFileNode->next_sibling())
    {
        std::string strSeedID = pSeedFileNode->first_attribute(NFrame::IObject::ID().c_str())->value();
        std::string strConfigID = pSeedFileNode->first_attribute(NFrame::IObject::ConfigID().c_str())->value();
        float fSeedX = lexical_cast<float>(pSeedFileNode->first_attribute(NFrame::IObject::X().c_str())->value());
        float fSeedY = lexical_cast<float>(pSeedFileNode->first_attribute(NFrame::IObject::Y().c_str())->value());
        float fSeedZ = lexical_cast<float>(pSeedFileNode->first_attribute(NFrame::IObject::Z().c_str())->value());

        if (!m_pElementModule->ExistElement(strConfigID))
        {
            assert(0);
        }

		m_pSceneAOIModule->AddSeedData(lexical_cast<int>(strSceneIDName), strSeedID, strConfigID, NFVector3(fSeedX, fSeedY, fSeedZ));
    }

    return true;
}

bool NFCSceneProcessModule::CreateSceneBaseGroup(const std::string & strSceneIDName)
{
	const int nSceneID = lexical_cast<int>(strSceneIDName);
	if (GetCloneSceneType(nSceneID) != SCENE_TYPE_CLONE_SCENE)
	{
		//line 1
		m_pKernelModule->RequestGroupScene(nSceneID);
		return true;
	}

	return false;
}
