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


bool NFCNoSqlModule::Init()
{
	mLastCheckTime = 0;
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

bool NFCNoSqlModule::Enable()
{
	return false;
}

bool NFCNoSqlModule::Busy()
{
	return false;
}

bool NFCNoSqlModule::KeepLive()
{
	return false;
}

bool NFCNoSqlModule::Execute()
{
	if (mLastCheckTime + 10 > pPluginManager->GetNowTime())
	{
		return false;
	}
	mLastCheckTime = pPluginManager->GetNowTime();

	NF_SHARE_PTR<NFIRedisClient> xNosqlDriver = this->mxNoSqlDriver.First();
	while (xNosqlDriver)
	{
		if (!xNosqlDriver->Enable())
		{
			//m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), xNosqlDriver->GetIP(), xNosqlDriver->GetAuthKey(), __FUNCTION__, __LINE__);

			//xNosqlDriver->ReConnect();
		}

		xNosqlDriver = this->mxNoSqlDriver.Next();
	}

	return true;
}

NF_SHARE_PTR<NFIRedisClient> NFCNoSqlModule::GetDriverBySuitRandom()
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElementBySuitRandom();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRedisClient> NFCNoSqlModule::GetDriverBySuitConsistent()
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElementBySuitConsistent();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRedisClient> NFCNoSqlModule::GetDriverBySuit(const std::string& strHash)
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElementBySuit(strHash);
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
		NF_SHARE_PTR<NFRedisClient> pNoSqlDriver(new NFRedisClient());
		pNoSqlDriver->Connect(strIP, 6379, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver(new NFRedisClient());
		pNoSqlDriver->Connect(strIP, nPort, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver(new NFRedisClient());
		pNoSqlDriver->Connect(strIP, nPort, strPass);
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

NFList<std::string> NFCNoSqlModule::GetDriverIdList()
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

NF_SHARE_PTR<NFIRedisClient> NFCNoSqlModule::GetDriver(const std::string& strID)
{
	NF_SHARE_PTR<NFIRedisClient> xDriver = mxNoSqlDriver.GetElement(strID);
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

bool NFCNoSqlModule::AUTH(const std::string & auth)
{
	return false;
}

bool NFCNoSqlModule::SelectDB(int dbnum)
{
	return false;
}

bool NFCNoSqlModule::DEL(const std::string & key)
{
	return false;
}

bool NFCNoSqlModule::EXISTS(const std::string & key)
{
	return false;
}

bool NFCNoSqlModule::EXPIRE(const std::string & key, const unsigned int secs)
{
	return false;
}

bool NFCNoSqlModule::EXPIREAT(const std::string & key, const int64_t unixTime)
{
	return false;
}

bool NFCNoSqlModule::PERSIST(const std::string & key)
{
	return false;
}

int NFCNoSqlModule::TTL(const std::string & key)
{
	return 0;
}

std::string NFCNoSqlModule::TYPE(const std::string & key)
{
	return std::string();
}

bool NFCNoSqlModule::APPEND(const std::string & key, const std::string & value, int & length)
{
	return false;
}

bool NFCNoSqlModule::DECR(const std::string & key, int64_t & value)
{
	return false;
}

bool NFCNoSqlModule::DECRBY(const std::string & key, const int64_t decrement, int64_t & value)
{
	return false;
}

bool NFCNoSqlModule::GET(const std::string & key, std::string & value)
{
	return false;
}

bool NFCNoSqlModule::GETSET(const std::string & key, const std::string & value, std::string & oldValue)
{
	return false;
}

bool NFCNoSqlModule::INCR(const std::string & key, int64_t & value)
{
	return false;
}

bool NFCNoSqlModule::INCRBY(const std::string & key, const int64_t increment, int64_t & value)
{
	return false;
}

bool NFCNoSqlModule::INCRBYFLOAT(const std::string & key, const float increment, float & value)
{
	return false;
}

bool NFCNoSqlModule::MGET(const string_vector & keys, string_vector & values)
{
	return false;
}

void NFCNoSqlModule::MSET(const string_pair_vector & values)
{
}

bool NFCNoSqlModule::SET(const std::string & key, const std::string & value)
{
	return false;
}

bool NFCNoSqlModule::SETEX(const std::string & key, const std::string & value, int time)
{
	return false;
}

bool NFCNoSqlModule::SETNX(const std::string & key, const std::string & value)
{
	return false;
}

bool NFCNoSqlModule::STRLEN(const std::string & key, int & length)
{
	return false;
}

int NFCNoSqlModule::HDEL(const std::string & key, const std::string & field)
{
	return 0;
}

int NFCNoSqlModule::HDEL(const std::string & key, const string_vector & fields)
{
	return 0;
}

bool NFCNoSqlModule::HEXISTS(const std::string & key, const std::string & field)
{
	return false;
}

bool NFCNoSqlModule::HGET(const std::string & key, const std::string & field, std::string & value)
{
	return false;
}

bool NFCNoSqlModule::HGETALL(const std::string & key, std::vector<string_pair>& values)
{
	return false;
}

bool NFCNoSqlModule::HINCRBY(const std::string & key, const std::string & field, const int by, int64_t & value)
{
	return false;
}

bool NFCNoSqlModule::HINCRBYFLOAT(const std::string & key, const std::string & field, const float by, float & value)
{
	return false;
}

bool NFCNoSqlModule::HKEYS(const std::string & key, std::vector<std::string>& fields)
{
	return false;
}

bool NFCNoSqlModule::HLEN(const std::string & key, int & number)
{
	return false;
}

bool NFCNoSqlModule::HMGET(const std::string & key, const string_vector & fields, string_vector & values)
{
	return false;
}

bool NFCNoSqlModule::HMSET(const std::string & key, const std::vector<string_pair>& values)
{
	return false;
}

bool NFCNoSqlModule::HSET(const std::string & key, const std::string & field, const std::string & value)
{
	return false;
}

bool NFCNoSqlModule::HSETNX(const std::string & key, const std::string & field, const std::string & value)
{
	return false;
}

bool NFCNoSqlModule::HVALS(const std::string & key, string_vector & values)
{
	return false;
}

bool NFCNoSqlModule::HSTRLEN(const std::string & key, const std::string & field, int & length)
{
	return false;
}

bool NFCNoSqlModule::LINDEX(const std::string & key, const int index, std::string & value)
{
	return false;
}

bool NFCNoSqlModule::LLEN(const std::string & key, int & length)
{
	return false;
}

bool NFCNoSqlModule::LPOP(const std::string & key, std::string & value)
{
	return false;
}

int NFCNoSqlModule::LPUSH(const std::string & key, const std::string & value)
{
	return 0;
}

int NFCNoSqlModule::LPUSHX(const std::string & key, const std::string & value)
{
	return 0;
}

bool NFCNoSqlModule::LRANGE(const std::string & key, const int start, const int end, string_vector & values)
{
	return false;
}

bool NFCNoSqlModule::LSET(const std::string & key, const int index, const std::string & value)
{
	return false;
}

bool NFCNoSqlModule::RPOP(const std::string & key, std::string & value)
{
	return false;
}

int NFCNoSqlModule::RPUSH(const std::string & key, const std::string & value)
{
	return 0;
}

int NFCNoSqlModule::RPUSHX(const std::string & key, const std::string & value)
{
	return 0;
}

int NFCNoSqlModule::ZADD(const std::string & key, const std::string & member, const double score)
{
	return 0;
}

bool NFCNoSqlModule::ZCARD(const std::string & key, int & nCount)
{
	return false;
}

bool NFCNoSqlModule::ZCOUNT(const std::string & key, const double start, const double end, int & nCount)
{
	return false;
}

bool NFCNoSqlModule::ZINCRBY(const std::string & key, const std::string & member, const double score, double & newScore)
{
	return false;
}

bool NFCNoSqlModule::ZRANGE(const std::string & key, const int start, const int end, string_score_vector & values)
{
	return false;
}

bool NFCNoSqlModule::ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector & values)
{
	return false;
}

bool NFCNoSqlModule::ZRANK(const std::string & key, const std::string & member, int & rank)
{
	return false;
}

bool NFCNoSqlModule::ZREM(const std::string & key, const std::string & member)
{
	return false;
}

bool NFCNoSqlModule::ZREMRANGEBYRANK(const std::string & key, const int start, const int end)
{
	return false;
}

bool NFCNoSqlModule::ZREMRANGEBYSCORE(const std::string & key, const double min, const double max)
{
	return false;
}

bool NFCNoSqlModule::ZREVRANGE(const std::string & key, const int start, const int end, string_score_vector & values)
{
	return false;
}

bool NFCNoSqlModule::ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector & values)
{
	return false;
}

bool NFCNoSqlModule::ZREVRANK(const std::string & key, const std::string & member, int & rank)
{
	return false;
}

bool NFCNoSqlModule::ZSCORE(const std::string & key, const std::string & member, double & score)
{
	return false;
}

void NFCNoSqlModule::FLUSHALL()
{
}

void NFCNoSqlModule::FLUSHDB()
{
}
