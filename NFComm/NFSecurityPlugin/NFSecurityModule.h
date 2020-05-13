/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#ifndef NF_SECURITY_MODULE_H
#define NF_SECURITY_MODULE_H

#include <memory>
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFISecurityModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"

class NFSecurityModule
    : public NFISecurityModule
{
public:
	NFSecurityModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

	//when a user login NF system, as a security module, you need to grant user an key.
	virtual const std::string GetSecurityKey(const std::string& strAccount);

	//when the user want to connect to proxy, as a user need to provide the key that got in login system.
	virtual bool VerifySecurityKey(const std::string& strAccount, const std::string& strSecurityKey);

	//when the user want to post a message to proxy, the message-data need to be verified.
	virtual std::string EncodeMsg(const std::string& strAccount, const std::string& strSecurityKey, const int nMessageID, const char* strMessageData, const int nLen);
	virtual std::string EncodeMsg(const std::string& strAccount, const std::string& strSecurityKey, const int nMessageID, const std::string& strMessageData);
	virtual std::string DecodeMsg(const std::string& strAccount, const std::string& strSecurityKey, const int nMessageID, const char* strMessageData, const int nLen);
	virtual std::string DecodeMsg(const std::string& strAccount, const std::string& strSecurityKey, const int nMessageID, const std::string& strMessageData);

protected:

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
    //////////////////////////////////////////////////////////////////////////
};
#endif
