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

#ifndef NF_EUIP_PROPERTY_MODULE_H
#define NF_EUIP_PROPERTY_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEquipModule.h"
#include "NFComm/NFPluginModule/NFIEquipPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"

class NFEquipPropertyModule
    : public NFIEquipPropertyModule
{
public:

	NFEquipPropertyModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFEquipPropertyModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	//all data,include base,gem,element
	virtual bool CalEquipProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList);

	//only part of property of equip
	virtual bool CalEquipBaseProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList);
	virtual bool CalEquipRandomProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList);
	virtual bool CalEquipGemProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList);
	virtual bool CalEquipIntensifyProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList);
	virtual bool CalEquipElementProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList);

private:
    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
  
};


#endif
