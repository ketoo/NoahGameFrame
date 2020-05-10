/*
    Copyright 2009 - 2019 NoahGameFrame/NoahFrame
    
   This source file is part of NoahGameFrame/NoahFrame.
   NoahGameFrame/NoahFrame is open-source software and you can redistribute it and/or modify
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

#include "NFNavigationDataModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
/*
#include "Dependencies/rapidjson/document.h"
#include "Dependencies/rapidjson/writer.h"
#include "Dependencies/rapidjson/stringbuffer.h"
*/

bool NFNavigationDataModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFIGameServerToWorldModule>();

	return true;
}

bool NFNavigationDataModule::AfterInit()
{
    /*
    rapidjson::Document document;
    document.Parse(jsonData.c_str());

    rapidjson::Value& tielConfigData = document["tileConfig"];
    rapidjson::Value& navigationData = document["data"];
    for (auto itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
    {
        printf("Type of member %s is %d\n", itr->name.GetString(), itr->value.GetType());
    }
    */

   // public Dictionary<int, Dictionary<NFGUID, NFVoxel>> data;
    auto sceneElement = m_pClassModule->GetElement(NFrame::Scene::ThisName());
    auto sceneList = sceneElement->GetIDList();
    for (auto it = sceneList.begin(); it != sceneList.end(); ++it)
    {
        const std::string& sceneID = *it;
        const int scene = std::atoi(sceneID.c_str());
        const std::string& sceneName = m_pElementModule->GetPropertyString(sceneID, NFrame::Scene::SceneName());

        std::string jsonData;
        pPluginManager->GetFileContent("../NFDataCfg/Ini/Scene/" + sceneName + ".json", jsonData);
        if (jsonData.empty())
        {
            continue;
        }

        auto groupData = ParseDefaultMapData(scene, jsonData, true);
        mGroupNavigationData.AddElement(NFGUID(scene, 0), groupData);
    }

	return true;
}

const std::string& NFNavigationDataModule::GetDefaultMapData(const int scene)
{
    auto data = mGroupNavigationData.GetElement(NFGUID(scene, 0));
    if (data)
    {
        return data->originalData;
    }

    return NULL_STR;
}

const NF_SHARE_PTR<GroupNavigationData> NFNavigationDataModule::GetMapData(const int scene, const int group)
{
    return mGroupNavigationData.GetElement(NFGUID(scene, group));
}

const NF_SHARE_PTR<NFVoxel> NFNavigationDataModule::GetMapData(const int scene, const int group, const int x, int z)
{
    auto data = mGroupNavigationData.GetElement(NFGUID(scene, group));
    if (data)
    {
        return data->data.GetElement(NFGUID(x, z));
    }

    return nullptr;
}

bool NFNavigationDataModule::SetMapDataOccupyItem(const int scene, const int group, const int x, const int z, const std::string& item)
{
    auto data = mGroupNavigationData.GetElement(NFGUID(scene, group));
    if (data)
    {
        auto voxel = data->data.GetElement(NFGUID(x, z));
        if (voxel)
        {
            voxel->item = item;

            return true;
        }
    }

    return false;
}

bool NFNavigationDataModule::SetMapDataMovable(const int scene, const int group, const int x, int z, const int movable)
{
    auto data = mGroupNavigationData.GetElement(NFGUID(scene, group));
    if (data)
    {
        auto voxel = data->data.GetElement(NFGUID(x, z));
        if (voxel)
        {
            voxel->movable = movable;

            return true;
        }
    }

    return false;
}

bool NFNavigationDataModule::SetMapDataOccupy(const int scene, const int group, const int x, int z, const NFGUID occupy)
{
    auto data = mGroupNavigationData.GetElement(NFGUID(scene, group));
    if (data)
    {
        auto voxel = data->data.GetElement(NFGUID(x, z));
        if (voxel)
        {
            voxel->occupyObject = occupy;

            return true;
        }
    }

    return false;
}

bool NFNavigationDataModule::SetMapDataLayer(const int scene, const int group, const int x, int z, const int layer)
{
    auto data = mGroupNavigationData.GetElement(NFGUID(scene, group));
    if (data)
    {
        auto voxel = data->data.GetElement(NFGUID(x, z));
        if (voxel)
        {
            voxel->layer = layer;

            return true;
        }
    }

    return false;
}

bool NFNavigationDataModule::Shut()
{

	return true;
}

bool NFNavigationDataModule::Execute()
{
	return true;
}

const NF_SHARE_PTR<GroupNavigationData> NFNavigationDataModule::GetMapData(const int scene)
{
	return GetMapData(scene, 0);
}
