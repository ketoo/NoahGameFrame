//
// Author: LUSHENG HUANG Created on 18/11/17.
//
#include <assert.h>
#include "NFRedisTester.h"

NFRedisTester::NFRedisTester()
{
    mxRedisClient.ConnectTo("127.0.0.1", 6379, "NoahGameFrame");
}

void NFRedisTester::RunTester()
{
    mxRedisClient.FLUSHDB();
	TestKey();
	TestString();
	TestList();
	TestServer();
    TestHash();
    TestSet();
    TestSort();
}

void NFRedisTester::TestHash()
{
	/*
	NF_SHARE_PTR<NFRedisResult> HINCRBY(const std::string& key, const std::string& field, const int by);
	NF_SHARE_PTR<NFRedisResult> HINCRBYFLOAT(const std::string& key, const std::string& field, const float by);

	NF_SHARE_PTR<NFRedisResult> HSETNX(const std::string& key, const std::string& field, const std::string& value);
	//NF_SHARE_PTR<NFRedisResult> HSCAN(const std::string& key, const std::string& field);
	*/

	std::string strKey = "TestHash::TestHash";
	std::string strField1 = "TestField1";
	std::string strField2 = "TestField2";
	std::string strField3 = "TestField3";
	std::string strField4 = "TestField4";
	std::string strField5 = "TestField5";
	std::string strValue1 = "12TestHash231";
	std::string strValue2 = "12TestHash232222";
	std::string strValue3 = "12TestHash23r33333";
	std::string strValue4 = "12TestHash23r4444";
	std::string strValue5 = "12TestHash23r5555";

	string_vector fields;
	fields.push_back(strField1);
	fields.push_back(strField2);
	fields.push_back(strField3);
	fields.push_back(strField4);
	fields.push_back(strField5);

	string_vector values;
	values.push_back(strValue1);
	values.push_back(strValue2);
	values.push_back(strValue3);
	values.push_back(strValue4);
	values.push_back(strValue5);

	std::vector<string_pair> pair_values;
	pair_values.push_back(string_pair(fields[0], values[0]));
	pair_values.push_back(string_pair(fields[1], values[1]));
	pair_values.push_back(string_pair(fields[2], values[2]));
	pair_values.push_back(string_pair(fields[3], values[3]));
	pair_values.push_back(string_pair(fields[4], values[4]));


	assert(mxRedisClient.DEL(strKey) == false);

	assert(mxRedisClient.HSET(strKey, strField1, strValue1) == true);

	std::string strHGET;
	assert(mxRedisClient.HGET(strKey, strField1, strHGET) == true);
	assert(strHGET == strValue1);
	
	assert(mxRedisClient.HEXISTS(strKey, strField1) == true);
	assert(mxRedisClient.HDEL(strKey, strField1) == true);
	assert(mxRedisClient.HEXISTS(strKey, strField1) == false);
	assert(mxRedisClient.HMSET(strKey, pair_values) == true);
	
	int nHLEN;
	assert(mxRedisClient.HLEN(strKey, nHLEN) == true);
	assert(nHLEN == pair_values.size());

	string_vector getValue;
	assert(mxRedisClient.HMGET(strKey, fields, getValue) == true);
	assert(fields.size() == getValue.size());
	for (int i = 0; i < values.size(); ++i)
	{
		assert(values[i] == getValue[i]);
	}

	std::vector<string_pair> get_pair_values;
	assert(mxRedisClient.HGETALL(strKey, get_pair_values) == true);
	assert(get_pair_values.size() == fields.size());
	for (int i = 0; i < fields.size(); ++i)
	{
		assert(fields[i] == get_pair_values[i].first);
		assert(values[i] == get_pair_values[i].second);
	}

	std::vector<std::string> get_keys;
	assert(mxRedisClient.HKEYS(strKey, get_keys) == true);
	assert(get_keys.size() == fields.size());
	for (int i = 0; i < fields.size(); ++i)
	{
		assert(fields[i] == get_keys[i]);
	}

	std::vector<std::string> get_valuess;
	assert(mxRedisClient.HVALS(strKey, get_valuess) == true);
	assert(get_valuess.size() == values.size());
	for (int i = 0; i < values.size(); ++i)
	{
		assert(values[i] == get_valuess[i]);
	}

	for (int i = 0; i < fields.size(); ++i)
	{
		int nHSTRLEN;
		assert(mxRedisClient.HSTRLEN(strKey, fields[i], nHSTRLEN) == true);
		assert(nHSTRLEN == get_valuess[i].length());
	}
}

void NFRedisTester::TestKey()
{
	std::string strKey = "NFRedisTester::TestKey";
	std::string strValue = "1232321123r34234";

	assert(mxRedisClient.SET(strKey, strValue) == true);

	assert(mxRedisClient.TYPE(strKey) == "string");

	assert(mxRedisClient.DEL(strKey) == true);
	assert(mxRedisClient.EXISTS(strKey) == false);

	assert(mxRedisClient.SET(strKey, strValue) == true);
	assert(mxRedisClient.EXISTS(strKey) == true);

	assert(mxRedisClient.EXPIRE(strKey, 5) == true);

	NFSLEEP(6000);
	std::string strGET;
	assert(mxRedisClient.GET(strKey, strGET) == false);
	assert(strGET == "");

	//pRedisResult = mxRedisClient.EXPIREAT(strKey, const int64_t unixTime);
	assert(mxRedisClient.PERSIST(strKey) == false);
	assert(mxRedisClient.TTL(strKey) == -2);
	assert(mxRedisClient.TYPE(strKey) == "");

}

