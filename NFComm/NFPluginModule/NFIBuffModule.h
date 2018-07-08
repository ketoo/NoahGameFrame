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

#ifndef NFI_BUFF_MODULE_H
#define NFI_BUFF_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFIBuffConfigModule.h"


class NFIBuffModule
    : public NFIModule
{

public:

    virtual bool AddBuff(const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent) = 0;


    virtual bool RemoveBuff(const NFGUID& self, const std::string& strConfig) = 0;


    virtual bool RemoveBuff(const NFGUID& self, NFIBuffConfigModule::BuffType eType) = 0;


    virtual bool SelfFree(const NFGUID& self) = 0;


    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty) = 0;

    virtual bool HasBuff(const NFGUID& self, const std::string& strConfig) = 0;



    virtual int Attack(const NFGUID& self, const NFGUID& other) = 0;


    virtual int OnDead(const NFGUID& self) = 0;

};

#endif