/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NFREDISPLUGIN_NFREDISTESTER_H
#define NFREDISPLUGIN_NFREDISTESTER_H

#include "NFRedisClient.h"



class NFRedisTester
{
public:
    NFRedisTester(const std::string& ip, int port, const std::string& auth = "");

    bool RunTester();
    void Execute();
	bool IsConnect();

	bool Test_1();

    void TestHash();
    void TestKey();
    void TestList();
    void TestSet();
    void TestSort();
    void TestString();
	void TestPubSub();

    NFRedisClient mxRedisClient;
};


#endif //NFREDISPLUGIN_NFREDISTESTER_H
