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
    //mxRedisClient.Flush();

    TestGEO();
    TestHash();
    TestKey();
    TestList();
    TestPubSub();
    TestServer();
    TestSet();
    TestSocket();
    TestSort();
    TestString();
}

void NFRedisTester::TestGEO()
{

}

void NFRedisTester::TestHash()
{
	/*
	NFRedisResult* HINCRBY(const std::string& key, const std::string& field, const int by);
	NFRedisResult* HINCRBYFLOAT(const std::string& key, const std::string& field, const float by);

	NFRedisResult* HSETNX(const std::string& key, const std::string& field, const std::string& value);
	//NFRedisResult* HSCAN(const std::string& key, const std::string& field);
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


	NFRedisResult* pRedisResult;

	pRedisResult = mxRedisClient.DEL(strKey);

	pRedisResult = mxRedisClient.HSET(strKey, strField1, strValue1);
	assert(pRedisResult->GetRespInt() == 1);

	pRedisResult = mxRedisClient.HGET(strKey, strField1);
	assert(pRedisResult->GetRespString() == strValue1);

	pRedisResult = mxRedisClient.HEXISTS(strKey, strField1);
	assert(pRedisResult->GetRespInt() == 1);

	pRedisResult = mxRedisClient.HDEL(strKey, strField1);
	assert(pRedisResult->GetRespInt() == 1);

	pRedisResult = mxRedisClient.HEXISTS(strKey, strField1);
	assert(pRedisResult->GetRespInt() == 0);

	pRedisResult = mxRedisClient.HMSET(strKey, pair_values);
	assert(pRedisResult->GetRespString() == NFREDIS_STATUS_OK);
	
	pRedisResult = mxRedisClient.HLEN(strKey);
	assert(pRedisResult->GetRespInt() == pair_values.size());

	string_vector getValue;
	pRedisResult = mxRedisClient.HMGET(strKey, fields, getValue);
	//assert(pRedisResult->GetRespInt() == getValue.size());
	assert(fields.size() == getValue.size());
	for (int i = 0; i < values.size(); ++i)
	{
		assert(values[i] == getValue[i]);
	}

	std::vector<string_pair> get_pair_values;
	pRedisResult = mxRedisClient.HGETALL(strKey, get_pair_values);
	assert(pRedisResult->GetResultStatus() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK);
	assert(get_pair_values.size() == fields.size());
	for (int i = 0; i < fields.size(); ++i)
	{
		assert(fields[i] == get_pair_values[i].first);
		assert(values[i] == get_pair_values[i].second);
	}

	std::vector<std::string> get_keys;
	pRedisResult = mxRedisClient.HKEYS(strKey, get_keys);
	assert(pRedisResult->GetResultStatus() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK);
	assert(get_keys.size() == fields.size());
	for (int i = 0; i < fields.size(); ++i)
	{
		assert(fields[i] == get_keys[i]);
	}

	std::vector<std::string> get_valuess;
	pRedisResult = mxRedisClient.HVALS(strKey, get_valuess);
	assert(pRedisResult->GetResultStatus() == NFREDIS_RESULT_STATUS::NFREDIS_RESULT_STATUS_OK);
	assert(get_valuess.size() == values.size());
	for (int i = 0; i < values.size(); ++i)
	{
		assert(values[i] == get_valuess[i]);
	}

	for (int i = 0; i < fields.size(); ++i)
	{
		pRedisResult = mxRedisClient.HSTRLEN(strKey, fields[i]);
		assert(pRedisResult->GetRespInt() == get_valuess[i].length());
	}
}

void NFRedisTester::TestKey()
{
	std::string strKey = "NFRedisTester::TestKey";
	std::string strValue = "1232321123r34234";

	NFRedisResult* pRedisResult;

	pRedisResult = mxRedisClient.SET(strKey, strValue);
	assert(pRedisResult->GetRespString() == NFREDIS_STATUS_OK);

	pRedisResult = mxRedisClient.TYPE(strKey);
	assert(pRedisResult->GetRespString() == "string");

	pRedisResult = mxRedisClient.DEL(strKey);
	pRedisResult = mxRedisClient.EXISTS(strKey);
	assert(pRedisResult->GetRespInt() == 0);

	pRedisResult = mxRedisClient.SET(strKey, strValue);
	pRedisResult = mxRedisClient.EXISTS(strKey);
	assert(pRedisResult->GetRespInt() == 1);

	pRedisResult = mxRedisClient.EXPIRE(strKey, 5);

	NFSLEEP(6000);
	pRedisResult = mxRedisClient.GET(strKey);
	assert(pRedisResult->GetRespString() == "");

	//pRedisResult = mxRedisClient.EXPIREAT(strKey, const int64_t unixTime);
	pRedisResult = mxRedisClient.PERSIST(strKey);
	pRedisResult = mxRedisClient.TTL(strKey);
	pRedisResult = mxRedisClient.TYPE(strKey);

}

void NFRedisTester::TestList()
{

}

void NFRedisTester::TestPubSub()
{

}

void NFRedisTester::TestServer()
{

}

void NFRedisTester::TestSet()
{

}

void NFRedisTester::TestSocket()
{

}

void NFRedisTester::TestSort()
{

}

void NFRedisTester::TestString()
{

	/*
	NFRedisResult* DECR(const std::string& key);
	NFRedisResult* DECRBY(const std::string& key, const int64_t v);
	NFRedisResult* GETSET(const std::string& key, const std::string& value);
	NFRedisResult* INCR(const std::string& key);
	NFRedisResult* INCRBY(const std::string& key, const int64_t v);
	NFRedisResult* INCRBYFLOAT(const std::string& key, const float fv);

	NFRedisResult* SETEX(const std::string& key, const std::string& value, int time);
	NFRedisResult* SETNX(const std::string& key, const std::string& value);
	NFRedisResult* STRLEN(const std::string& key);
	*/
	std::string strKey = "TestString";
	std::string strValue = "1232TestString234";

    NFRedisResult* pRedisResult;

    pRedisResult = mxRedisClient.SET(strKey, strValue);
	assert(pRedisResult->GetRespString() == NFREDIS_STATUS_OK);

	pRedisResult = mxRedisClient.GET(strKey);
	assert(pRedisResult->GetRespString() == strValue);

    pRedisResult = mxRedisClient.APPEND(strKey, strValue);
	assert(pRedisResult->GetRespInt() == strValue.length() * 2);

	pRedisResult = mxRedisClient.STRLEN(strKey);
	assert(pRedisResult->GetRespInt() == strValue.length() * 2);

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
	std::cout << "test cmd:" << pRedisResult->GetCommand() << std::endl;

	std::vector<std::string> vstringListValue;
    mxRedisClient.MGET(vstringListKey, vstringListValue);
	assert(vstringListKey.size() == vstringListValue.size());

	for (int i = 0; i < vstringListKey.size(); ++i)
	{
		assert(mxRedisClient.GET(vstringListKey[i])->GetRespString() == vstringListValue[i]);
	}

}

void NFRedisTester::Execute()
{
    mxRedisClient.Execute();
}
