// -------------------------------------------------------------------------
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-27
//    @Module           :    NFITaskModule
//    @Desc             :
// -------------------------------------------------------------------------


#ifndef NFI_TAG_MODULE_H
#define NFI_TAG_MODULE_H

#include <iostream>
#include "NFIModule.h"


class NFITagModule
    : public NFIModule
{

public:
    enum TAG_DEFINE
    {
        TAG_DEFAULT,
        TAG_BC,
        TAG_CHAT,
        TAG_TEAM,
    };

    virtual bool GetTagList(const std::string& strTag, NFList<NFGUID>& xList) = 0;
    virtual bool AddToTagList(const std::string& strTag, const NFGUID object) = 0;
    virtual bool RemoveFrmTagList(const std::string& strTag, const NFGUID object) = 0;

    virtual bool GetTagList(const TAG_DEFINE eTag, const std::string& strTag, NFList<NFGUID>& xList) = 0;
    virtual bool AddToTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFGUID object) = 0;
    virtual bool RemoveFrmTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFGUID object) = 0;

};

#endif