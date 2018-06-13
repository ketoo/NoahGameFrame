// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataNoSqlModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCNoSqlModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFIPluginManager* xPluginManager;
NFCNoSqlModule::NFCNoSqlModule(NFIPluginManager* p)
{
	xPluginManager = p;
	pPluginManager = p;
}

NFCNoSqlModule::~NFCNoSqlModule()
{

}

void YieldFunction()
{
	xPluginManager->YieldCo();
}

void StartFunction()
{
    //xPluginManager->StartCoroutine();
}

bool NFCNoSqlModule::Init()
{
	mLastCheckTime = 0;

    redis::YieldFunction = &YieldFunction;
    redis::StartFunction = &StartFunction;

	return true;
}

bool NFCNoSqlModule::Shut()
{

	return true;
}

bool NFCNoSqlModule::AfterInit()
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
				m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

			}
			else
			{
				std::ostringstream strLog;
				strLog << "Cannot connect NoSqlServer[" << strIP << "], Port = " << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
			}
		}
	}

	return true;
}

bool NFCNoSqlModule::Execute()
{
	if (mLastCheckTime + 10 > pPluginManager->GetNowTime())
	{
		return false;
	}
	mLastCheckTime = pPluginManager->GetNowTime();

	NF_SHARE_PTR<NFINoSqlDriver> xNosqlDriver = this->mxNoSqlDriver.First();
	while (xNosqlDriver)
	{
		if (!xNosqlDriver->Enable())
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), xNosqlDriver->GetIP(), xNosqlDriver->GetAuthKey(), __FUNCTION__, __LINE__);

			xNosqlDriver->ReConnect();
		}

		xNosqlDriver = this->mxNoSqlDriver.Next();
	}

	return true;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuitRandom()
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuitRandom();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuitConsistent()
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuitConsistent();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuit(const std::string& strHash)
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuit(strHash);
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

/*
NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuit(const int nHash)
{
return mxNoSqlDriver.GetElementBySuit(nHash);
}
*/
bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
		pNoSqlDriver->Connect(strIP, 6379, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
		pNoSqlDriver->Connect(strIP, nPort, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
		pNoSqlDriver->Connect(strIP, nPort, strPass);
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

NFList<std::string> NFCNoSqlModule::GetDriverIdList()
{
	NFList<std::string> lDriverIdList;
	std::string strDriverId;
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = mxNoSqlDriver.First(strDriverId);
	while (pDriver)
	{
		lDriverIdList.Add(strDriverId);
		pDriver = mxNoSqlDriver.Next(strDriverId);
	}
	return lDriverIdList;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriver(const std::string& strID)
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElement(strID);
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

bool NFCNoSqlModule::RemoveConnectSql(const std::string& strID)
{
	return mxNoSqlDriver.RemoveElement(strID);
}

const bool NFCNoSqlModule::Del(const std::string &strKey)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->Del(strKey);
	}

	return false;
}

const bool NFCNoSqlModule::Exists(const std::string &strKey)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->Exists(strKey);
	}

	return false;
}

const bool NFCNoSqlModule::Expire(const std::string &strKey, unsigned int nSecs)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->Expire(strKey, nSecs);
	}

	return false;
}

const bool NFCNoSqlModule::Expireat(const std::string &strKey, unsigned int nUnixTime)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->Expireat(strKey, nUnixTime);
	}

	return false;
}

const bool NFCNoSqlModule::Set(const std::string &strKey, const std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->Set(strKey, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::Get(const std::string &strKey, std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->Get(strKey, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::SetNX(const std::string &strKey, const std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->SetNX(strKey, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::SetEX(const std::string &strKey, const std::string &strValue, const unsigned int nSeconds)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->SetEX(strKey, strValue, nSeconds);
	}

	return false;
}

const bool NFCNoSqlModule::HSet(const std::string &strKey, const std::string &strField, const std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HSet(strKey, strField, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::HGet(const std::string &strKey, const std::string &strField, std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HGet(strKey, strField, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::HMSet(const std::string &strKey, const std::vector<std::string> &fieldVec,
								 const std::vector<std::string> &valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HMSet(strKey, fieldVec, valueVec);
	}

	return false;
}

const bool NFCNoSqlModule::HMGet(const std::string &strKey, const std::vector<std::string> &fieldVec,
								 std::vector<std::string> &valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HMGet(strKey, fieldVec, valueVec);
	}

	return false;
}

const bool NFCNoSqlModule::HExists(const std::string &strKey, const std::string &strField)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HExists(strKey, strField);
	}

	return false;
}

const bool NFCNoSqlModule::HDel(const std::string &strKey, const std::string &strField)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HDel(strKey, strField);
	}

	return false;
}

const bool NFCNoSqlModule::HLength(const std::string &strKey, int &nLen)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HLength(strKey, nLen);
	}

	return false;
}

