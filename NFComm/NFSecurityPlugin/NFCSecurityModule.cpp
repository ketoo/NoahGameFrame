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
	return true;
}

bool NFCSecurityModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

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
