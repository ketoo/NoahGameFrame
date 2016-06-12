// -------------------------------------------------------------------------
//    @FileName      :   NFCHeroItemConsumeProcessModule.cpp
//    @Author           :   Chuanbo.Guo
//    @Date             :   2013-10-02
//    @Module           :   NFCHeroItemConsumeProcessModule
//    @Desc             :   道具消费机制类,详细的具体某类道具消费流程以及扣除机制
// -------------------------------------------------------------------------

#include "NFCNormalConsumeProcessModule.h"

bool NFCNormalConsumeProcessModule::Init()
{
    

    return true;
}

bool NFCNormalConsumeProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pItemConsumeManagerModule = pPluginManager->FindModule<NFIItemConsumeManagerModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();

    m_pItemConsumeManagerModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_NORMAL, this);
    m_pItemConsumeManagerModule->ResgisterConsumeModule(NFMsg::EItemType::EIT_CURRENCY, this);

    return true;
}

bool NFCNormalConsumeProcessModule::Shut()
{
    return true;
}

bool NFCNormalConsumeProcessModule::Execute()
{
    return true;
}


int NFCNormalConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
    return 1;
}

int NFCNormalConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFGUID& targetID )
{
    const std::string& strAwardProperty = m_pElementModule->GetPropertyString(strItemName, "AwardProperty");
    if (!strAwardProperty.empty())
    {
        AwardItemProperty(self, strAwardProperty);
    }

    m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, self, strItemName, "There is no element", __FUNCTION__, __LINE__);

    return 0;
}

bool NFCNormalConsumeProcessModule::AwardItemProperty(const NFGUID& self, const std::string& strID)
{
    if (strID.empty())
    {
        return false;
    }

    const int nVIPEXP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::VIPEXP());
    const int nEXP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::EXP());
    const int nHP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::HP());
    const int nSP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::SP());
    const int nMP = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::MP());
    const int nGold = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::Gold());
    const int nMoney = m_pElementModule->GetPropertyInt(strID, NFrame::ConsumeData::Money());

    if (nVIPEXP > 0)
    {
        const NFINT64 nOldValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::VIPEXP());
        const NFINT64 nNewValue = nOldValue + nVIPEXP;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::VIPEXP(), nNewValue);
    }

    if (nEXP > 0)
    {
        const NFINT64 nOldValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::EXP());
        const NFINT64 nNewValue = nOldValue + nEXP;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::EXP(), nNewValue);
    }

    if (nHP > 0)
    {
        m_pPropertyModule->AddHP(self, nHP);
    }
    if (nSP > 0)
    {
        m_pPropertyModule->AddSP(self, nHP);
    }

    if (nMP > 0)
    {
        m_pPropertyModule->AddMP(self, nHP);
    }

    if (nGold > 0)
    {
        m_pPropertyModule->AddDiamond(self, nHP);
    }

    if (nMoney > 0)
    {
        m_pPropertyModule->AddMoney(self, nHP);
    }

    return true;
}

