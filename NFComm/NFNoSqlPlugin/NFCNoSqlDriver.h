// -------------------------------------------------------------------------
//    @FileName			:    NFCNoSqlDriver.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-12-19 11:05
//    @Module           :    NFCNoSqlDriver
//
// -------------------------------------------------------------------------

#ifndef NFC_DATABASEDRIVER_H
#define NFC_DATABASEDRIVER_H

#ifdef _MSC_VER
#pragma warning(disable: 4244 4267 4101 4390)
#endif
#include "Dependencies/redis-cplusplus-client/redisclient.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267 4101 4390)
#endif

#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class  NFCNoSqlDriver : public NFINoSqlDriver
{
public:
    NFCNoSqlDriver();
    virtual ~NFCNoSqlDriver();

	virtual const bool Connect(const std::string& strDns, const int nPort = 6379, const std::string& strAuthKey = "");
	virtual const bool ReConnect();
	virtual const bool Enable();

	virtual const std::string& GetIP();
	virtual const int GetPort();
	virtual const std::string&  GetAuthKey();

	virtual const bool Del(const std::string& strKey);
	virtual const bool Exists(const std::string& strKey);
	virtual const bool Expire(const std::string& strKey, unsigned int nSecs);
	virtual const bool Expireat(const std::string& strKey, unsigned int nUnixTime);

	///////////////////////////////////////////////////////////

	virtual const bool Set(const std::string& strKey, const std::string& strValue);
	virtual const bool Get(const std::string& strKey, std::string& strValue);

	///////////////////////////////////////////////////////////

	//SET if Not eXists
	virtual const bool SetNX(const std::string& strKey, const std::string& strValue);
	//set key->value and set Expire time
	virtual const bool SetEX(const std::string& strKey, const std::string& strValue, const unsigned int nSeconds);

	virtual const bool HSet(const std::string& strKey, const std::string& strField, const std::string& strValue);
	virtual const bool HGet(const std::string& strKey, const std::string& strField, std::string& strValue);
	virtual const bool HMSet(const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec);
	virtual const bool HMGet(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);

	virtual const bool HExists(const std::string& strKey, const std::string& strField);
	virtual const bool HDel(const std::string& strKey, const std::string& strField);
	virtual const bool HLength(const std::string& strKey, int& nLen);

	virtual const bool HKeys(const std::string& strKey, std::vector<std::string>& fieldVec);
	virtual const bool HValues(const std::string& strKey, std::vector<std::string>& valueVec);
	virtual const bool HGetAll(const std::string& strKey, std::vector<std::pair<std::string, std::string> >& valueVec);

	/////////////
	
	virtual const bool ZAdd(const std::string& strKey, const double nScore, const std::string& strData);
	virtual const bool ZIncrBy(const std::string& strKey, const std::string& strMember, const double nIncrement);
	
	virtual const bool ZRem(const std::string& strKey, const std::string& strMember);
	virtual const bool ZRemRangeByRank(const std::string& strKey, const int nStart, const int nStop);
	virtual const bool ZRemRangeByScore(const std::string& strKey, const int nMin, const int nMax);

	
	virtual const bool ZScore(const std::string& strKey, const std::string& strMember, double& nScore);

	
	virtual const bool ZCard(const std::string& strKey, int& nCount);
	
	virtual const bool ZCount(const std::string& strKey, const int nMin, const int nMax, int& nCount);

	
	virtual const bool ZRevRange(const std::string& strKey, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec);
	virtual const bool ZRevRank(const std::string& strKey, const std::string& strMember, int& nRank);
	virtual const bool ZRangeByScore(const std::string& strKey, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec);

	///////////////////////////////////////////////////////////
	//push form back of the list
	//pop form head of the list
	virtual const bool ListPush(const std::string& strKey, const std::string& strValue);
	virtual const bool ListPop(const std::string& strKey, std::string& strValue);

	//>= star, < end
	virtual const bool ListRange(const std::string& strKey, const int nStar, const int nEnd, std::vector<std::string>& xList);
	virtual const bool ListLen(const std::string& strKey, int& nLength);

	virtual const bool ListIndex(const std::string& strKey, const int nIndex, std::string& strValue);
	virtual const bool ListRem(const std::string& strKey, const int nCount, const std::string& strValue);
	virtual const bool ListSet(const std::string& strKey, const int nCount, const std::string& strValue);
	virtual const bool ListTrim(const std::string& strKey, const int nStar, const int nEnd);

protected:

private:
	std::string mstrNoExistKey;
	bool mbEnable;
    redis::client* m_pNoSqlClient;

	int nPort;
	std::string strIP;
	std::string strAuthKey;
};

#endif