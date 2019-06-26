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


#ifndef NF_SKILL_MODULE_H
#define NF_SKILL_MODULE_H

#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISkillModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFISkillCooldownModule.h"

class NFSkillModule
    : public NFISkillModule
{
public:

    NFSkillModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFSkillModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual int ExistSkill(const NFGUID& self, const std::string& strSkillName);
	virtual int UseSkill(const NFGUID& self, const std::string& strSkillName, const NFGUID& target, const int index = -1);
	virtual int UseSkill(const NFGUID& self, const std::string& strSkillName, const NFDataList& target, const int index = -1);

protected:
    int OnClassObjectEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var );


protected:
	void OnClienUseSkill(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFISkillCooldownModule* m_pSkillCooldownModule;
	NFINetModule* m_pNetModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIEventModule* m_pEventModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
