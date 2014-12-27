// -------------------------------------------------------------------------
//    文件名      ：    NFCHateModule.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2012-07-14 08:51
//    功能描述    ：    仇恨管理模块
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "NFCHateModule.h"
//

bool NFCHateModule::Empty(const NFIDENTID& self)
{
    TOBJECTHATEMAP::iterator it = mtObjectHateMap.find(self);
    if (it != mtObjectHateMap.end())
    {
        it->second->empty();
    }

    return true;
}

bool NFCHateModule::AddHate(const NFIDENTID& self, const NFIDENTID& other, const int nValue)
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

bool NFCHateModule::SetHateValue(const NFIDENTID& self, const NFIDENTID& other, const int nValue)
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

bool NFCHateModule::CopyHateList(const NFIDENTID& self, const NFIDENTID& other)
{
    return true;
}

void NFCHateModule::CompSortList(const NFIDENTID& self)
{

}

bool NFCHateModule::QueryMaxHateObject(const NFIDENTID& self, std::list<std::string>& HateList, const int nCount)
{
    return true;
}

NFIDENTID NFCHateModule::QueryMaxHateObject(const NFIDENTID& self)
{
    return NFIDENTID();
}

int NFCHateModule::QueryHateValue(const NFIDENTID& self, const NFIDENTID& other)
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

bool NFCHateModule::ClearHate(const NFIDENTID& self)
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