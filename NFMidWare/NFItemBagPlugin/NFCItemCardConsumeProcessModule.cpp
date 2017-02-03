// -------------------------------------------------------------------------
//    @FileName      :   NFCCardItemConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCCardItemConsumeProcessModule

// -------------------------------------------------------------------------

#include "NFCItemCardConsumeProcessModule.h"

bool NFCItemCardConsumeProcessModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();

	m_pItemModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_CARD, this);

    return true;
}

bool NFCItemCardConsumeProcessModule::AfterInit()
{


    return true;
}

bool NFCItemCardConsumeProcessModule::Shut()
{
    return true;
}

bool NFCItemCardConsumeProcessModule::Execute()
{
    return true;
}


int NFCItemCardConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID )
{
    return 1;
}

int NFCItemCardConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemID, const NFIDataList& targetID )
{

	m_pHeroModule->AddHero(self, strItemID);

    m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strItemID, "There is no element", __FUNCTION__, __LINE__);

    return 0;
}
