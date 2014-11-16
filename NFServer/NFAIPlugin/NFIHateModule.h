// -------------------------------------------------------------------------
//    文件名      ：    NFIHateModule.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2012-07-14 08:51
//    功能描述    ：    仇恨管理模块
//
// -------------------------------------------------------------------------

#ifndef _NFI_HATE_MODULE_H_
#define _NFI_HATE_MODULE_H_

#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIHateModule
    : public NFILogicModule
{
public:
    virtual bool ClearHate(const NFIDENTID& self) = 0;

    virtual bool Empty(const NFIDENTID& self) = 0;

    virtual bool AddHate(const NFIDENTID& self, const NFIDENTID& other, const int nValue) = 0;

    virtual bool SetHateValue(const NFIDENTID& self, const NFIDENTID& other, const int nValue) = 0;

    virtual bool CopyHateList(const NFIDENTID& self, const NFIDENTID& other) = 0;

    virtual void CompSortList(const NFIDENTID& self) = 0;

    virtual NFIDENTID QueryMaxHateObject(const NFIDENTID& self) = 0;

    virtual bool QueryMaxHateObject(const NFIDENTID& self, std::list<std::string>& HateList, const int nCount = 1) = 0;

    virtual int QueryHateValue(const NFIDENTID& self, const NFIDENTID& other) = 0;

protected:

private:
};

#endif