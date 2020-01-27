/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: bluesky
   
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

#include "HelloWorld6.h"

bool NFHelloWorld6::Init()
{ 

    std::cout << "Hello, world, Init" << std::endl;

    return true;
}

bool NFHelloWorld6::AfterInit()
{
	m_pNavigationModule = pPluginManager->FindModule<NFINavigationModule>();

	int sceneId = 1;
	NFVector3 start(0, 0, 0);
	NFVector3 end(23, 0, 5);
	vector<NFVector3> pathResult;

	int ret = m_pNavigationModule->FindPath(sceneId, start, end, pathResult);
	if (ret > 0)
	{
		std::cout << "FindPath Success" << std::endl;
	}
	else
	{
		std::cout << "FindPath Fail" << std::endl;
	}
    return true;
}

bool NFHelloWorld6::Execute()
{
    return true;
}

bool NFHelloWorld6::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFHelloWorld6::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}