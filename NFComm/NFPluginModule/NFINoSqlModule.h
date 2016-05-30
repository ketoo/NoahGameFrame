// -------------------------------------------------------------------------
//    @FileName         :    NFINoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFINoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFI_NOSQL_MODULE_H
#define NFI_NOSQL_MODULE_H

#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFINoSqlDriver
{
public:
	virtual ~NFINoSqlDriver() {}

	virtual const bool Connect(const std::string& strDns, const int nPort = 6379, const std::string& strAuthKey = "") = 0;
	virtual const bool Enable() = 0;

	virtual const std::string& GetIP() = 0;
	virtual const int GetPort() = 0;
	virtual const std::string&  GetAuthKey() = 0;

	virtual const bool Del(const std::string& strKey) = 0;
	virtual const bool Exists(const std::string& strKey) = 0;
	virtual const bool Expire(const std::string& strKey, unsigned int nSecs) = 0;
	virtual const bool Expireat(const std::string& strKey, unsigned int nUnixTime) = 0;

	///////////////////////////////////////////////////////////

	virtual const bool Set(const std::string& strKey, const std::string& strValue) = 0;
	virtual const bool Get(const std::string& strKey, std::string& strValue) = 0;

	///////////////////////////////////////////////////////////

	//SET if Not eXists
	virtual const bool SetNX(const std::string& strKey, const std::string& strValue) = 0;
	//set key->value and set Expire time
	virtual const bool SetEX(const std::string& strKey, const std::string& strValue, const unsigned int nSeconds) = 0;

	virtual const bool HSet(const std::string& strKey, const std::string& strField, const std::string& strValue) = 0;
	virtual const bool HGet(const std::string& strKey, const std::string& strField, std::string& strValue) = 0;
	virtual const bool HMSet(const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) = 0;
	virtual const bool HMGet(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;
	
	virtual const bool HExists(const std::string& strKey, const std::string& strField) = 0;
	virtual const bool HDel(const std::string& strKey, const std::string& strField) = 0;
	virtual const bool HLength(const std::string& strKey, int& nLen) = 0;

	virtual const bool HKeys(const std::string& strKey, std::vector<std::string>& fieldVec) = 0;
	virtual const bool HValues(const std::string& strKey, std::vector<std::string>& valueVec) = 0;
	virtual const bool HGetAll(const std::string& strKey, std::vector<std::pair<std::string, std::string> >& valueVec) = 0;

	///////////////默认：大到小排序 ////////////////////////////////////////////
	// sorted set系列
	virtual const bool ZAdd(const std::string& strKey, const double nScore, const std::string& strData) = 0;
	virtual const bool ZIncrBy(const std::string& strKey, const std::string& strMember, const int nIncrement) = 0;
	// 移除key中的成员member
	virtual const bool ZRem(const std::string& strKey, const std::string& strMember) = 0;
	virtual const bool ZRemRangeByRank(const std::string& strKey, const int nStart, const int nStop) = 0;
	virtual const bool ZRemRangeByScore(const std::string& strKey, const int nMin, const int nMax) = 0;

	// 返回有序集 key 中，成员 member 的 score 值
	virtual const bool ZScore(const std::string& strKey, const std::string& strMember, int& nScore) = 0;

	//累count.得到个数
	virtual const bool ZCard(const std::string& strKey, int& nCount) = 0;
	// score 值在 min 和 max 之间的成员的数量
	virtual const bool ZCount(const std::string& strKey, const int nMin, const int nMax, int& nCount) = 0;
	
	// 返回key中指定区间内的成员, 包含分数:大到小排序
	virtual const bool ZRevRange(const std::string& strKey, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;
	virtual const bool ZRevRank(const std::string& strKey, const std::string& strMember, int& nRank) = 0;
	virtual const bool ZRangeByScore(const std::string& strKey, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;

	///////////////////////////////////////////////////////////
	//push/pop form back of list
	//mpush. mpop
	virtual const bool ListPush(const std::string& strKey, const std::string& strValue) = 0;
	virtual const bool ListPop(const std::string& strKey, std::string& strValue) = 0;

	//>= star, < end
	virtual const bool ListRange(const std::string& strKey, const int nStar, const int nEnd, std::vector<std::string>& xList) = 0;
	virtual const bool ListLen(const std::string& strKey, int& nLength) = 0;

	virtual const bool ListIndex(const std::string& strKey, const int nIndex, std::string& strValue) = 0;
	virtual const bool ListRem(const std::string& strKey, const int nCount, const std::string& strValue) = 0;
	virtual const bool ListSet(const std::string& strKey, const int nCount, const std::string& strValue) = 0;
	virtual const bool ListTrim(const std::string& strKey, const int nStar, const int nEnd) = 0;
};

class NFINoSqlModule
    : public NFILogicModule
{
public:

    virtual NFINoSqlDriver* GetDriver() = 0;
    virtual bool ConnectSql(const std::string& strIP) = 0;
	virtual bool ConnectSql(const std::string& strIP, const int nPort, const std::string& strPass) = 0;
};

#endif