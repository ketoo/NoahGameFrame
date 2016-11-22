// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCPotionConsumeProcessModule
// -------------------------------------------------------------------------

#ifndef NFC_POTIONITEMCONSUME_PROCESS_MODULE_H
#define NFC_POTIONITEMCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCPotionItemConsumeProcessModule
    : public NFIItemConsumeProcessModule
{

public:
    NFCPotionItemConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID);

	virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
    NFIPackModule* m_pPackModule;
    NFIElementModule* m_pElementModule;
    NFILevelModule* m_pLevelModule;
    NFIPropertyModule* m_pPropertyModule;
};

#endif
