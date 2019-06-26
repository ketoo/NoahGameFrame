/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "NFBigMapRedisModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"

NFBigMapRedisModule::NFBigMapRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

NFBigMapRedisModule::~NFBigMapRedisModule()
{

}

bool NFBigMapRedisModule::Init()
{
	return true;
}

bool NFBigMapRedisModule::Shut()
{

	return true;
}

bool NFBigMapRedisModule::AfterInit()
{
	
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pClanRedisModule = pPluginManager->FindModule<NFIClanRedisModule>();

	return true;
}

bool NFBigMapRedisModule::Execute()
{
	return true;
}

std::string NFBigMapRedisModule::GetGridBaseKey()
{
	return "GridBaseKey";
}

std::string NFBigMapRedisModule::GetGridLeaveMsgKey(const std::string&strGridID)
{
	return "GridLeaveMsgKey_" + strGridID;
}

std::string NFBigMapRedisModule::GetGridWarHistoryKey(const std::string&strGridID)
{
	return "GridWarHistoryKey_" + strGridID;
}

std::string NFBigMapRedisModule::GetGridStationHistoryKey(const std::string&strGridID)
{
	return "GridStationKey_" + strGridID;
}

bool NFBigMapRedisModule::GetGridBaseInfo(const std::string&strGridID, NFMsg::BigMapGridBaseInfo& xBaseInfo)
{
	if (!m_pElementModule->ExistElement(strGridID))
	{
		return false;
	}
	
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strGridID);
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridBaseKey();
		std::string strData;
		if (pNoSqlDriver->HGET(strKey, strGridID, strData))
		{
			if (xBaseInfo.ParseFromString(strData))
			{
				return true;
			}
		}
	}

	return false;
}

bool NFBigMapRedisModule::GetGridBaseInfo(const std::vector<std::string>& strGridID, std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfoList)
{
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::vector<std::string> vValues;
		std::string strKey = GetGridBaseKey();
		std::string strData;
		if (pNoSqlDriver->HMGET(strKey, strGridID, vValues))
		{
			for (int i = 0; i < vValues.size(); ++i)
			{
				const std::string& strData = vValues[i];
				NFMsg::BigMapGridBaseInfo xBaseInfo;
				if (xBaseInfo.ParseFromString(strData))
				{
					xBaseInfoList.push_back(xBaseInfo);
				}
			}

			return true;
		}
	}

	return false;
}

bool NFBigMapRedisModule::GetGridBaseInfo(std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfo)
{
 	NF_SHARE_PTR<NFIClass> xLogicClass = m_pLogicClassModule->GetElement(NFrame::Map::ThisName());
	const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
	std::vector<std::string> vFields; 
	for (int i = 0; i < strIdList.size(); ++i)
	{
		const std::string& strId = strIdList[i];

		vFields.push_back(strId);
 	}

	return GetGridBaseInfo(vFields, xBaseInfo);
}

bool NFBigMapRedisModule::GetGridLeaveMsgInfo(const std::string&strGridID, std::vector<NFMsg::BigMapLeaveMsg>& xLeaveMsgList)
{
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridLeaveMsgKey(strGridID);
		std::vector<std::string> xValues;
		if (pNoSqlDriver->LRANGE(strKey, 0, 10, xValues))
		{
			for (int i = 0; i< xValues.size(); ++i)
			{
				NFMsg::BigMapLeaveMsg xLeaveMsgData;
				if (xLeaveMsgData.ParseFromString(xValues[i]))
				{
					xLeaveMsgList.push_back(xLeaveMsgData);
				}
			}

			return true;
		}
	}
	return false;
}

bool NFBigMapRedisModule::GetGridWarHistoryInfo(const std::string&strGridID, std::vector<NFMsg::BigMapWarHistory>& xWarHistoryList)
{
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridWarHistoryKey(strGridID);
		std::vector<std::string> xValues;
		if (pNoSqlDriver->LRANGE(strKey, 0, 10, xValues))
		{
			for (int i = 0; i< xValues.size(); ++i)
			{
				NFMsg::BigMapWarHistory xWarData;
				if (xWarData.ParseFromString(xValues[i]))
				{
					xWarHistoryList.push_back(xWarData);
				}
			}

			return true;
		}
	}

	return false;
}

