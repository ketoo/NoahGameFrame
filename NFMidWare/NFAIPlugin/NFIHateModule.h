// -------------------------------------------------------------------------
//    @FileName			:    NFIHateModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFIHateModule
//    @Desc             :
// -------------------------------------------------------------------------

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