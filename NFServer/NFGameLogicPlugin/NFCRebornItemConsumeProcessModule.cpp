// -------------------------------------------------------------------------
//    @FileName      :   NFCPotionConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCPotionConsumeProcessModule
//    @Desc             :   �������ѻ�����,��ϸ�ľ���ĳ��������������Լ��۳�����
// -------------------------------------------------------------------------

#include "NFCRebornItemConsumeProcessModule.h"

bool NFCRebornItemConsumeProcessModule::Init()
{
    return true;
}

bool NFCRebornItemConsumeProcessModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>("NFCItemConsumeManagerModule");
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>("NFCLevelModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>( "NFCLogModule" );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pItemConsumeManagerModule );
    assert( NULL != m_pPackModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLevelModule );
    assert( NULL != m_pLogModule );

    //�����˸���ҩˮ(��Ѫ��)
    m_pItemConsumeManagerModule->ResgisterConsumeModule( NFMsg::EGIT_ITEM_REBORN, this );

    return true;
}

bool NFCRebornItemConsumeProcessModule::Shut()
{
    return true;
}

bool NFCRebornItemConsumeProcessModule::Execute()
{
    return true;
}

int NFCRebornItemConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
  
    return 0;
}

int NFCRebornItemConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
	return 0;
}
