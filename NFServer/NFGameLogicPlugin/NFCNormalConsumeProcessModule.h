// -------------------------------------------------------------------------
//    @FileName      :   NFCHeroItemConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCHeroItemConsumeProcessModule
// -------------------------------------------------------------------------

#ifndef NFC_NORMALITEMCONSUME_PROCESS_MODULE_H
#define NFC_NORMALITEMCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"

class NFCNormalConsumeProcessModule
    : public NFIItemConsumeProcessModule
{

public:
    NFCNormalConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID);

	virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID);
    
    bool AwardItemProperty(const NFGUID& self, const std::string& strID);

private:
    NFIPropertyModule* m_pPropertyModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
    NFIPackModule* m_pPackModule;
    NFIElementModule* m_pElementModule;
};

#endif
