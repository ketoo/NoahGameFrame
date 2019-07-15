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

#ifndef NF_ITEM_MODULE_H
#define NF_ITEM_MODULE_H

#include "NFPackModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFItemModule
    : public NFIItemModule
{
public:

    NFItemModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFItemModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual bool UseItem(const NFGUID& self, const std::string& strItemID, const NFGUID& targetID, const NFVector3& vector);

protected:
	virtual bool CheckConfig();


	int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var );
	int OnRequireUseItemPosEvent( const NFGUID& self, const NFEventDefine nEventID, const NFDataList& var );
    int AddItemEffectDataProperty( const NFGUID& self, const NFGUID& xTarget, const std::string& strItemID);
    bool ConsumeDataItemProperty( const NFGUID& self, const std::string& strID);

protected:
	void OnClientUseItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPickItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
    NFIEventModule* m_pEventModule;
	NFIKernelModule* m_pKernelModule;
	NFINetModule* m_pNetModule;
	NFILogModule* m_pLogModule;
    NFIPackModule* m_pPackModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pLogicClassModule;
    NFIPropertyModule* m_pPropertyModule;
	NFIHeroModule* m_pHeroModule;
	NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
	NFICommonConfigModule* m_pCommonConfigModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
