/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: yu.tang
   
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


#ifndef NFC_ITEM_ITEM_CONSUME_PROCESS_MODULE_H
#define NFC_ITEM_ITEM_CONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIHeroPropertyModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIPVPModule.h"

class NFCItemItemConsumeProcessModule
	: public NFIItemConsumeProcessModule
{

public:
	NFCItemItemConsumeProcessModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}
	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

	
	virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID);

	
	virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID);

protected:
	virtual int ConsumeNormalProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID);
	virtual int ConsumeTempProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID);

private:
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFIPackModule* m_pPackModule;
	NFIElementModule* m_pElementModule;
	NFIItemModule* m_pItemModule;
	NFIHeroModule* m_pHeroModule;
	NFIHeroPropertyModule* m_pHeroPropertyModule;
	NFIPropertyModule* m_pPropertyModule;
	NFILevelModule* m_pLevelModule;
};

#endif
