/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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


#ifndef NFC_LUA_PB_MODULE_H
#define NFC_LUA_PB_MODULE_H

//just define it as 0 if you want to use luaintf with C
//#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0

#include "Dependencies/LuaIntf/LuaIntf.h"
#include "Dependencies/LuaIntf/LuaRef.h"
#include <google/protobuf/stubs/common.h>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCLuaPBModule
    : public NFILuaPBModule
{
public:
    NFCLuaPBModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

	virtual bool Awake();
	virtual bool Init();
    virtual bool Shut();
	virtual bool ReadyExecute();
	virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

	const LuaRef Decode(const std::string& strMsgTypeName, const std::string& strData);
	const std::string& Encode(const std::string& strMsgTypeName, const LuaRef& luaTable);

private:
	const LuaRef MsgToTbl(const std::string& strMsgTypeName, const std::string& strData);
	const std::string& TblToMsg(const std::string& strMsgTypeName, const LuaRef& luaTable);

protected:
    int64_t mnTime;
    std::string strVersionCode;
    LuaIntf::LuaContext mLuaContext;
	google::protobuf::compiler::Importer mImporter;
	google::protobuf::DynamicMessageFactory mFactory;
};

#endif
