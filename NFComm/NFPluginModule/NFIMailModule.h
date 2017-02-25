// -------------------------------------------------------------------------
//    @FileName         :   NFIMailModule.h
//    @Author           :   NickYang
//    @Date             :   2012/12/15
//    @Module           :   NFIMailModule
//
// -------------------------------------------------------------------------

#ifndef NFI_MAIL_MODULE_H
#define NFI_MAIL_MODULE_H

#include "NFIModule.h"

class NFIMailModule : public NFIModule
{
public:
    virtual bool SendMail(const NFGUID& reciever, const int nMailType, const NFDataList& xItemList, const std::string& strNailContent = NULL_STR) = 0;
    virtual int OnDrawMail(const NFGUID& self, const NFDataList& var) = 0;
    virtual int OnOpenMail(const NFGUID& self, const NFDataList& var) = 0;
    virtual int OnDeleteMail(const NFGUID& self, const NFDataList& var) = 0;
};

#endif // !NFI_MAIL_MODULE_H