// -------------------------------------------------------------------------
//    @FileName			:    NFCLevelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelModule
//
// -------------------------------------------------------------------------

#include "NFCLevelModule.h"

bool NFCLevelModule::Init()
{
	mbExpForHero = true;
    return true;
}


bool NFCLevelModule::Shut()
{
    return true;
}

bool NFCLevelModule::Execute()
{
    
    return true;
}

bool NFCLevelModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	
    return true;
}

int NFCLevelModule::AddExp(const NFGUID& self, const int64_t nExp)
{
	if (mbExpForHero)
	{
		m_pHeroModule->AddHeroExp(self, nExp);
		return 0;
	}

    int eJobType = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Job());
    int64_t nCurExp = m_pKernelModule->GetPropertyInt(self, NFrame::Player::EXP());
    int nLevel = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Level());
    int64_t nMaxExp = (int)m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());

    nCurExp += nExp;

    int64_t nRemainExp = nCurExp - nMaxExp;
    while (nRemainExp >= 0)
    {
        
        nLevel++;
        
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Level(), nLevel);

        nCurExp = nRemainExp;

        nMaxExp = (int)m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());
        if (nMaxExp <= 0)
        {
            break;
        }

        nRemainExp -= nMaxExp;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::EXP(), nCurExp);

    return 0;
}