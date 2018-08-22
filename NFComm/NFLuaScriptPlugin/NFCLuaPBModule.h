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

#include "Dependencies/LuaIntf/LuaIntf.h"
#include "Dependencies/LuaIntf/LuaRef.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILuaPBModule.h"

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

	virtual void ImportProtoFile(const std::string& strFile);

protected:
	const void SetLuaState(lua_State* pState);

	LuaIntf::LuaRef Decode(const std::string& strMsgTypeName, const std::string& strData);
	const std::string& Encode(const std::string& strMsgTypeName, const LuaIntf::LuaRef& luaTable);

	friend class NFCLuaScriptModule;
private:
	LuaIntf::LuaRef MessageToTbl(const google::protobuf::Message& message);

	LuaIntf::LuaRef GetField(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const;
	LuaIntf::LuaRef GetRepeatedField(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const;
	// index starts from 0.
	LuaIntf::LuaRef GetRepeatedFieldElement(const google::protobuf::FieldDescriptor& field, int index) const;


	///////////////
	const std::string& TblToMessage(const std::string& strMsgTypeName, const LuaIntf::LuaRef& luaTable);


protected:
    int64_t mnTime;
    std::string strVersionCode;
	lua_State* m_pLuaState;
	google::protobuf::compiler::Importer* m_pImporter;
	google::protobuf::DynamicMessageFactory* m_pFactory;
};

#endif
