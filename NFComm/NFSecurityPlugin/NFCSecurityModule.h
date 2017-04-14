// -------------------------------------------------------------------------
//    @FileName			:    NFCSecurityModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-04-14 08:51
//    @Module           :    NFCSecurityModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_SECURITY_MODULE_H
#define NFC_SECURITY_MODULE_H

#include <memory>
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFISecurityModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"

class NFCSecurityModule
    : public NFISecurityModule
{
public:
	NFCSecurityModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

	//when a user login NF system, as a security module, you need to grant user an key.
	virtual const std::string& GetSecurityKey(const std::string& strAcount);

	//when the user want to connect to proxy, as a user need to provide the key that got in login system.
	virtual bool VirifySecurityKey(const std::string& strAcount, const std::string& strSucrityKey);

	//when the user want to post a message to proxy, the message-data need to be virified.
	virtual std::string EncodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const char* strMessageData, const int nLen);
	virtual std::string EncodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const std::string& strMessageData);
	virtual std::string DecodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const char* strMessageData, const int nLen);
	virtual std::string DecodeMsg(const std::string& strAcount, const std::string& strSucrityKey, const int nMessageID, const std::string& strMessageData);

protected:

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
    //////////////////////////////////////////////////////////////////////////
};
#endif