bool NFBigMapRedisModule::GetGridStationInfo(const std::string& strGridID, std::vector<NFMsg::GridClanBaseInfo>& xWarHistoryList)
{
	/*
	if (!m_pElementModule->ExistElement(strGridID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridStationHistoryKey(strGridID);
		int nCount = 0;
		if (pNoSqlDriver->ZCard(strKey, nCount))
		{
			std::vector<std::pair<std::string, double> > memberScoreList;
			if (pNoSqlDriver->ZRevRange(strKey, 0, nCount, memberScoreList))
			{
				std::vector<std::pair<std::string, double> >::iterator it = memberScoreList.begin();
				for (it; it != memberScoreList.end(); it++)
				{
					std::string strClanID = it->first;
					NFGUID xClanID;
					if (xClanID.FromString(strClanID))
					{
						NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pClanRedisModule->GetClanCachePropertyInfo(xClanID);
						if (xPropertyManager)
						{
							NFGUID xClanID = xPropertyManager->GetPropertyObject(NFrame::Clan::ClanID());
							const int nLevel = xPropertyManager->GetPropertyInt(NFrame::Clan::ClanLevel());
							const int nCount = xPropertyManager->GetPropertyInt(NFrame::Clan::ClanMemeberCount());
							const int nResource = xPropertyManager->GetPropertyInt(NFrame::Clan::Clan::KingWarResource());
							const std::string& strIcon = xPropertyManager->GetPropertyString(NFrame::Clan::ClanIcon());

							NFMsg::GridClanBaseInfo xGridClanBaseInfo;
							xGridClanBaseInfo.mutable_id()->CopyFrom(NFINetModule::NFToPB(xClanID));
							xGridClanBaseInfo.set_level(nLevel);
							xGridClanBaseInfo.set_count(nCount);
							xGridClanBaseInfo.set_resource(nResource);
							xGridClanBaseInfo.set_icon(strIcon);

							xWarHistoryList.push_back(xGridClanBaseInfo);
						}
					}
				}
			}
		}
	}
	*/

	return false;
}

bool NFBigMapRedisModule::SetGridBaseInfo(const std::string&strGridID, const NFMsg::BigMapGridBaseInfo& xBaseInfo)
{
	if (!m_pElementModule->ExistElement(strGridID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridBaseKey();
		std::string strData;
		if (xBaseInfo.SerializeToString(&strData))
		{
			return pNoSqlDriver->HSET(strKey, strGridID, strData);
		}
	}

	return false;
}

bool NFBigMapRedisModule::AddGridLeaveMsgInfo(const std::string&strGridID, const NFMsg::BigMapLeaveMsg& xLeaveMsg)
{
	if (!m_pElementModule->ExistElement(strGridID))
	{
		return false;
	}

	
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridLeaveMsgKey(strGridID);
		std::string strData;
		if (xLeaveMsg.SerializeToString(&strData))
		{
			return pNoSqlDriver->LPUSH(strKey, strData);
		}
	}

	return false;
}

bool NFBigMapRedisModule::AddGridWarHistoryInfo(const std::string&strGridID, const NFMsg::BigMapWarHistory& xWarHistory)
{
	if (!m_pElementModule->ExistElement(strGridID))
	{
		return false;
	}

	
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridWarHistoryKey(strGridID);
		std::string strData;
		if (xWarHistory.SerializeToString(&strData))
		{
			//
			return pNoSqlDriver->LPUSH(strKey, strData);
		}
	}

	return false;
}

bool NFBigMapRedisModule::AddGridStationInfo(const std::string& strGridID, const NFGUID& self, const int nResource)
{
	if (!m_pElementModule->ExistElement(strGridID))
	{
		return false;
	}

	
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridStationHistoryKey(strGridID);
		return pNoSqlDriver->ZADD(strKey, self.ToString(), nResource);
	}

	return false;
}

bool NFBigMapRedisModule::RemoveGridStationInfo(const std::string& strGridID, const NFGUID& self, const int nResource)
{
	if (!m_pElementModule->ExistElement(strGridID))
	{
		return false;
	}

	
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridStationHistoryKey(strGridID);
		return pNoSqlDriver->ZREM(strKey, self.ToString());
	}

	return false;
}
