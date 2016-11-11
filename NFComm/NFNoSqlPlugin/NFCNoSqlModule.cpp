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
	if (!m_pNoSqlDriver)
	{
		m_pNoSqlDriver = new NFCNoSqlDriver();
		return m_pNoSqlDriver->Connect(strIP, 6379);
	}
	return false;
}

bool NFCNoSqlModule::ConnectSql(const std::string & strIP, const int nPort)
{
	if (!m_pNoSqlDriver)
	{
		m_pNoSqlDriver = new NFCNoSqlDriver();
		return m_pNoSqlDriver->Connect(strIP, nPort, "");
	}
	return false;
}

bool NFCNoSqlModule::ConnectSql(const std::string & strIP, const int nPort, const std::string & strPass)
{
	if (!m_pNoSqlDriver)
	{
		m_pNoSqlDriver = new NFCNoSqlDriver();
		return m_pNoSqlDriver->Connect(strIP, nPort, strPass);
	}
	return false;
}

NFINoSqlDriver * NFCNoSqlModule::GetDriver()
{
	return m_pNoSqlDriver;
}
