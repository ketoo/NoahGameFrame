// -------------------------------------------------------------------------
//    @FileName      :    NFCLevelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelModule
//
// -------------------------------------------------------------------------

#include "NFCLevelModule.h"

bool NFCLevelModule::Init()
{
    return true;
}


bool NFCLevelModule::Shut()
{
    return true;
}

bool NFCLevelModule::Execute()
{
    //λ����
    return true;
}

bool NFCLevelModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>("NFCPropertyConfigModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pPropertyConfigModule);
    assert(NULL != m_pElementInfoModule);



    return true;
}

int NFCLevelModule::AddExp(const NFGUID& self, const int nExp)
{
    NFJobType eJobType = (NFJobType)m_pKernelModule->GetPropertyInt(self, NFrame::Player::Job());
    int nCurExp = m_pKernelModule->GetPropertyInt(self, NFrame::Player::EXP());
    int nLevel = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level());
    int nMaxExp = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());

    nCurExp += nExp;

    int nRemainExp = nCurExp - nMaxExp;
    while (nRemainExp >= 0)
    {
        //����
        nLevel++;
        //��ֹԽ��BUG
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Level(), nLevel);

        nCurExp = nRemainExp;

        nMaxExp = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());
        if (nMaxExp <= 0)
        {
            break;
        }

        nRemainExp -= nMaxExp;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::EXP(), nCurExp);

    return 0;
}
