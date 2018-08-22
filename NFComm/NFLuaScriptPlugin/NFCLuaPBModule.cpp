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


#include <assert.h>
#include "NFCLuaPBModule.h"
#include "NFLuaScriptPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

bool NFCLuaPBModule::Awake()
{
	mnTime = pPluginManager->GetNowTime();

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
    m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
    m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
    m_pNetModule = pPluginManager->FindModule<NFINetModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();

   
	return true;
}
NFCLuaPBModule::Init()
{

    return true;
}

bool NFCLuaPBModule::AfterInit()
{

    return true;
}

bool NFCLuaPBModule::Shut()
{
    return true;
}

bool NFCLuaPBModule::ReadyExecute()
{
	return true;
}

bool NFCLuaPBModule::Execute()
{
    return true;
}

bool NFCLuaPBModule::BeforeShut()
{
    return true;
}

void NFCLuaPBModule::ImportProtoFile(const std::string & strFile)
{
	const google::protobuf::FileDescriptor* pDesc = mImporter.Import(strFile);
	if (pDesc) return;
}

const LuaIntf::LuaRef NFCLuaPBModule::Decode(const std::string& strMsgTypeName, const std::string& strData)
{
const google::protobuf::Descriptor* pDesc = mImporter.pool()->FindMessageTypeByName(strMsgTypeName);
    if (!pDesc)
    {
        return "";
    }

    const google::protobuf::Message* pProtoType = mFactory.GetPrototype(pDesc);
    if (!pProtoType)
    {
        return "";
    }

    //GC
    std::shared_ptr<google::protobuf::Message> xMessageBody(pProtoType->New());

    if (xMessageBody->ParseFromString(strData))
    {
        //MsgToTbl
    }

    return LuaIntf::LuaRef(L, nullptr);
}

const std::string& NFCLuaPBModule::Encode(const std::string& strMsgTypeName, const LuaIntf::LuaRef& luaTable)
{
    luaTable.checkTable();

    const google::protobuf::Descriptor* pDesc = mImporter.pool()->FindMessageTypeByName(strMsgTypeName);
    if (!pDesc)
    {
        return NULL_STR;
    }

    const google::protobuf::Message* pProtoType = mFactory.GetPrototype(pDesc);
    if (!pProtoType)
    {
        return NULL_STR;
    }

    //GC
    std::shared_ptr<google::protobuf::Message> xMessageBody(pProtoType->New());

    //TblToMsg
    return xMessageBody->SerializeAsString();
}

const LuaIntf::LuaRef NFCLuaPBModule::MessageToTbl(const std::string & strMsgTypeName, const google::protobuf::Message& message)
{
	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	assert(pDesc);

	std::unordered_set<const google::protobuf::OneofDescriptor*> oneofDescSet;
	LuaIntf::LuaRef tbl = LuaIntf::LuaRef::createTable(&m_rLuaState);

	int nField = pDesc->field_count();
	for (int i = 0; i < nField; ++i)
	{
		const google::protobuf::FieldDescriptor* pField = pDesc->field(i);
		assert(pField);
		const google::protobuf::OneofDescriptor* pOneof = pField->containing_oneof();
		if (pOneof)
		{
			oneofDescSet.insert(pOneof);
			continue;  // Oneof field should not set default value.
		}
		tbl[pField->name()] = GetField(*pField);
	}

	// Set oneof fields.
	for (const OneofDescriptor* pOneof : oneofDescSet)
	{
		const FieldDescriptor* pField = m_pRefl->
			GetOneofFieldDescriptor(m_msg, pOneof);
		if (pField)
			tbl[pField->name()] = GetField(*pField);
	}

	return tbl;

	return LuaIntf::LuaRef();
}

LuaIntf::LuaRef NFCLuaPBModule::GetField(const google::protobuf::FieldDescriptor & field) const
{
	if (field.is_repeated())
	{
		// returns (TableRef, "") or (nil, error_string)
		return GetRepeatedField(field);
	}

	lua_State* L = nullptr;// &m_rLuaState;
	google::protobuf::FieldDescriptor::CppType eCppType = field.cpp_type();
	switch (eCppType)
	{
		// Scalar field always has a default value.
	case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		return LuaRefValue(L, m_pRefl->GetInt32(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		return LuaRefValue(L, m_pRefl->GetInt64(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		return LuaRefValue(L, m_pRefl->GetUInt32(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		return LuaRefValue(L, m_pRefl->GetUInt64(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		return LuaRefValue(L, m_pRefl->GetDouble(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		return LuaRefValue(L, m_pRefl->GetFloat(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		return LuaRefValue(L, m_pRefl->GetBool(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		return LuaRefValue(L, m_pRefl->GetEnumValue(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		return LuaRefValue(L, m_pRefl->GetString(m_msg, &field));
	case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
	{
		// For message field, the default value is null.
		if (m_pRefl->HasField(m_msg, &field))
		{
			const google::protobuf::Message& subMsg = m_pRefl->GetMessage(m_msg, &field);
			return MessageToTbl(*L, subMsg).ToTbl();
		}
		return  LuaIntf::LuaRef(L, nullptr);
	}
		
	default:
		break;
	}
}

LuaIntf::LuaRef NFCLuaPBModule::GetRepeatedField(const google::protobuf::FieldDescriptor & field) const
{
	return LuaIntf::LuaRef();
}

LuaIntf::LuaRef NFCLuaPBModule::GetRepeatedFieldElement(const google::protobuf::FieldDescriptor & field, int index) const
{
	return LuaIntf::LuaRef();
}

const std::string & NFCLuaPBModule::TblToMessage(const std::string & strMsgTypeName, const LuaIntf::LuaRef & luaTable)
{
	// TODO: insert return statement here
}
