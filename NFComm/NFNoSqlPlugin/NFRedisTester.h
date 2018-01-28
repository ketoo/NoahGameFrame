//
// Author: LUSHENG HUANG Created on 18/11/17.
//

#ifndef NFREDISPLUGIN_NFREDISTESTER_H
#define NFREDISPLUGIN_NFREDISTESTER_H

#include "NFRedisClient.h"



class NFRedisTester
{
public:
    NFRedisTester();

    void RunTester();
    void Execute();
private:

    void TestHash();
    void TestKey();
    void TestList();
    void TestSet();
    void TestSort();
    void TestString();

private:
    NFRedisClient mxRedisClient;
};


#endif //NFREDISPLUGIN_NFREDISTESTER_H
