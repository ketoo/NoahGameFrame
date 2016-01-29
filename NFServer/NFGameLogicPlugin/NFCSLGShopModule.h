// -------------------------------------------------------------------------
//    @FileName      :    NFCSLGShopModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-10-05
//    @Module           :    NFCSLGShopModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SLG_SHOP_MODULE_H
#define NFC_SLG_SHOP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFISLGShopModule.h"
#include "NFComm/NFPluginModule/NFISLGBuildingModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCSLGShopModule
    : public NFISLGShopModule
{
public:
    NFCSLGShopModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCSLGShopModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual bool ReqBuyItem(const NFGUID& self, const std::string& strID, float fX, float fY, float fZ);

protected:
	/////////SLG_START/////////////////////////////////////////////////////////////////
	void OnSLGClienBuyItem(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
    NFIElementInfoModule* m_pEslementInfoModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFISLGBuildingModule* m_pSLGBuildingModule;
    NFIPropertyModule* m_pPropertyModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
