/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#include "HelloWorld7.h"

bool NFHelloWorld7::Init()
{
    mxRedisClient.Connect("127.0.0.1", 6379, "NoahGameFrame");


    std::cout << "Hello, world, Init7" << std::endl;

    return true;
}

bool NFHelloWorld7::AfterInit()
{
	
    return true;
}

bool NFHelloWorld7::Execute()
{
    mxRedisClient.Execute();

	if (mxRedisClient.IsConnect())
	{
        if (!mxRedisClient.GetAuthKey().empty() && !mxRedisClient.AUTH(mxRedisClient.GetAuthKey()))
        {
            printf("password error!\n");
        }
        mxRedisClient.FLUSHALL();
        
        static bool tested = false;
        if (!tested)
        {
            tested = true;
   
            NFPerformance performance;
            mxRedisClient.SET("testkey", "123456");
            std::string data;
            mxRedisClient.GET("testkey", data);

            performance.CheckTimePoint();
            std::cout << "cost: " << performance.TimeScope() << " ==> " << data << std::endl;
        }
    }

    return true;
}

bool NFHelloWorld7::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFHelloWorld7::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}