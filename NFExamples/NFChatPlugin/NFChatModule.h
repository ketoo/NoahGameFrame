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

#ifndef NF_CHAT_MODULE_H
#define NF_CHAT_MODULE_H

#include <memory>
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
////////////////////////////////////////////////////////////////////////////

class NFIChatModule
    : public NFIModule
 {

};

class NFChatModule
    : public NFIChatModule
{
public:
	NFChatModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init() override;
    virtual bool Shut() override;
    virtual bool Execute() override;

    virtual bool AfterInit() override;

private:
	void OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

    //////////////////////////////////////////////////////////////////////////
    NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFIKernelModule* m_pKernelModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};
#endif
