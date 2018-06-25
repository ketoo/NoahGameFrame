// -------------------------------------------------------------------------
//    @FileName				:    NFCTestModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-06-11
//    @Module               :    NFCTestModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCTestModule.h"

bool NFCTestModule::Awake()
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

bool NFCTestModule::Init()
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

bool NFCTestModule::AfterInit()
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

bool NFCTestModule::CheckConfig()
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

bool NFCTestModule::ReadyExecute()
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

bool NFCTestModule::Execute()
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

bool NFCTestModule::BeforeShut()
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

bool NFCTestModule::Shut()
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

bool NFCTestModule::Finalize()
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

bool NFCTestModule::OnReloadPlugin()
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

int NFCTestModule::Factorial(int n)
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