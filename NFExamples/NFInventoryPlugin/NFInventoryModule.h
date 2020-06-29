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


#ifndef NF_INVENTORY_MODULE_H
#define NF_INVENTORY_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIInventoryModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFInventoryModule
    : public NFIInventoryModule
{
public:

	NFInventoryModule(NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFInventoryModule() {};

    virtual bool Init() override;
    virtual bool Shut() override;
    virtual bool Execute() override;
    virtual bool AfterInit() override;

	///////////
	virtual NFGUID CreateEquip( const NFGUID& self, const std::string& strConfigName, const int nCount = 1);
	virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const int nCount);

	virtual int ItemCount(const NFGUID& self, const std::string& strItemConfigID);

	virtual bool DeleteEquip(const NFGUID& self, const NFGUID& id);
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);

protected:
	bool CreateItemInNormalBag(const NFGUID& self, const std::string& strConfigName, const int nCount);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
};


#endif
