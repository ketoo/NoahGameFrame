/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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


#include "NFCTileMapModule.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCTileMapModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pTileModule = pPluginManager->FindModule<NFITileModule>();
	
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

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
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

	NF_SHARE_PTR<NFMapEx<TileKey, TileData>> xStaticTileMap = mxStaticTileData.GetElement(nSceneID);
	if (xStaticTileMap == nullptr)
	{
		return false;
	}

	NF_SHARE_PTR<NFMapEx<TileKey, TileData>> xPlayerTileMap = mxPlayerTileData.GetElement(self);
	if (xPlayerTileMap == nullptr)
	{
		xPlayerTileMap = NF_SHARE_PTR<NFMapEx<TileKey, TileData>>(NF_NEW NFMapEx<TileKey, TileData>());
		mxPlayerTileData.AddElement(self, xPlayerTileMap);
	}

	NF_SHARE_PTR<TileData> xStaticTileData = xStaticTileMap->First();
	for (xStaticTileData != nullptr; xStaticTileData != nullptr; xStaticTileData = xStaticTileMap->Next())
	{
		xPlayerTileMap->AddElement(TileKey(xStaticTileData->x, xStaticTileData->y), xStaticTileData);
	}

	//remove tile which the player has removed
	std::string strTileData;
	if (m_pTileModule->GetOnlinePlayerTileData(other, strTileData))
	{

	}

	return true;
}

bool NFCTileMapModule::ReleaseTileMap(const NFGUID self)
{
	return true;
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
	GenerateTileMap(self, self, nSceneID);

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
		std::string strTileName = pSeedFileNode->first_attribute("tileType")->value();
		int x = lexical_cast<int>(pSeedFileNode->first_attribute("X")->value());
		int y = lexical_cast<int>(pSeedFileNode->first_attribute("Y")->value());
		int z = lexical_cast<int>(pSeedFileNode->first_attribute("Z")->value());

		NF_SHARE_PTR<NFMapEx<TileKey, TileData>> xTileMap = mxStaticTileData.GetElement(nSceneID);
		if (xTileMap != nullptr)
		{
			//NFASSERT(nSceneID, "There exist scene xml file", __FILE__, __FUNCTION__);
		}

		xTileMap = NF_SHARE_PTR<NFMapEx<TileKey, TileData>>(NF_NEW NFMapEx<TileKey, TileData>());
		mxStaticTileData.AddElement(nSceneID, xTileMap);

		NF_SHARE_PTR<TileData> xTileData = xTileMap->GetElement(TileKey(x, y));
		if (xTileData != nullptr)
		{
			//NFASSERT(0, "There exist no App ID", __FILE__, __FUNCTION__);
		}

		xTileData = NF_SHARE_PTR<TileData>(NF_NEW TileData(x, y, strTileName));
		xTileMap->AddElement(TileKey(x, y), xTileData);
	}
}