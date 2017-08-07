// -------------------------------------------------------------------------
//    @FileName         :    NFILogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOG_MODULE_H
#define NFI_LOG_MODULE_H

#include "NFIModule.h"

class NFILogModule
    : public NFIModule
{

public:

    enum NF_LOG_LEVEL
    {
        NLL_DEBUG_NORMAL,
		NLL_INFO_NORMAL,
        NLL_WARING_NORMAL,
        NLL_ERROR_NORMAL,
        NLL_FATAL_NORMAL,
    };

    virtual bool LogElement(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strElement, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogProperty(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strProperty, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogObject(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogRecord(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol, const char* func = "", int line = 0) = 0;
    virtual bool LogRecord(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strRecord, const std::string& strDesc, const char* func = "", int line = 0) = 0;

    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strInfo, const int64_t nDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strInfo, const std::string& strDesc, const  char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;

};

#endif