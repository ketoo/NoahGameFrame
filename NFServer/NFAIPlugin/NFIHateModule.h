// -------------------------------------------------------------------------
//    文件名      ：    NFIHateModule.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2012-07-14 08:51
//    功能描述    ：    仇恨管理模块
//
// -------------------------------------------------------------------------

#ifndef _NFI_HATE_MODULE_H
#define _NFI_HATE_MODULE_H

#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIHateModule
    : public NFILogicModule
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