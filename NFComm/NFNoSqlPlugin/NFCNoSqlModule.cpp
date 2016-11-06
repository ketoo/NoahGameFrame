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
	m_pNoSqlDriver = NULL;
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
    delete m_pNoSqlDriver;
	m_pNoSqlDriver = NULL;

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
    m_pNoSqlDriver = new NFCNoSqlDriver();
	m_pNoSqlDriver->Connect(strIP, 3306, "");

    return true;
}

bool NFCNoSqlModule::ConnectSql(const std::string & strIP, const int nPort, const std::string & strPass)
{
	m_pNoSqlDriver = new NFCNoSqlDriver();
	m_pNoSqlDriver->Connect(strIP, nPort, strPass);

	return true;
}
