// -------------------------------------------------------------------------
//    @FileName			:    NFCNoSqlDriverManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCNoSqlDriverManager
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCNoSqlDriverManager.h"

NFCNoSqlDriverManager::NFCNoSqlDriverManager()
{
}

NFCNoSqlDriverManager::~NFCNoSqlDriverManager()
{

}

bool NFCNoSqlDriverManager::Init()
{
	mLastCheckTime = 0;

	return true;
}

bool NFCNoSqlDriverManager::Shut()
{

	return true;
}

bool NFCNoSqlDriverManager::AfterInit()
{
	
	return true;
}

bool NFCNoSqlDriverManager::Execute()
{


	return true;
}