void NFRedisTester::TestList()
{
	/*
	NF_SHARE_PTR<NFRedisResult> RPUSHX(const std::string& key, const std::string& value);

	NF_SHARE_PTR<NFRedisResult> LSET(const std::string& key, const int index, const std::string& value);

	*/

	std::string strKey = "TestList::";
	std::vector<std::string> strList;
	strList.push_back("123232111111134");
	strList.push_back("1232321222222");
	strList.push_back("123232113333");
	strList.push_back("123444444");


	assert(mxRedisClient.DEL(strKey) == false);

	for (int i = 0; i < strList.size(); ++i)
	{
		assert(mxRedisClient.RPUSH(strKey, strList[i]) == (i + 1));
	}

	for (int i = 0; i < strList.size(); ++i)
	{
		std::string strLINDEX;
		assert(mxRedisClient.LINDEX(strKey, i, strLINDEX) == true);
		assert(strLINDEX == strList[i]);
	}

	int nLLEN;
	assert(mxRedisClient.LLEN(strKey, nLLEN) == true);
	assert(nLLEN == strList.size());

	for (int i = strList.size() - 1; i >= 0; --i)
	{
		std::string strRPOP;
		assert(mxRedisClient.RPOP(strKey, strRPOP) == true);
		assert(strRPOP == strList[i]);
	}

	assert(mxRedisClient.LLEN(strKey, nLLEN) == true);
	assert(nLLEN == 0);
	//////
	for (int i = strList.size() - 1; i >= 0; --i)
	{
		assert(mxRedisClient.LPUSH(strKey, strList[i]) == (strList.size()-i));
	}

	for (int i = 0; i < strList.size(); ++i)
	{
		std::string strLINDEX;
		assert(mxRedisClient.LINDEX(strKey, i, strLINDEX) == true);
		assert(strLINDEX == strList[i]);
	}

	assert(mxRedisClient.LLEN(strKey, nLLEN) == true);
	assert(nLLEN == strList.size());

	for (int i = 0; i < strList.size(); ++i)
	{
		std::string strLPOP;
		assert(mxRedisClient.LPOP(strKey, strLPOP) == true);
		assert(strLPOP == strList[i]);
	}

	assert(mxRedisClient.LLEN(strKey, nLLEN) == true);
	assert(nLLEN == 0);
	/////

	assert(mxRedisClient.LPUSH(strKey, strKey) == 1);

	assert(mxRedisClient.LLEN(strKey, nLLEN) == true);
	assert(nLLEN == 1);

	for (int i = 0; i < strList.size(); ++i)
	{
		assert(mxRedisClient.LPUSHX(strKey, strList[i]) == (i+2));
	}

	string_vector values;
	assert(mxRedisClient.LRANGE(strKey, 0, strList.size(), values) == true);
	assert(values.size() == strList.size() + 1);
}


void NFRedisTester::TestServer()
{

}

void NFRedisTester::TestSet()
{

}


void NFRedisTester::TestSort()
{

}

void NFRedisTester::TestString()
{

	/*
	NF_SHARE_PTR<NFRedisResult> DECR(const std::string& key);
	NF_SHARE_PTR<NFRedisResult> DECRBY(const std::string& key, const int64_t v);
	NF_SHARE_PTR<NFRedisResult> GETSET(const std::string& key, const std::string& value);
	NF_SHARE_PTR<NFRedisResult> INCR(const std::string& key);
	NF_SHARE_PTR<NFRedisResult> INCRBY(const std::string& key, const int64_t v);
	NF_SHARE_PTR<NFRedisResult> INCRBYFLOAT(const std::string& key, const float fv);

	NF_SHARE_PTR<NFRedisResult> SETEX(const std::string& key, const std::string& value, int time);
	NF_SHARE_PTR<NFRedisResult> SETNX(const std::string& key, const std::string& value);
	NF_SHARE_PTR<NFRedisResult> STRLEN(const std::string& key);
	*/
	std::string strKey = "TestString";
	std::string strValue = "1232TestString234";

	assert(mxRedisClient.SET(strKey, strValue) == true);

	std::string strGET;
	assert(mxRedisClient.GET(strKey, strGET) == true);
	assert(strGET == strValue);

	int nAPPEND;
	assert(mxRedisClient.APPEND(strKey, strValue, nAPPEND) == true);
	assert(nAPPEND == strValue.length() * 2);

	int nSTRLEN;
	assert(mxRedisClient.STRLEN(strKey, nSTRLEN) == true);
	assert(nSTRLEN == strValue.length() * 2);

	assert(mxRedisClient.STRLEN("321321", nSTRLEN) == false);

    std::cout << "test cmd:" << std::endl;

    std::vector<string_pair> vstring_pair;
    std::vector<std::string> vstringListKey;

    for (int i = 0; i < 5; ++i)
    {
        string_pair vPair;

        std::stringstream streamFirst;
        std::stringstream streamSecond;
        streamFirst << "MSET_KEY" << i;
        streamSecond << "MSET_Value" << i;
        streamFirst >> vPair.first;
        streamSecond >> vPair.second;

        vstring_pair.push_back(vPair);

        vstringListKey.push_back(vPair.first);

    }

    mxRedisClient.MSET(vstring_pair);

	std::vector<std::string> vstringListValue;
    mxRedisClient.MGET(vstringListKey, vstringListValue);
	assert(vstringListKey.size() == vstringListValue.size());

	for (int i = 0; i < vstringListKey.size(); ++i)
	{
		std::string strGET;
		assert(mxRedisClient.GET(vstringListKey[i], strGET) == true);
		assert(strGET == vstringListValue[i]);
	}

}

void NFRedisTester::Execute()
{
    mxRedisClient.Execute();
}
