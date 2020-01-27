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

#include <iostream>
#include <sstream>
#include "NFRedisClient.h"
#include "NFRedisTester.h"
#include "NFRedisTester.h"
using namespace std;

#pragma comment( lib, "ws2_32" )

#ifdef _DEBUG
#pragma comment( lib, "hiredis_d.lib") 
#else
#pragma comment( lib, "hiredis.lib") 
#endif	

#define _IP_ "127.0.0.1"
#define _PORT_ 6379

int main()
{
	NFRedisTester xRedisTester(_IP_, _PORT_);

	while (1)
	{
		if (xRedisTester.IsConnect())
		{
			break;
		}

		xRedisTester.Execute();
	}

	cout << "test begin......" << endl;
	while (1)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (xRedisTester.RunTester())
			break;

		xRedisTester.Execute();
	}

	cout << "test end......" << endl;
	std::getchar();
	return 0;
}
