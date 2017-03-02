// -------------------------------------------------------------------------
//    @FileName			:   NFCItemCardConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCItemCardConsumeProcessModule
// -------------------------------------------------------------------------

#ifndef NFC_ITEM_CARD_CONSUME_PROCESS_MODULE_H
#define NFC_ITEM_CARD_CONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"

class NFCItemCardConsumeProcessModule
    : public NFIItemConsumeProcessModule
{

public:
    NFCItemCardConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	
	virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID);

	virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFDataList& targetID);
protected:

	bool AwardItemProperty(const NFGUID& self, const NFGUID& xHeroID, const std::string& strID);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIPackModule* m_pPackModule;
	NFIElementModule* m_pElementModule;
	NFIItemModule* m_pItemModule;
	NFIHeroModule* m_pHeroModule;
};

#endif
