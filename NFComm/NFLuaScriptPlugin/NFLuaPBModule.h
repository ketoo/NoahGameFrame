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


#ifndef NF_LUA_PB_MODULE_H
#define NF_LUA_PB_MODULE_H
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include "Dependencies/LuaIntf/LuaIntf.h"
#include "Dependencies/LuaIntf/LuaRef.h"
#include "NFComm/NFCore/NFException.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILuaPBModule.h"

class NFMultiFileErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
	NFMultiFileErrorCollector() {}
	virtual ~NFMultiFileErrorCollector() {};

	// Line and column numbers are zero-based.  A line number of -1 indicates
	// an error with the entire file (e.g. "not found").
	virtual void AddError(const string& filename, int line, int column, const string& message)
	{
		std::cout << filename << " line:" << line << " column:" << column  << " message:" << message  << std::endl;
	}
};

class NFLuaPBModule
    : public NFILuaPBModule
{
public:
    NFLuaPBModule(NFIPluginManager* p)
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
	void SetLuaState(lua_State* pState);
	void PrintMessage(const google::protobuf::Message& messag, const bool bEncode);

	LuaIntf::LuaRef Decode(const std::string& strMsgTypeName, const std::string& strData);
	const std::string Encode(const std::string& strMsgTypeName, const LuaIntf::LuaRef& luaTable);

	friend class NFLuaScriptModule;

private:
	LuaIntf::LuaRef MessageToTbl(const google::protobuf::Message& message) const;

	LuaIntf::LuaRef GetField(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const;
	LuaIntf::LuaRef GetRepeatedField(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const;
	LuaIntf::LuaRef GetRepeatedFieldElement(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, int index) const;


	///////////////
	const bool TblToMessage(const LuaIntf::LuaRef& luaTable, google::protobuf::Message& message);
	
	void SetField(google::protobuf::Message& message, const std::string& sField, const LuaIntf::LuaRef& luaValue);
	void SetRepeatedField(google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, const LuaIntf::LuaRef& luaTable);
	void SetRepeatedMapField(google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, const LuaIntf::LuaRef& luaTable);
	void AddToRepeatedField(google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, const LuaIntf::LuaRef& luaValue);
	void AddToMapField(google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, const LuaIntf::LuaRef& key, const LuaIntf::LuaRef& val);
	int GetEnumValue(google::protobuf::Message& message, const LuaIntf::LuaRef& luaValue, const google::protobuf::FieldDescriptor* field) const;

protected:
    int64_t mnTime;
    std::string strVersionCode;
	lua_State* m_pLuaState;

	NFMultiFileErrorCollector mErrorCollector;
	google::protobuf::compiler::DiskSourceTree mSourceTree;
	google::protobuf::compiler::Importer* m_pImporter;
	google::protobuf::DynamicMessageFactory* m_pFactory;
};

#endif
