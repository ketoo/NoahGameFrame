/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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

#include <algorithm>
#include "NFNoSqlModule.h"
#include "NFNoSqlClusterModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFNoSqlClusterModule::NFNoSqlClusterModule(NFIPluginManager* p)
{
	executableModule = true;
	pPluginManager = p;
}

NFNoSqlClusterModule::~NFNoSqlClusterModule()
{

}


bool NFNoSqlClusterModule::Init()
{
	mLastCheckTime = 0;
	return true;
}

bool NFNoSqlClusterModule::Shut()
{

	return true;
}

bool NFNoSqlClusterModule::AfterInit()
{
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::NoSqlServer::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::ServerID());
			const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::Port());
			const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::IP());
			const std::string& strAuth = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::Auth());
/*
			if (this->AddConnectSql(strId, ip, nPort, strAuth))
			{
				std::ostringstream strLog;
				strLog << "Connected NoSqlServer[" << ip << "], Port = [" << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogInfo(strLog, __FUNCTION__, __LINE__);

			}
			else
			{
				std::ostringstream strLog;
				strLog << "Cannot connect NoSqlServer[" << ip << "], Port = " << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogInfo(strLog, __FUNCTION__, __LINE__);
			}
			*/
		}
	}

	return true;
}


bool NFNoSqlClusterModule::Execute()
{

	return true;
}

