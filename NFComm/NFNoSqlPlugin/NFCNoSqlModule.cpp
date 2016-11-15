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
NFINoSqlDriver* NFCNoSqlModule::GetDriverBySuit(const std::string& strHash)
{
	return nullptr;
}

NFINoSqlDriver* NFCNoSqlModule::GetDriverBySuit(const int strHash)
{
	return nullptr;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
		pNoSqlDriver->Connect(strIP, 6379, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
		pNoSqlDriver->Connect(strIP, nPort, "");
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass)
{
	if (!mxNoSqlDriver.ExistElement(strID))
	{
		NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
		pNoSqlDriver->Connect(strIP, nPort, strPass);
		return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
	}

	return false;
}

NFINoSqlDriver* NFCNoSqlModule::GetDriver(const std::string& strID)
{
	return nullptr;
}

bool NFCNoSqlModule::RemoveConnectSql(const std::string& strID)
{
	return mxNoSqlDriver.RemoveElement(strID);
}