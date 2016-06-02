// -------------------------------------------------------------------------
//    @FileName			:    NFCBigMapRedisModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCBigMapRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCBigMapRedisModule.h"

NFCBigMapRedisModule::NFCBigMapRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

NFCBigMapRedisModule::~NFCBigMapRedisModule()
{

}

bool NFCBigMapRedisModule::Init()
{
	return true;
}

bool NFCBigMapRedisModule::Shut()
{

	return true;
}

bool NFCBigMapRedisModule::AfterInit()
{
	
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>("NFCNoSqlModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");

	assert(NULL != m_pNoSqlModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogicClassModule);

	return true;
}

bool NFCBigMapRedisModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	return true;
}

std::string NFCBigMapRedisModule::GetGridBaseKey()
{
	return "GridBaseKey";
}

std::string NFCBigMapRedisModule::GetGridLeaveMsgKey(const std::string&strGridID)
{
	return "GridLeaveMsgKey_" + strGridID;
}

std::string NFCBigMapRedisModule::GetGridWarHistoryKey(const std::string&strGridID)
{
	return "GridWarHistoryKey_" + strGridID;
}

bool NFCBigMapRedisModule::GetGridBaseInfo(const std::string&strGridID, NFMsg::BigMapGridBaseInfo& xBaseInfo)
{
	if (!m_pElementInfoModule->ExistElement(strGridID))
	{
		return false;
	}

	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridBaseKey();
		std::string strData;
		if (pNoSqlDriver->HGet(strKey, strGridID, strData))
		{
			if (xBaseInfo.ParseFromString(strData))
			{
				return true;
			}
		}
	}

	return false;
}

bool NFCBigMapRedisModule::GetGridBaseInfo(std::vector<NFMsg::BigMapGridBaseInfo>& xBaseInfoList)
{
	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Map");
	NFList<std::string>& xElementList = xLogicClass->GetConfigNameList();

	std::vector<std::string> vFields;
	std::string strID;
	for (xElementList.First(strID); !strID.empty(); xElementList.Next(strID))
	{
		vFields.push_back(strID);

		strID.clear();
	}


	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (pNoSqlDriver)
	{
		std::vector<std::string> vValues;
		std::string strKey = GetGridBaseKey();
		std::string strData;
		if (pNoSqlDriver->HMGet(strKey, vFields, vValues))
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

bool NFCBigMapRedisModule::GetGridLeaveMsgInfo(const std::string&strGridID, std::vector<NFMsg::BigMapLeaveMsg>& xLeaveMsgList)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridLeaveMsgKey(strGridID);
		std::vector<std::string> xValues;
		if (pNoSqlDriver->ListRange(strKey, 0, 10, xValues))
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

bool NFCBigMapRedisModule::GetGridWarHistoryInfo(const std::string&strGridID, std::vector<NFMsg::BigMapWarHistory>& xWarHistoryList)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridWarHistoryKey(strGridID);
		std::vector<std::string> xValues;
		if (pNoSqlDriver->ListRange(strKey, 0, 10, xValues))
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

bool NFCBigMapRedisModule::SetGridBaseInfo(const std::string&strGridID, const NFMsg::BigMapGridBaseInfo& xBaseInfo)
{
	if (!m_pElementInfoModule->ExistElement(strGridID))
	{
		return false;
	}

	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridBaseKey();
		std::string strData;
		if (xBaseInfo.SerializeToString(&strData))
		{
			return pNoSqlDriver->HSet(strKey, strGridID, strData);
		}
	}

	return false;
}

bool NFCBigMapRedisModule::AddGridLeaveMsgInfo(const std::string&strGridID, const NFMsg::BigMapLeaveMsg& xLeaveMsg)
{
	if (!m_pElementInfoModule->ExistElement(strGridID))
	{
		return false;
	}

	//数量限制
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridLeaveMsgKey(strGridID);
		std::string strData;
		if (xLeaveMsg.SerializeToString(&strData))
		{
			return pNoSqlDriver->ListPush(strKey, strData);
		}
	}

	return false;
}

bool NFCBigMapRedisModule::AddGridWarHistoryInfo(const std::string&strGridID, const NFMsg::BigMapWarHistory& xWarHistory)
{
	if (!m_pElementInfoModule->ExistElement(strGridID))
	{
		return false;
	}

	//数量限制
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (pNoSqlDriver)
	{
		std::string strKey = GetGridWarHistoryKey(strGridID);
		std::string strData;
		if (xWarHistory.SerializeToString(&strData))
		{
			//
			return pNoSqlDriver->ListPush(strKey, strData);
		}
	}

	return false;
}