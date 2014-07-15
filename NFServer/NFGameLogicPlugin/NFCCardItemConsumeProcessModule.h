// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCPotionConsumeProcessModule
//    @Desc             :   道具消费机制类--使用物品，获得技能或者道具
//                                  --技能书学习技能,礼包类获得物品,BUFF类得到BUFF,技能类使用技能
// -------------------------------------------------------------------------

#ifndef _NFC_CARDITEMCONSUME_PROCESS_MODULE_H_
#define _NFC_CARDITEMCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCCardItemConsumeProcessModule
    : public NFIItemConsumeProcessModule
{

public:
    NFCCardItemConsumeProcessModule( NFIPluginManager* p )
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
};

#endif
