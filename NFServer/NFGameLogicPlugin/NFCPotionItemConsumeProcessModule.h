// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCPotionConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制--药剂消费
// -------------------------------------------------------------------------

#ifndef _NFC_POTIONITEMCONSUME_PROCESS_MODULE_H_
#define _NFC_POTIONITEMCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

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
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    //物品使用是否合法
    virtual int ConsumeLegal( const NFIDENTID& self, int nItemRowID,  const NFIValueList& other );

    //使用物品的消耗
    virtual int ConsumeSelf( const NFIDENTID& self, int nItemRowID );

    //合法,消耗,那么处理过程[消耗后,nItemRowID已经找不到了，因为可能被清空了]
    virtual int ConsumeProcess( const NFIDENTID& self, const std::string& strItemName, const NFIValueList& other );

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
    NFIPackModule* m_pPackModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILevelModule* m_pLevelModule;
    NFIPropertyModule* m_pPropertyModule;
};

#endif
