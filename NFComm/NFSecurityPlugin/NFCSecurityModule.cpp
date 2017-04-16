// -------------------------------------------------------------------------
//    @FileName			:    NFCSecurityModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-04-14 08:51
//    @Module           :    NFCSecurityModule
//    @Desc             :

#include "NFCSecurityModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCSecurityModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	
	return true;
}

bool NFCSecurityModule::AfterInit()
{
	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Security::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];
			const std::string& strSecurityData = m_pElementModule->GetPropertyString(strId, NFrame::Security::SecurityData());

		}
	}

	return true;
}

const std::string & NFCSecurityModule::GetSecurityKey(const std::string & strAcount)
{
	return strAcount;
}

bool NFCSecurityModule::VirifySecurityKey(const std::string & strAcount, const std::string & strSucrityKey)
{
	if (strAcount == strSucrityKey)
	{
		return true;
	}

	return false;
}

std::string NFCSecurityModule::EncodeMsg(const std::string & strAcount, const std::string & strSucrityKey, const int nMessageID, const char * strMessageData, const int nLen)
{
	return std::string(strMessageData, nLen);
}

std::string NFCSecurityModule::EncodeMsg(const std::string & strAcount, const std::string & strSucrityKey, const int nMessageID, const std::string & strMessageData)
{
	return strMessageData;
}

std::string NFCSecurityModule::DecodeMsg(const std::string & strAcount, const std::string & strSucrityKey, const int nMessageID, const char * strMessageData, const int nLen)
{
	return std::string(strMessageData, nLen);
}

std::string NFCSecurityModule::DecodeMsg(const std::string & strAcount, const std::string & strSucrityKey, const int nMessageID, const std::string & strMessageData)
{
	return strMessageData;
}

bool NFCSecurityModule::Shut()
{

	return true;
}

bool NFCSecurityModule::Execute()
{
	return true;
}
