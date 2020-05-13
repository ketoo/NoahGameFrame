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

#include "NFSecurityModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFSecurityModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	
	return true;
}

bool NFSecurityModule::AfterInit()
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Security::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];
			const std::string& strSecurityData = m_pElementModule->GetPropertyString(strId, NFrame::Security::SecurityData());

		}
	}

	return true;
}

const std::string NFSecurityModule::GetSecurityKey(const std::string & strAccount)
{
	return strAccount;
}

bool NFSecurityModule::VerifySecurityKey(const std::string & strAccount, const std::string & strSecurityKey)
{
	//you would implement this function by yourself
	//if (strAccount == strSecurityKey)
	{
		return true;
	}

	return false;
}

std::string NFSecurityModule::EncodeMsg(const std::string & strAccount, const std::string & strSecurityKey, const int nMessageID, const char * strMessageData, const int nLen)
{
	return std::string(strMessageData, nLen);
}

std::string NFSecurityModule::EncodeMsg(const std::string & strAccount, const std::string & strSecurityKey, const int nMessageID, const std::string & strMessageData)
{
	return strMessageData;
}

std::string NFSecurityModule::DecodeMsg(const std::string & strAccount, const std::string & strSecurityKey, const int nMessageID, const char * strMessageData, const int nLen)
{
	return std::string(strMessageData, nLen);
}

std::string NFSecurityModule::DecodeMsg(const std::string & strAccount, const std::string & strSecurityKey, const int nMessageID, const std::string & strMessageData)
{
	return strMessageData;
}

bool NFSecurityModule::Shut()
{

	return true;
}

bool NFSecurityModule::Execute()
{
	return true;
}
