/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: chuanbo.guo
   
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


#ifndef NF_EUIP_MODULE_H
#define NF_EUIP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEquipModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFEquipModule
    : public NFIEquipModule
{
public:

    NFEquipModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFEquipModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	/////////////for hero/////////////////////////////////////////////////////////////
	virtual bool DressEquipForHero(const NFGUID& self, const NFGUID& hero, const NFGUID& id);
	virtual bool TakeOffEquipForm(const NFGUID& self, const NFGUID& hero, const NFGUID& id);

protected:
	///////////////////////////////////////////////
	bool InlaystoneToEquip(const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex);
	bool HoleToEquip(const NFGUID& self, const NFGUID& xEquipID);
	bool IntensifylevelToEquip(const NFGUID& self, const NFGUID& xEquipID);
	bool ElementlevelToEquip(const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType);

	/////////////random propertyid/////////////////////////////////////////////////////////////
	bool SetEquipRandPropertyID(const NFGUID& self, const NFGUID& id, const std::string& strPropertyID);
	/////////////sotne/////////////////////////////////////////////////////////////
	bool SetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, const int eIndex, const std::string& strPropertyID);
	/////////////slot/hole/////////////////////////////////////////////////////////////
	bool AddEquipHoleCount(const NFGUID& self, const NFGUID& id);
	int GetEquipHoleCount(const NFGUID& self, const NFGUID& id);
	/////////////intensify/////////////////////////////////////////////////////////////
	bool AddEquipIntensifyLevel(const NFGUID& self, const NFGUID& id);
	int GetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id);
	/////////////wind, fire, ice, light, poison/////////////////////////////////////////////////////////////
	bool AddEquipElementLevel(const NFGUID& self, const NFGUID& id, int eIndex);
	int GetEquipElementLevel(const NFGUID& self, const NFGUID& id, int eIndex);

protected:
    bool Ramdom(const int nNowLevel, const int nMaxLevel);
	
private:
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
	NFIPackModule* m_pPackModule;
	NFICommonConfigModule* m_pCommonConfigModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
