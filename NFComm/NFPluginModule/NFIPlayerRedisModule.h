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


#ifndef NFI_PLAYER_REDIS_MODULE_H
#define NFI_PLAYER_REDIS_MODULE_H

#include "NFIModule.h"

class NFIPlayerRedisModule
    : public NFIModule
{

public:
	virtual bool ExistRoleName(const std::string& strRoleName) = 0;
	virtual bool CreateRole(const std::string& strAccount, const std::string& strRoleName, const NFGUID& id, const int nHomeSceneID) = 0;
	virtual bool GetRoleInfo(const std::string& strAccount, std::string& strRoleName, NFGUID& id) = 0;
	
	//virtual bool LoadPlayerData(const NFGUID& self, NFMsg::RoleDataPack& roleData) = 0;
	//virtual bool LoadPlayerData(const NFGUID& self, NFMsg::PVPPlayerInfo& roleData) = 0;
	virtual bool SavePlayerTile(const int nSceneID, const NFGUID& self, const std::string& strTileData) = 0;

	virtual bool LoadPlayerTile(const int nSceneID, const NFGUID& self, std::string& strTileData) = 0;
	virtual bool LoadPlayerTileRandom(const int nSceneID, NFGUID& xPlayer, std::string& strTileData) = 0;

	virtual bool GetPropertyList(const NFGUID& self, const std::vector<std::string>& fields, std::vector<std::string>& values) = 0;

	virtual NFINT64 GetPropertyInt(const NFGUID& self, const std::string& strPropertyName) = 0;
	virtual int GetPropertyInt32(const NFGUID& self, const std::string& strPropertyName) = 0;
	virtual double GetPropertyFloat(const NFGUID& self, const std::string& strPropertyName) = 0;
	virtual std::string GetPropertyString(const NFGUID& self, const std::string& strPropertyName) = 0;
	virtual NFGUID GetPropertyObject(const NFGUID& self, const std::string& strPropertyName) = 0;
	virtual NFVector2 GetPropertyVector2(const NFGUID& self, const std::string& strPropertyName) = 0;
	virtual NFVector3 GetPropertyVector3(const NFGUID& self, const std::string& strPropertyName) = 0;
};

#endif