const bool NFCNoSqlModule::HKeys(const std::string &strKey, std::vector<std::string> &fieldVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HKeys(strKey, fieldVec);
	}

	return false;
}

const bool NFCNoSqlModule::HValues(const std::string &strKey, std::vector<std::string> &valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HValues(strKey, valueVec);
	}

	return false;
}

const bool
NFCNoSqlModule::HGetAll(const std::string &strKey, std::vector<std::pair<std::string, std::string> > &valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->HGetAll(strKey, valueVec);
	}

	return false;
}

const bool NFCNoSqlModule::ZAdd(const std::string &strKey, const double nScore, const std::string &strMember)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZAdd(strKey, nScore, strMember);
	}

	return false;
}

const bool NFCNoSqlModule::ZIncrBy(const std::string &strKey, const std::string &strMember, const double nIncrement)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZIncrBy(strKey, strMember, nIncrement);
	}

	return false;
}

const bool NFCNoSqlModule::ZRem(const std::string &strKey, const std::string &strMember)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRem(strKey, strMember);
	}

	return false;
}

const bool NFCNoSqlModule::ZRemRangeByRank(const std::string &strKey, const int nStart, const int nStop)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRemRangeByRank(strKey, nStart, nStop);
	}

	return false;
}

const bool NFCNoSqlModule::ZRemRangeByScore(const std::string &strKey, const int nMin, const int nMax)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRemRangeByScore(strKey, nMin, nMax);
	}

	return false;
}

const bool NFCNoSqlModule::ZScore(const std::string &strKey, const std::string &strMember, double &nScore)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZScore(strKey, strMember, nScore);
	}

	return false;
}

const bool NFCNoSqlModule::ZCard(const std::string &strKey, int &nCount)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZCard(strKey, nCount);
	}

	return false;
}

const bool NFCNoSqlModule::ZRank(const std::string &strKey, const std::string &strMember, int &nRank)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRank(strKey, strMember, nRank);
	}

	return false;
}

const bool NFCNoSqlModule::ZCount(const std::string &strKey, const int nMin, const int nMax, int &nCount)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZCount(strKey, nMin, nMax, nCount);
	}

	return false;
}

const bool NFCNoSqlModule::ZRevRange(const std::string &strKey, const int nStart, const int nStop,
									 std::vector<std::pair<std::string, double> > &memberScoreVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRevRange(strKey, nStart, nStop, memberScoreVec);
	}

	return false;
}

const bool NFCNoSqlModule::ZRevRank(const std::string &strKey, const std::string &strMember, int &nRank)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRevRank(strKey, strMember, nRank);
	}

	return false;
}

const bool NFCNoSqlModule::ZRange(const std::string &strKey, const int nStartIndex, const int nEndIndex,
								  std::vector<std::pair<std::string, double> > &memberScoreVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRange(strKey, nStartIndex, nEndIndex, memberScoreVec);
	}

	return false;
}

const bool NFCNoSqlModule::ZRangeByScore(const std::string &strKey, const int nMin, const int nMax,
										 std::vector<std::pair<std::string, double> > &memberScoreVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ZRangeByScore(strKey, nMin, nMax, memberScoreVec);
	}

	return false;
}

const bool NFCNoSqlModule::ListPush(const std::string &strKey, const std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListPush(strKey, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::ListPop(const std::string &strKey, std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListPop(strKey, strValue);
	}

	return false;
}

const bool
NFCNoSqlModule::ListRange(const std::string &strKey, const int nStar, const int nEnd, std::vector<std::string> &xList)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListRange(strKey, nStar, nEnd, xList);
	}

	return false;
}

const bool NFCNoSqlModule::ListLen(const std::string &strKey, int &nLength)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListLen(strKey, nLength);
	}

	return false;
}

const bool NFCNoSqlModule::ListIndex(const std::string &strKey, const int nIndex, std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListIndex(strKey, nIndex, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::ListRem(const std::string &strKey, const int nCount, const std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListRem(strKey, nCount, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::ListSet(const std::string &strKey, const int nCount, const std::string &strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListSet(strKey, nCount, strValue);
	}

	return false;
}

const bool NFCNoSqlModule::ListTrim(const std::string &strKey, const int nStar, const int nEnd)
{
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = GetDriverBySuit(strKey);
	if (xNoSqlDriver && xNoSqlDriver->Enable())
	{
		while (xNoSqlDriver->Busy())
		{
			YieldCo();
		}

		return xNoSqlDriver->ListTrim(strKey, nStar, nEnd);
	}

	return false;
}
