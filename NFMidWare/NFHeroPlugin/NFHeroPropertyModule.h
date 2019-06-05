/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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


#ifndef NF_HERO_PROPERTY_MODULE_MODULE_H
#define NF_HERO_PROPERTY_MODULE_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIHeroPropertyModule.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEquipPropertyModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFHeroPropertyModule
    : public NFIHeroPropertyModule
{
public:
	NFHeroPropertyModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFHeroPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();


	virtual bool CalFightintHeroProperty(const NFGUID& self);

protected:
	int OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

	virtual int OnObjectHeroRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);
	virtual bool OnHeroPropertyUpdate(const NFGUID& self, const NFGUID& xHeroGUID);

	virtual bool AddHeroProperty(const NFGUID& self, const NFGUID& xHeroGUID);

	virtual bool CalHeroAllProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList);
	virtual bool CalHeroBaseProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList);
	virtual bool CalHeroTalentProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList);
	virtual bool CalHeroEquipProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList);


	int CalHeroDiamond(const int nStar, int& leftStar);
private:

	NFIHeroModule* m_pHeroModule;
	NFIClassModule* m_pLogicClassModule;
    NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIEquipPropertyModule* m_pEquipPropertyModule;
	NFIPropertyModule* m_pPropertyModule;
	NFIPropertyConfigModule* m_pPropertyConfigModule;
	NFILogModule* m_pLogModule;
};

#endif
