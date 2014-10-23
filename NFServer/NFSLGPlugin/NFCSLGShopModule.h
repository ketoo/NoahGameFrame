// -------------------------------------------------------------------------
//    @FileName      :    NFCSLGShopModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-10-05
//    @Module           :    NFCSLGShopModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_SLG_SHOP_MODULE_H_
#define _NFC_SLG_SHOP_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFISLGShopModule.h"
#include "NFComm/NFPluginModule/NFISLGBuildingModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"

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
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

	virtual bool OnReqBuyItem(const NFIDENTID& self, const NFIDataList& var);

private:
    NFIElementInfoModule* m_pElementInfoModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFISLGBuildingModule* m_pSLGBuildingModule;
    NFIPropertyModule* m_pPropertyModule;
};


#endif
