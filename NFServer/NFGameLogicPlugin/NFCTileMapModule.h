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


#ifndef NFC_TILE_MAP_MODULE_H
#define NFC_TILE_MAP_MODULE_H

#include "NFComm/NFPluginModule/NFITileMapModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIBigMapRedisModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildRedisModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFITileModule.h"

class NFCTileMapModule
    : public NFITileMapModule
{
public:
	NFCTileMapModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual ~NFCTileMapModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	/*
	 * if you stay at your home, than you can use your home scene data generate the map
	 * but, if you are attacking the others, you need to use that player's home scene to build the map for you
	 * the reason is now you are walking in that player's tile map
	 */
	virtual bool GenerateTileMap(const NFGUID self, const NFGUID other, const int nSceneID);
	virtual bool ReleaseTileMap(const NFGUID self);

protected:

	int BeforeEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterEnterSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int BeforeLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterLeaveSceneGroupEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	int OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var);
	
	void LoadStaticTileData(const std::string& strSceneIDName);

private:

	class TileData
	{
	public:
		TileData(int nx, int ny, std::string name)
		{
			x = nx;
			y = ny;
			tileName = name;
		}
		int x;
		int y;
		std::string tileName;
	};

	typedef std::pair<int, int> TileKey;

	NFMapEx<NFGUID, NFMapEx<TileKey, TileData> > mxPlayerTileData;
	NFMapEx<int, NFMapEx<TileKey, TileData> > mxStaticTileData;


private:
	NFITileModule* m_pTileModule;
	NFINetModule* m_pNetModule;
	NFIClassModule* m_pClassModule;
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pLogicClassModule;
	NFIElementModule* m_pElementModule;
	NFISceneAOIModule* m_pSceneAOIModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
