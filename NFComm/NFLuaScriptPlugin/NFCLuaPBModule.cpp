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
#include <unordered_map>
#include "NFCLuaPBModule.h"
#include "NFLuaScriptPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

bool NFCLuaPBModule::Awake()
{
	mSourceTree.MapPath("", "./");
	m_pImporter = new google::protobuf::compiler::Importer(&mSourceTree, NULL);
	m_pFactory = new google::protobuf::DynamicMessageFactory();

	mnTime = pPluginManager->GetNowTime();
   
	return true;
}

bool NFCLuaPBModule::Init()
{
    return true;
}

bool NFCLuaPBModule::AfterInit()
{

    return true;
}

bool NFCLuaPBModule::Shut()
{
	delete m_pFactory;
	m_pFactory = nullptr;

	delete m_pImporter;
	m_pImporter = nullptr;

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
	const google::protobuf::FileDescriptor* pDesc = m_pImporter->Import(strFile);
	if (pDesc) return;
}

void NFCLuaPBModule::SetLuaState(lua_State * pState)
{
	m_pLuaState = pState;
}

LuaIntf::LuaRef NFCLuaPBModule::Decode(const std::string& strMsgTypeName, const std::string& strData)
{
	const google::protobuf::Descriptor* pDescriptor = m_pImporter->pool()->FindMessageTypeByName(strMsgTypeName);
    if (!pDescriptor)
    {
		throw NFException("unknow message struct name: " + strMsgTypeName);
    }

    const google::protobuf::Message* pProtoType = m_pFactory->GetPrototype(pDescriptor);
    if (!pProtoType)
    {
		throw NFException("cannot find the message body from factory: " + strMsgTypeName);
    }

    //GC
    std::shared_ptr<google::protobuf::Message> xMessageBody(pProtoType->New());

    if (xMessageBody->ParseFromString(strData))
    {
		return MessageToTbl(*xMessageBody);
    }

    return LuaIntf::LuaRef(m_pLuaState, nullptr);
}

const std::string& NFCLuaPBModule::Encode(const std::string& strMsgTypeName, const LuaIntf::LuaRef& luaTable)
{
    luaTable.checkTable();

    const google::protobuf::Descriptor* pDesc = m_pImporter->pool()->FindMessageTypeByName(strMsgTypeName);
    if (!pDesc)
    {
        return NULL_STR;
    }

    const google::protobuf::Message* pProtoType = m_pFactory->GetPrototype(pDesc);
    if (!pProtoType)
    {
        return NULL_STR;
    }

    //GC
    std::shared_ptr<google::protobuf::Message> xMessageBody(pProtoType->New());

	return TblToMessage(luaTable, *xMessageBody);
}

LuaIntf::LuaRef NFCLuaPBModule::MessageToTbl(const google::protobuf::Message& message) const
{
	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc)
	{
		std::unordered_set<const google::protobuf::OneofDescriptor*> oneofDescSet;
		LuaIntf::LuaRef tbl = LuaIntf::LuaRef::createTable(m_pLuaState);

		int nField = pDesc->field_count();
		for (int i = 0; i < nField; ++i)
		{
			const google::protobuf::FieldDescriptor* pField = pDesc->field(i);
			if (pField)
			{
				const google::protobuf::OneofDescriptor* pOneof = pField->containing_oneof();
				if (pOneof)
				{
					oneofDescSet.insert(pOneof);
					continue;  // Oneof field should not set default value.
				}

				tbl[pField->name()] = GetField(message, pField);
			}
		}

		// Set oneof fields.
		for (const google::protobuf::OneofDescriptor* pOneof : oneofDescSet)
		{
			const google::protobuf::Reflection* pReflection = message.GetReflection();
			if (pReflection)
			{
				const google::protobuf::FieldDescriptor* pField = pReflection->GetOneofFieldDescriptor(message, pOneof);
				if (pField)
				{
					tbl[pField->name()] = GetField(message, pField);
				}
			}
		}

		return tbl;
	}


	return LuaIntf::LuaRef();
}

