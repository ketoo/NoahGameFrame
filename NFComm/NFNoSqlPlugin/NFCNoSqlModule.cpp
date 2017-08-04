// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataNoSqlModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCNoSqlModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFCNoSqlModule::NFCNoSqlModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCNoSqlModule::~NFCNoSqlModule()
{

}

bool NFCNoSqlModule::Init()
{
	mLastCheckTime = 0;

	return true;
}

bool NFCNoSqlModule::Shut()
{

	return true;
}

bool NFCNoSqlModule::AfterInit()
{
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::NoSqlServer::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::ServerID());
			const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::Port());
			const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::IP());
			const std::string& strAuth = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::Auth());

			if (this->AddConnectSql(strId, strIP, nPort, strAuth))
			{
				std::ostringstream strLog;
				strLog << "Connected NoSqlServer[" << strIP << "], Port = [" << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

			}
			else
			{
				std::ostringstream strLog;
				strLog << "Cannot connect NoSqlServer[" << strIP << "], Port = " << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
			}
		}
	}

	return true;
}

bool NFCNoSqlModule::Execute()
{
	if (mLastCheckTime + 10 > pPluginManager->GetNowTime())
	{
		return false;
	}
	mLastCheckTime = pPluginManager->GetNowTime();

	NF_SHARE_PTR<NFINoSqlDriver> xNosqlDriver = this->mxNoSqlDriver.First();
	while (xNosqlDriver)
	{
		if (!xNosqlDriver->Enable())
		{
			xNosqlDriver->ReConnect();
		}

		xNosqlDriver = this->mxNoSqlDriver.Next();
	}

	return true;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuitRandom()
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuitRandom();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuitConsistent()
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuitConsistent();
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuit(const std::string& strHash)
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuit(strHash);
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

/*
NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuit(const int nHash)
{
return mxNoSqlDriver.GetElementBySuit(nHash);
}
*/
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
		if (pNoSqlDriver->Connect(strIP, nPort, strPass))
		{
			mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

NFList<std::string> NFCNoSqlModule::GetDriverIdList()
{
	NFList<std::string> lDriverIdList;
	std::string strDriverId;
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = mxNoSqlDriver.First(strDriverId);
	while (pDriver)
	{
		lDriverIdList.Add(strDriverId);
		pDriver = mxNoSqlDriver.Next(strDriverId);
	}
	return lDriverIdList;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriver(const std::string& strID)
{
	NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElement(strID);
	if (xDriver && xDriver->Enable())
	{
		return xDriver;
	}

	return nullptr;
}

bool NFCNoSqlModule::RemoveConnectSql(const std::string& strID)
{
	return mxNoSqlDriver.RemoveElement(strID);
}