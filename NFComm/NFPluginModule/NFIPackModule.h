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


#ifndef NFI_PACK_MODULE_H
#define NFI_PACK_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif

class NFIPackModule : public NFIModule
{
public:
    virtual const NFGUID& CreateEquip(const NFGUID& self, const std::string& strConfigName) = 0;
    virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const int nCount) = 0;

    virtual bool DeleteEquip(const NFGUID& self, const NFGUID& id) = 0;
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;

};

#endif