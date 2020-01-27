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

//#include "Dependencies/googletest-release-1.8.0/googletest/include/gtest/gtest.h"
#include "NFTestModule.h"

bool NFTestModule::Awake()
{
	int argc = 0;
	char* c = new char[1];
	//::testing::GTEST_FLAG(output) = "xml:hello.xml";
	//::testing::InitGoogleTest(&argc, &c);

	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->Awake();
	}

	return true;
}

bool NFTestModule::Init()
{
	//find all plugins and all modules, then check whether they have a tester
	//if any module have't a tester for it then  can not start the application
	//this is a rule for NF's world to keep high quality code under TDD

	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->Init();
	}

    return true;
}

bool NFTestModule::AfterInit()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->AfterInit();
	}

	return true;
}

bool NFTestModule::CheckConfig()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->CheckConfig();
	}

	return true;
}

bool NFTestModule::ReadyExecute()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->ReadyExecute();
	}

	return true;
}

bool NFTestModule::Execute()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->Execute();
	}

    return true;
}

bool NFTestModule::BeforeShut()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->BeforeShut();
	}

	return true;
}

bool NFTestModule::Shut()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->Shut();
	}

	return true;
}

bool NFTestModule::Finalize()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->Finalize();
	}

	return true;
}

bool NFTestModule::OnReloadPlugin()
{
	std::list<NFIModule*> xModules = pPluginManager->Modules();
	for (auto it : xModules)
	{
		NFIModule* pModule = it;
		NFIModule* pTestModule = pPluginManager->FindTestModule(pModule->strName);
		pTestModule->OnReloadPlugin();
	}

	return true;
}

int NFTestModule::Factorial(int n)
{
	if (1 == n)
	{
		return 1;
	}

	return n*Factorial(n-1);
}

// 测试用例
/*
TEST(FactorialTest, HandlesPositiveInput) {
    EXPECT_EQ(1, this->Factorial(1));
    EXPECT_EQ(2, this->Factorial(2));
    EXPECT_EQ(6, this->Factorial(3));
    EXPECT_EQ(40320, this->Factorial(8));
}
*/