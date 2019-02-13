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

#include "NFCHateModule.h"

bool NFCHateModule::Empty(const NFGUID& self)
{
    TOBJECTHATEMAP::iterator it = mtObjectHateMap.find(self);
    if (it != mtObjectHateMap.end())
    {
        it->second->empty();
    }

    return true;
}

bool NFCHateModule::AddHate(const NFGUID& self, const NFGUID& other, const int nValue)
{
    TOBJECTHATEMAP::iterator it = mtObjectHateMap.find(other);
    if (it == mtObjectHateMap.end())
    {
        THATEMAP* pHatelist = new THATEMAP;
        pHatelist->insert(THATEMAP::value_type(other, nValue));
        mtObjectHateMap.insert(TOBJECTHATEMAP::value_type(other, pHatelist));
    }
    else
    {
        THATEMAP* pHatelist = it->second;
        THATEMAP::iterator itr = pHatelist->find(other);
        if (itr != pHatelist->end())
        {
            it->second += nValue;
        }
    }

    return true;
}

bool NFCHateModule::SetHateValue(const NFGUID& self, const NFGUID& other, const int nValue)
{
    TOBJECTHATEMAP::iterator it = mtObjectHateMap.find(other);
    if (it != mtObjectHateMap.end())
    {
        if (nValue <= 0)
        {
            mtObjectHateMap.erase(it);
        }
        else
        {
            THATEMAP* pHatelist = it->second;
            THATEMAP::iterator itr = pHatelist->find(other);
            if (itr != pHatelist->end())
            {
                it->second += nValue;
            }
        }

    }
    return true;
}

bool NFCHateModule::CopyHateList(const NFGUID& self, const NFGUID& other)
{
    return true;
}

void NFCHateModule::CompSortList(const NFGUID& self)
{

}

bool NFCHateModule::QueryMaxHateObject(const NFGUID& self, std::list<std::string>& HateList, const int nCount)
{
    return true;
}

const NFGUID& NFCHateModule::QueryMaxHateObject(const NFGUID& self)
{
    return NULL_OBJECT;
}

int NFCHateModule::QueryHateValue(const NFGUID& self, const NFGUID& other)
{
    TOBJECTHATEMAP::iterator it = mtObjectHateMap.find(other);
    if (it != mtObjectHateMap.end())
    {
        THATEMAP* pHatelist = it->second;
        THATEMAP::iterator itr = pHatelist->find(other);
        if (itr != pHatelist->end())
        {
            return itr->second;
        }
    }

    return -1;
}

bool NFCHateModule::ClearHate(const NFGUID& self)
{
    TOBJECTHATEMAP::iterator it = mtObjectHateMap.find(self);
    if (it != mtObjectHateMap.end())
    {
        THATEMAP* pList = it->second;
        pList->clear();
        delete pList;
        mtObjectHateMap.erase(it);

        return true;
    }

    return false;
}