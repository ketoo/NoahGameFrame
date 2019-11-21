/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

    typedef std::function<void(const NFILogModule::NF_LOG_LEVEL, const std::string&)> LOG_HOOKER_FUNCTOR;
    typedef NF_SHARE_PTR<LOG_HOOKER_FUNCTOR> LOG_HOOKER_FUNCTOR_PTR;

    virtual bool LogElement(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strElement, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogProperty(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strProperty, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogObject(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogRecord(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol, const char* func = "", int line = 0) = 0;
    virtual bool LogRecord(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strRecord, const std::string& strDesc, const char* func = "", int line = 0) = 0;

    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strInfo, const int64_t nDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strInfo, const std::string& strDesc, const  char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;

    virtual bool LogDebug(const std::string& strLog, const char* func = "", int line = 0) = 0;
    virtual bool LogInfo(const std::string& strLog, const  char* func = "", int line = 0) = 0;
    virtual bool LogWarning(const std::string& strLog, const char* func = "", int line = 0) = 0;
    virtual bool LogError(const std::string& strLog, const char* func = "", int line = 0) = 0;
    virtual bool LogFatal(const std::string& strLog, const char* func = "", int line = 0) = 0;

    virtual bool LogDebug(const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
    virtual bool LogInfo(const std::ostringstream& stream, const  char* func = "", int line = 0) = 0;
    virtual bool LogWarning(const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
    virtual bool LogError(const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
    virtual bool LogFatal(const std::ostringstream& stream, const char* func = "", int line = 0) = 0;

    virtual bool LogDebug(const NFGUID ident, const std::string& strLog, const char* func = "", int line = 0) = 0;
    virtual bool LogInfo(const NFGUID ident, const std::string& strLog, const  char* func = "", int line = 0) = 0;
    virtual bool LogWarning(const NFGUID ident, const std::string& strLog, const char* func = "", int line = 0) = 0;
    virtual bool LogError(const NFGUID ident, const std::string& strLog, const char* func = "", int line = 0) = 0;
    virtual bool LogFatal(const NFGUID ident, const std::string& strLog, const char* func = "", int line = 0) = 0;

    virtual bool LogDebug(const NFGUID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
    virtual bool LogInfo(const NFGUID ident, const std::ostringstream& stream, const  char* func = "", int line = 0) = 0;
    virtual bool LogWarning(const NFGUID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
    virtual bool LogError(const NFGUID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
    virtual bool LogFatal(const NFGUID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;

	virtual void StackTrace() = 0;

    template<typename BaseType>
    void SetHooker(BaseType* pBase, void (BaseType::*handler)(const NFILogModule::NF_LOG_LEVEL, const std::string&))
    {
        auto functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
        LOG_HOOKER_FUNCTOR_PTR functorPtr(NF_NEW LOG_HOOKER_FUNCTOR(functor));
        return SetHooker(functorPtr);
    }

protected:
    virtual void SetHooker(LOG_HOOKER_FUNCTOR_PTR hooker) = 0;
};

#endif