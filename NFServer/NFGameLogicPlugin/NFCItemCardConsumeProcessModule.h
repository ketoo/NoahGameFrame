// -------------------------------------------------------------------------
//    @FileName      :   NFCItemCardConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCItemCardConsumeProcessModule
//    @Desc             :   道具消费机制类--使用物品，获得技能或者道具
//                                  --技能书学习技能,礼包类获得物品,BUFF类得到BUFF,技能类使用技能
// -------------------------------------------------------------------------

#ifndef NFC_ITEM_CARD_CONSUME_PROCESS_MODULE_H
#define NFC_ITEM_CARD_CONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIItemCardConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"

class NFCItemCardConsumeProcessModule
    : public NFIItemCardConsumeProcessModule
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

	//物品使用是否合法
	virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID);

	//合法,消耗,那么处理过程[消耗后,nItemRowID已经找不到了，因为可能被清空了]
	virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIPackModule* m_pPackModule;
	NFIElementModule* m_pElementModule;
	NFIItemModule* m_pItemModule;
	NFIHeroModule* m_pHeroModule;
};

#endif
