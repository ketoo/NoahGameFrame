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

#ifndef NFI_HATE_MODULE_H
#define NFI_HATE_MODULE_H

#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"

class NFIHateModule
    : public NFIModule
{
public:
    virtual bool ClearHate(const NFGUID& self) = 0;

    virtual bool Empty(const NFGUID& self) = 0;

    virtual bool AddHate(const NFGUID& self, const NFGUID& other, const int nValue) = 0;

    virtual bool SetHateValue(const NFGUID& self, const NFGUID& other, const int nValue) = 0;

    virtual bool CopyHateList(const NFGUID& self, const NFGUID& other) = 0;

    virtual void CompSortList(const NFGUID& self) = 0;

    virtual const NFGUID& QueryMaxHateObject(const NFGUID& self) = 0;

    virtual bool QueryMaxHateObject(const NFGUID& self, std::list<std::string>& HateList, const int nCount = 1) = 0;

    virtual int QueryHateValue(const NFGUID& self, const NFGUID& other) = 0;

protected:

private:
};

#endif