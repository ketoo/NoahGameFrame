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

const LuaRef NFCLuaPBModule::Decode(const std::string& strMsgTypeName, const std::string& strData)
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

    return LuaRef(L, nullptr);
}

const std::string& NFCLuaPBModule::Encode(const std::string& strMsgTypeName, const LuaRef& luaTable)
{
    luaTable.checkTable();

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

    //TblToMsg
    return xMessageBody->SerializeAsString();
}
