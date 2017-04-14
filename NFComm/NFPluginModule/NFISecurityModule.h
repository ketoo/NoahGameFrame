// -------------------------------------------------------------------------
//    @FileName			:    NFISecurityModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-04-14
//    @Module           :    NFISecurityModule
//    @Desc             :
// -------------------------------------------------------------------------


#ifndef NFI_SECURITY_MODULE_H
#define NFI_SECURITY_MODULE_H

#include "NFIModule.h"

class NFISecurityModule : public NFIModule
{
public:
	//when a user login NF system, as a security module, you need to grant user an key.
	virtual const std::string& GetSecurityKey(const std::string& strAcount) = 0;

	//when the user want to connect to proxy, as a user need to provide the key that got in login system.
	virtual bool VirifySecurityKey(const std::string& strAcount, const std::string& strSucrityKey) = 0;

	//when the user want to post a message to proxy, the message-data need to be virified.
	virtual std::string EncodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const char* strMessageData, const int nLen) = 0;
	virtual std::string EncodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const std::string& strMessageData) = 0;
	virtual std::string DecodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const char* strMessageData, const int nLen) = 0;
	virtual std::string DecodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const std::string& strMessageData) = 0;

};

#endif