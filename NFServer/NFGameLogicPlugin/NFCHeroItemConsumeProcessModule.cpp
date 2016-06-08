// -------------------------------------------------------------------------
//    @FileName      :   NFCHeroItemConsumeProcessModule.cpp
//    @Author           :   Chuanbo.Guo
//    @Date             :   2013-10-02
//    @Module           :   NFCHeroItemConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------

#include "NFCHeroItemConsumeProcessModule.h"

bool NFCHeroItemConsumeProcessModule::Init()
{
    

    return true;
}

bool NFCHeroItemConsumeProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>("NFCItemConsumeManagerModule");
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>("NFCHeroModule");

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pItemConsumeManagerModule);
	assert(NULL != m_pPackModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pHeroModule);

    m_pItemConsumeManagerModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_HERO_STONE, this);

    return true;
}

bool NFCHeroItemConsumeProcessModule::Shut()
{
    return true;
}

bool NFCHeroItemConsumeProcessModule::Execute()
{
    return true;
}


int NFCHeroItemConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
    NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
    if (nullptr == pHero.get())
    {
        return  0;
    }

    NFCDataList varList;
    if (pHero->FindObject(NFrame::Player::PlayerHero::PlayerHero_GUID, targetID, varList)<=0)
    {
        return 0;
    }

    const int nItemType = m_pElementInfoModule->GetPropertyInt(strItemName, NFrame::Item::ItemType());
    const int nItemSubType = m_pElementInfoModule->GetPropertyInt(strItemName, NFrame::Item::ItemSubType());

    return 1;
}

int NFCHeroItemConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
    NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
    if (nullptr == pHero.get())
    {
        return  0;
    }

    NFCDataList varList;
    if (pHero->FindObject(NFrame::Player::PlayerHero::PlayerHero_GUID, targetID, varList) <= 0)
    {
        return 0;
    }

    const int nItemType = m_pElementInfoModule->GetPropertyInt(strItemName, NFrame::Item::ItemType());
    const int nItemSubType = m_pElementInfoModule->GetPropertyInt(strItemName, NFrame::Item::ItemSubType());

    
    const std::string& strAwardProperty = m_pElementInfoModule->GetPropertyString(strItemName, "AwardProperty");
    if (!strAwardProperty.empty())
    {
        AwardItemProperty(self, targetID, strAwardProperty);
    }

    m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strItemName, "There is no element", __FUNCTION__, __LINE__);

    return 1;
}


bool NFCHeroItemConsumeProcessModule::AwardItemProperty(const NFGUID& self, const NFGUID& xHeroID, const ::string& strID)
{
    NF_SHARE_PTR<NFIRecord> pHero = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
    if (nullptr == pHero.get())
    {
        return  0;
    }

    NFCDataList varList;
    if (pHero->FindObject(NFrame::Player::PlayerHero::PlayerHero_GUID, xHeroID, varList) <= 0)
    {
        return 0;
    }

    if (strID.empty())
    {
        return false;
    }

    const int nRow = varList.Int(0);

    const int nVIPEXP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::VIPEXP());
    const int nEXP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::EXP());
//     const int nHP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::HP());
//     const int nSP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::SP());
//     const int nMP = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::MP());
//     const int nGold = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::Gold());
//     const int nMoney = m_pElementInfoModule->GetPropertyInt(strID, NFrame::ConsumeData::Money());

//     if (nVIPEXP > 0)
//     {
//         const NFINT64 nOldValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::VIPEXP());
//         const NFINT64 nNewValue = nOldValue + nVIPEXP;
//         m_pKernelModule->SetPropertyInt(self, NFrame::Player::VIPEXP(), nNewValue);
//     }

    if (nEXP > 0)
    {
        m_pHeroModule->AddHeroExp(self, xHeroID, nEXP);
    }

//     if (nHP > 0)
//     {
//         m_pPropertyModule->AddHP(self, nHP);
//     }
//     if (nSP > 0)
//     {
//         m_pPropertyModule->AddSP(self, nHP);
//     }
// 
//     if (nMP > 0)
//     {
//         m_pPropertyModule->AddMP(self, nHP);
//     }
// 
//     if (nGold > 0)
//     {
//         m_pPropertyModule->AddDiamond(self, nHP);
//     }
// 
//     if (nMoney > 0)
//     {
//         m_pPropertyModule->AddMoney(self, nHP);
//     }

    return true;
}
