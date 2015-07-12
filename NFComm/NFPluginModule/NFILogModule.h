// -------------------------------------------------------------------------
//    @FileName         :    NFILogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LOG_MODULE_H_
#define _NFI_LOG_MODULE_H_

#include "NFILogicModule.h"

class NFILogModule
    : public NFILogicModule
{

public:

    enum NF_LOG_LEVEL
    {
        NLL_DEBUG_NORMAL,
        NLL_INFO_NORMAL,//无意义，只为打印
        NLL_WARING_NORMAL,
        NLL_ERROR_NORMAL,
        NLL_FATAL_NORMAL,
    };

    virtual bool LogElement(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strElement, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogProperty(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strProperty, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogObject(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strDesc, const char* func = "", int line = 0) = 0;

    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const int nDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const std::string& strDesc,const  char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;

};

#endif