// -------------------------------------------------------------------------
//    @FileName			:    NFCAsyNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCAsyNoSqlModule
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCAsyNoSqlModule.h"

NFCAsyNoSqlModule::NFCAsyNoSqlModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCAsyNoSqlModule::~NFCAsyNoSqlModule()
{

}

bool NFCAsyNoSqlModule::Init()
{

	return true;
}

bool NFCAsyNoSqlModule::Shut()
{

	return true;
}

bool NFCAsyNoSqlModule::AfterInit()
{

	return true;
}

bool NFCAsyNoSqlModule::Execute()
{
	

	return true;
}
