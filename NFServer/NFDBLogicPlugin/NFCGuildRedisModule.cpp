/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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

#include "NFCGuildRedisModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"

NFCGuildRedisModule::NFCGuildRedisModule(NFIPluginManager * p)
{
    pPluginManager = p;
}

bool NFCGuildRedisModule::Init()
{
    return true;
}

bool NFCGuildRedisModule::Shut()
{
    return true;
}

bool NFCGuildRedisModule::Execute()
{
    return true;
}

bool NFCGuildRedisModule::AfterInit()
{
    m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
    m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

    return true;
}

bool NFCGuildRedisModule::ExistGuild(const std::string & strName)
{
	return false;
}

bool NFCGuildRedisModule::CreateGuild(const NFGUID & self, const std::string & strGuildName, const NFGUID & xCreater)
{
	return false;
}


