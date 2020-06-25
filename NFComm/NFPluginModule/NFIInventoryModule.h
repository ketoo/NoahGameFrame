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


#ifndef NFI_INVENTORY_MODULE_H
#define NFI_INVENTORY_MODULE_H

#include <iostream>
#include "NFIModule.h"

class NFIInventoryModule : public NFIModule
{
public:
    virtual NFGUID CreateEquip(const NFGUID& self, const std::string& strConfigName, const int nCount = 1) = 0;
    virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const int nCount) = 0;
	virtual int ItemCount(const NFGUID& self, const std::string& strItemConfigID) = 0;

    virtual bool DeleteEquip(const NFGUID& self, const NFGUID& id) = 0;
	virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
};

#endif