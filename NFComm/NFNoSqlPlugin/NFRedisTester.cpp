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

void NFRedisTester::TestHash() {


}

void NFRedisTester::TestKey()
{


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
    NFRedisResult* pRedisResult;

    std::string value = "1232321123r34234";
    pRedisResult = mxRedisClient.SET("test", value);
	std::cout << "test cmd: SET" << pRedisResult->GetCommand() << std::endl;
    std::cout << pRedisResult->GetRespString() << "  " << std::endl;

    pRedisResult = mxRedisClient.GET("test");
	std::cout << "test cmd:" << pRedisResult->GetCommand() << std::endl;
    std::cout << pRedisResult->GetRespString() << "  " << std::endl;

    assert( pRedisResult->GetRespString() == value);

    pRedisResult = mxRedisClient.GET("test1");
	std::cout << "test cmd:" << pRedisResult->GetCommand() << std::endl;
    std::cout << pRedisResult->GetRespString() << "  " << std::endl;

    pRedisResult = mxRedisClient.APPEND("test", value);
	std::cout << "test cmd:" << pRedisResult->GetCommand() << std::endl;
    std::cout << pRedisResult->GetRespInt() << "  " << std::endl;

    pRedisResult = mxRedisClient.APPEND("test", value);
	std::cout << "test cmd:" << pRedisResult->GetCommand() << std::endl;
    std::cout << pRedisResult->GetRespInt() << "  " << std::endl;


    std::cout << "test cmd:" << std::endl;

    std::vector<string_pair> vstring_pair;
    std::vector<std::string> vstringListKey;
    std::vector<std::string> vstringListValue;

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

    mxRedisClient.MGET(vstringListKey, vstringListValue);
	std::cout << "test cmd:" << pRedisResult->GetCommand() << std::endl;
}

void NFRedisTester::Execute()
{
    mxRedisClient.Execute();
}
