// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataNoSqlModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <algorithm>
#include "NFCNoSqlModule.h"

NFCNoSqlModule::NFCNoSqlModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

NFCNoSqlModule::~NFCNoSqlModule()
{

}

bool NFCNoSqlModule::Init()
{
    return true;
}

bool NFCNoSqlModule::Shut()
{
    delete m_pRedisDriver;
	m_pRedisDriver = NULL;

    return true;
}

bool NFCNoSqlModule::AfterInit()
{
    return true;
}

bool NFCNoSqlModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCNoSqlModule::ConnectSql(const std::string& strIP)
{
    m_pRedisDriver = new NFCNoSqlDriver();
	m_pRedisDriver->Connect(strIP, 3306, "");

    return true;
}