// -------------------------------------------------------------------------
//    @FileName			:    NFCTileMapModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-25
//    @Module           :    NFCTileMapModule
//
// -------------------------------------------------------------------------

#include "NFCTileMapModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCTileMapModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pBigMapRedisModule = pPluginManager->FindModule<NFIBigMapRedisModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pGuildRedisModule = pPluginManager->FindModule<NFIGuildRedisModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	
    return true;
}

bool NFCTileMapModule::Shut()
{
    return true;
}

bool NFCTileMapModule::Execute()
{
    return true;
}

bool NFCTileMapModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTileMapModule::OnObjectClassEvent);

	m_pSceneAOIModule->AddBeforeEnterSceneGroupCallBack(this, &NFCTileMapModule::BeforeEnterSceneGroupEvent);
	m_pSceneAOIModule->AddAfterEnterSceneGroupCallBack(this, &NFCTileMapModule::AfterEnterSceneGroupEvent);
	m_pSceneAOIModule->AddBeforeLeaveSceneGroupCallBack(this, &NFCTileMapModule::BeforeLeaveSceneGroupEvent);
	m_pSceneAOIModule->AddAfterLeaveSceneGroupCallBack(this, &NFCTileMapModule::AfterLeaveSceneGroupEvent);

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Map::ThisName());
	if (xLogicClass)
	{
		const std::vector <std::string> &strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string &strId = strIdList[i];

			LoadStaticTileData(strId);
		}
	}

    return true;
}

bool NFCTileMapModule::GenerateTileMap(const NFGUID self, const NFGUID other, const int nSceneID)
{
	ReleaseTileMap(self);

}

bool NFCTileMapModule::ReleaseTileMap(const NFGUID self)
{

}

int NFCTileMapModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_BEFOREDESTROY == eClassEvent)
	{
		ReleaseTileMap(self);
	}

	return 0;
}

int NFCTileMapModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCTileMapModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	GenerateTileMap(self, nSceneID);

	return 0;
}

int NFCTileMapModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	ReleaseTileMap(self);

	return 0;
}

int NFCTileMapModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

void NFCTileMapModule::LoadStaticTileData(const std::string& strSceneIDName)
{
	const std::string& strSceneFilePath = m_pElementModule->GetPropertyString(strSceneIDName, NFrame::Scene::ResPath());

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

		//mxStaticTileData
	}
}