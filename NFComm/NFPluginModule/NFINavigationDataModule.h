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


#ifndef NFI_NAVIGATION_DATA_MODULE_H
#define NFI_NAVIGATION_DATA_MODULE_H

#include "NFIModule.h"
#include "Dependencies/ajson/ajson.hpp"
#include "Dependencies/nlohmann/json.hpp"

using json = nlohmann::json;

class NFTileConfig
{
public:
    int maxLayer = 10;
    int accumulationLayer = 3;
    int mapSize = 32;

    int cellSizeX = 2;
    int cellSizeZ = 2;
    float layerHeight = 1.0f;
};

AJSON(NFTileConfig, maxLayer, accumulationLayer, mapSize, cellSizeX, cellSizeZ, layerHeight)

class NFVoxel
{
public:
    int layer = 1;
    string name;
    string item;
    int movable = 0;
    int breakable = 0;
    int stair_h = 0;
    int stair_v = 0;
    //expand
    NFGUID occupyObject;
    int x = 0;
    int z = 0;
};
AJSON(NFVoxel, layer, name, item, movable, breakable, stair_h, stair_v)

class GroupNavigationData
{
private:
    GroupNavigationData() {}
public:
    GroupNavigationData(const int scene, const int group)
    {
        this->sceneID = scene;
        this->groupID = group;
    }

    int sceneID;
    int groupID;

    std::string originalData; 

    NFTileConfig tileConfig;
    //<x, z> ==> data
    NFMapEx<NFGUID, NFVoxel> data;
};

class NFINavigationDataModule : public NFIModule
{
public:
    virtual const std::string& GetDefaultMapData(const int scene) = 0;

    static NF_SHARE_PTR<GroupNavigationData> ParseDefaultMapData(const int scene, const std::string& sceneMapData, const bool saveOriginalData = false)
    {
        auto groupData = NF_SHARE_PTR<GroupNavigationData>(NF_NEW GroupNavigationData(scene, 0));
        if (!sceneMapData.empty())
        {
            auto map = json::parse(sceneMapData);
            auto tileConfig = map["tileConfig"];
            auto data = map["data"];

            if (saveOriginalData)
			{
				groupData->originalData = sceneMapData;
			}

            std::string tileConfigValue = tileConfig.dump();
            ajson::load_from_buff(groupData->tileConfig, tileConfigValue.c_str(), tileConfigValue.length());
            for (auto it = data.begin(); it != data.end(); ++it)
            {
                const std::string& cellID = it.key();
                const std::string& cellData = it.value().dump();
                NFGUID posID(cellID);
                auto voxelData = groupData->data.GetElement(posID);
                if (voxelData == nullptr)
                {
                    voxelData = NF_SHARE_PTR<NFVoxel>(NF_NEW NFVoxel());
                    ajson::load_from_buff(*voxelData, cellData.c_str(), cellData.length());
                    voxelData->x = posID.GetHead();
                    voxelData->z = posID.GetData();

                    groupData->data.AddElement(posID, voxelData);
                }
            }
        }

        return groupData;
    }

	virtual const NF_SHARE_PTR<GroupNavigationData> GetMapData(const int scene) = 0;
	virtual const NF_SHARE_PTR<GroupNavigationData> GetMapData(const int scene, const int group) = 0;
    virtual const NF_SHARE_PTR<NFVoxel> GetMapData(const int scene, const int group, const int x, int z) = 0;

    //modify map data in run time
    virtual bool SetMapDataOccupyItem(const int scene, const int group, const int x, const int z, const std::string& item) = 0;
    virtual bool SetMapDataMovable(const int scene, const int group, const int x, int z, const int movable) = 0;
    virtual bool SetMapDataOccupy(const int scene, const int group, const int x, int z, const NFGUID occupy) = 0;
    virtual bool SetMapDataLayer(const int scene, const int group, const int x, int z, const int layer) = 0;

    //register callback function for modifying the map data

};

#endif