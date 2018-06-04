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
	testing::InitGoogleTest(&argc, &c);

	return true;
}

bool NFCTestModule::Init()
{
	//find all plugins and all modules, then check whether they have a tester for each module
	//if any module have't a tester for it then  can not start the application
	//this is a rule for NF's world to keep high quality code under TDD

    return true;
}

bool NFCTestModule::Shut()
{
    return true;
}

bool NFCTestModule::Execute()
{
    return true;
}

bool NFCTestModule::AfterInit()
{


    return true;
}