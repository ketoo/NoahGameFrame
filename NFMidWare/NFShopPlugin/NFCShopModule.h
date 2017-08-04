// -------------------------------------------------------------------------
//    @FileName			:    NFCShopModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCShopModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_SHOP_MODULE_H
#define NFC_SHOP_MODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIShopModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"

class NFCShopModule
    : public NFIShopModule
{
public:
	NFCShopModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

	virtual bool ReqBuyItem(const NFGUID& self, const std::string& strID);
	virtual bool ReqBuyItem(const NFGUID& self, const std::string& strID, const NFVector3& v);

protected:
	void OnClienBuyItem(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIPropertyModule* m_pPropertyModule;
	NFIPackModule* m_pPackModule;
    //////////////////////////////////////////////////////////////////////////
};
#endif
