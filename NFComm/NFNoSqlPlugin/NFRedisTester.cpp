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
    TestHash();
    TestSet();
    TestSort();
}

void NFRedisTester::TestHash()
{
	int64_t nnn;
	assert(mxRedisClient.HINCRBY("12123ddd121wssdsdsdd", "121212", 13, nnn) == true);
	assert(nnn == 13);

	float fval;
	assert(mxRedisClient.HINCRBYFLOAT("12123dd323ssss123ddd", "12444441212", 3.0f, fval) == true);

	assert(mxRedisClient.HSETNX("12123ddd121wssdsdsdd", "12444441212", "121212") == true);
	assert(mxRedisClient.HSETNX("12123ddd121wssdsdsdd", "12444441212", "121212") == false);


	//NF_SHARE_PTR<NFRedisResult> HSCAN(const std::string& key, const std::string& field);

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
	int64_t nnn;
	assert(mxRedisClient.INCRBY("12123ddddd", 13, nnn) == true);
	assert(nnn == 13);

	float fval;
	assert(mxRedisClient.INCRBYFLOAT("12123dd323123ddd", 3.0f, fval) == true);

	assert(mxRedisClient.SETNX("12123dd323123ddd", "121212") == false);
	assert(mxRedisClient.SETNX("124422dd1212", "121212") == true);


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


	assert(mxRedisClient.LSET("12122121", 1, "12212") == false);
	assert(mxRedisClient.LPUSH("12122121", "12212") == true);
	std::string lpop;
	assert(mxRedisClient.LPOP("12122121", lpop) == true);
	assert("12212" == lpop);

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

void NFRedisTester::TestSet()
{

}

void NFRedisTester::TestSort()
{

}

void NFRedisTester::TestString()
{
	std::string strKey11 = "TestString1112121";
	std::string strValu11e = "111";

	int64_t nValueDECR;
	assert(mxRedisClient.DECR(strKey11, nValueDECR) == false);
	assert(mxRedisClient.SET(strKey11, strValu11e) == true);
	assert(mxRedisClient.DECR(strKey11, nValueDECR) == true);
	assert(nValueDECR == 110);
	assert(mxRedisClient.DECRBY(strKey11, 10, nValueDECR) == true);
	assert(nValueDECR == 100);
	std::string oldGETSET;
	assert(mxRedisClient.GETSET(strKey11, "200", oldGETSET) == true);
	assert(oldGETSET == "100");

	assert(mxRedisClient.INCR(strKey11, nValueDECR) == true);
	assert(nValueDECR == 101);

	assert(mxRedisClient.INCRBY(strKey11, 100, nValueDECR) == true);
	assert(nValueDECR == 201);

	std::string strKey = "TestString";
	std::string strValue = "1232TestString234";

	assert(mxRedisClient.SET(strKey, strValue) == true);

	std::string strGET;
	assert(mxRedisClient.GET(strKey, strGET) == true);
	assert(strGET == strValue);

	int nAPPEND = 0;
	assert(mxRedisClient.APPEND(strKey, strValue, nAPPEND) == true);
	assert(nAPPEND == strValue.length() * 2);

	int nSTRLEN = 0;
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