LuaIntf::LuaRef NFCLuaPBModule::GetField(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const
{
	if (nullptr == field)
	{
		return LuaIntf::LuaRef();
	}

	if (field->is_repeated())
	{
		// returns (TableRef, "") or (nil, error_string)
		return GetRepeatedField(message, field);
	}

	const google::protobuf::Reflection* pReflection = message.GetReflection();
	if (nullptr == pReflection)
	{
		return LuaIntf::LuaRef();
	}

	google::protobuf::FieldDescriptor::CppType eCppType = field->cpp_type();
	switch (eCppType)
	{
		// Scalar field always has a default value.
	case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetInt32(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetInt64(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetUInt32(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetUInt64(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetDouble(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetFloat(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetBool(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetEnumValue(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetString(message, field));
	case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
	{
		// For message field, the default value is null.
		if (pReflection->HasField(message, field))
		{
			const google::protobuf::Message& subMsg = pReflection->GetMessage(message, field);
			return MessageToTbl(subMsg);
		}
		return  LuaIntf::LuaRef(m_pLuaState, nullptr);
	}
		
	default:
		break;
	}


	return LuaIntf::LuaRef();
}

LuaIntf::LuaRef NFCLuaPBModule::GetRepeatedField(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const
{
	if (!field->is_repeated())
	{
		return LuaIntf::LuaRef();
	}

	const google::protobuf::Reflection* pReflection = message.GetReflection();
	if (nullptr == pReflection)
	{
		return LuaIntf::LuaRef();
	}

	LuaIntf::LuaRef tbl = LuaIntf::LuaRef::createTable(m_pLuaState);
	int nFldSize = pReflection->FieldSize(message, field);
	if (!field->is_map())
	{
		for (int index = 0; index < nFldSize; ++index)
		{
			tbl[index + 1] = GetRepeatedFieldElement(message, field, index);
		}

		return tbl;
	}

	// map
	if (field->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE)
	{
		return tbl;
	}

	for (int index = 0; index < nFldSize; ++index)
	{
		const google::protobuf::Message& entryMsg = pReflection->GetRepeatedMessage(message, field, index);
		LuaIntf::LuaRef entryTbl = MessageToTbl(message);
		const LuaIntf::LuaRef& key = entryTbl["key"];
		const LuaIntf::LuaRef& value = entryTbl["value"];
		tbl[key] = value;
	}
	return tbl;
}

LuaIntf::LuaRef NFCLuaPBModule::GetRepeatedFieldElement(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, int index) const
{
	if (!field->is_repeated())
	{
		return LuaIntf::LuaRef();
	}

	if (index < 0)
	{
		return LuaIntf::LuaRef();
	}

	const google::protobuf::Reflection* pReflection = message.GetReflection();
	if (nullptr == pReflection)
	{
		return LuaIntf::LuaRef();
	}

	if (index >= pReflection->FieldSize(message, field))
	{
		return LuaIntf::LuaRef();
	}

	google::protobuf::FieldDescriptor::CppType eCppType = field->cpp_type();
	switch (eCppType)
	{
	case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedInt32(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedInt64(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedUInt32(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedUInt64(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedDouble(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedFloat(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedBool(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedEnumValue(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		return LuaIntf::LuaRefValue(m_pLuaState, pReflection->GetRepeatedString(message, field, index));
	case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
	{
		const google::protobuf::Message& subMsg = pReflection->GetRepeatedMessage(message, field, index);
		return MessageToTbl(subMsg);
	}
	default:
		break;
	}

	return LuaIntf::LuaRef();
}

const std::string & NFCLuaPBModule::TblToMessage(const LuaIntf::LuaRef& luaTable, google::protobuf::Message& message)
{
	if (!luaTable.isTable())
	{
		return NULL_STR;
	}

	const auto itrEnd = luaTable.end();
	for (auto itr = luaTable.begin(); itr != itrEnd; ++itr)
	{
		const LuaIntf::LuaRef& key = itr.key();
		if (LuaIntf::LuaTypeID::STRING != key.type())
		{
			continue;
		}

		const string& sKey = key.toValue<string>();
		// std::cout << sKey << std::endl;
		const LuaIntf::LuaRef& val = itr.value();

		SetField(message, sKey, val);
	}

	return NULL_STR;
}

void NFCLuaPBModule::SetField(google::protobuf::Message& messag, const std::string & sField, const LuaIntf::LuaRef & luaValue)
{
}

void NFCLuaPBModule::SetRepeatedField(google::protobuf::Message& messag, const google::protobuf::FieldDescriptor * field, const LuaIntf::LuaRef & luaTable)
{
}

void NFCLuaPBModule::SetRepeatedMapField(google::protobuf::Message& messag, const google::protobuf::FieldDescriptor * field, const LuaIntf::LuaRef & luaTable)
{
}

void NFCLuaPBModule::AddToRepeatedField(google::protobuf::Message& messag, const google::protobuf::FieldDescriptor * field, const LuaIntf::LuaRef & luaValue)
{
}

void NFCLuaPBModule::AddToMapField(google::protobuf::Message& messag, const google::protobuf::FieldDescriptor * field, const LuaIntf::LuaRef & key, const LuaIntf::LuaRef & val)
{
}

int NFCLuaPBModule::GetEnumValue(google::protobuf::Message& messag, const LuaIntf::LuaRef & luaValue, const google::protobuf::FieldDescriptor * field) const
{
	return 0;
}
