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

#include <algorithm>
#include "NFNoSqlModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFIPluginManager* xPluginManager;
NFNoSqlModule::NFNoSqlModule(NFIPluginManager* p)
{
    m_bIsExecute = true;
	xPluginManager = p;
	pPluginManager = p;
}

NFNoSqlModule::~NFNoSqlModule()
{

}


bool NFNoSqlModule::Init()
{
	mLastCheckTime = 0;
	return true;
}

bool NFNoSqlModule::Shut()
{

	return true;
}

bool NFNoSqlModule::AfterInit()
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

			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::ServerID());
			const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::Port());
			const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::IP());
			const std::string& strAuth = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::Auth());

			if (this->AddConnectSql(strId, strIP, nPort, strAuth))
			{
				std::ostringstream strLog;
				strLog << "Connected NoSqlServer[" << strIP << "], Port = [" << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogInfo(strLog, __FUNCTION__, __LINE__);

			}
			else
			{
				std::ostringstream strLog;
				strLog << "Cannot connect NoSqlServer[" << strIP << "], Port = " << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogInfo(strLog, __FUNCTION__, __LINE__);
			}
		}
	}

	return true;
}

bool NFNoSqlModule::Enable()
{
	return false;
}

bool NFNoSqlModule::Busy()
{
	return false;
}

bool NFNoSqlModule::KeepLive()
{
	return false;
}

bool NFNoSqlModule::Execute()
{
	NF_SHARE_PTR<NFIRedisClient> xNosqlDriver = this->mxNoSqlDriver.First();
	while (xNosqlDriver)
	{
		xNosqlDriver->Execute();

		xNosqlDriver = this->mxNoSqlDriver.Next();
	}

	CheckConnect();
	
	return true;
}

NF_SHARE_PTR<NFIRedisClient> NFNoSqlModule::GetDriverBySuitRandom()
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElementBySuitRandom();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRedisClient> NFNoSqlModule::GetDriverBySuitConsistent()
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElementBySuitConsistent();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRedisClient> NFNoSqlModule::GetDriverBySuit(const std::string& strHash)
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElementBySuit(strHash);
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	std::ostringstream os;
	os << "GetDriverBySuit ===> NULL";
	os << strHash;

	m_pLogModule->LogError(os);
	return nullptr;
}

/*
NF_SHARE_PTR<NFIRedisClient> NFNoSqlModule::GetDriverBySuit(const int nHash)
{
return mxNoSqlDriver.GetElementBySuit(nHash);
}
*/
bool NFNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFRedisClient> pNoSqlDriver(new NFRedisClient());
		pNoSqlDriver->Connect(strIP, 6379, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver(new NFRedisClient());
		pNoSqlDriver->Connect(strIP, nPort, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver(NF_NEW NFRedisClient());
		pNoSqlDriver->Connect(strIP, nPort, strPass);
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

NFList<std::string> NFNoSqlModule::GetDriverIdList()
{
	NFList<std::string> lDriverIdList;
	std::string strDriverId;
	NF_SHARE_PTR<NFIRedisClient> pDriver = mxNoSqlDriver.First(strDriverId);
	while (pDriver)
	{
		lDriverIdList.Add(strDriverId);
		pDriver = mxNoSqlDriver.Next(strDriverId);
	}
	return lDriverIdList;
}

NF_SHARE_PTR<NFIRedisClient> NFNoSqlModule::GetDriver(const std::string& strID)
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElement(strID);
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

bool NFNoSqlModule::RemoveConnectSql(const std::string& strID)
{
	return mxNoSqlDriver.RemoveElement(strID);
}

void NFNoSqlModule::CheckConnect()
{
	static const int CHECK_TIME = 15;
	if (mLastCheckTime + CHECK_TIME > pPluginManager->GetNowTime())
	{
		return;
	}

	mLastCheckTime = pPluginManager->GetNowTime();

	NF_SHARE_PTR<NFIRedisClient> xNosqlDriver = this->mxNoSqlDriver.First();
	while (xNosqlDriver)
	{
		if (xNosqlDriver->Enable() && !xNosqlDriver->Authed())
		{
			xNosqlDriver->AUTH(xNosqlDriver->GetAuthKey());
		}
		else if (!xNosqlDriver->Enable())
		{
			//reconnect
			xNosqlDriver->ReConnect();
		}

		xNosqlDriver = this->mxNoSqlDriver.Next();
	}
}
