// -------------------------------------------------------------------------
//    �ļ���      ��    NFCDeadState.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-08 23:26
//    ��������    ��
//
// -------------------------------------------------------------------------

#include "../NFCAIModule.h"

bool NFCDeadState::Enter(const NFGUID& self)
{
    if (!NFIState::Enter(self))
    {
        m_pHateModule->ClearHate(self);
        m_pKernelModule->SetPropertyInt(self, "StateType", (int)NFObjectStateType::NOST_DEAD);
    }

    return true;
}

bool NFCDeadState::Execute(const NFGUID& self)
{
    if (!NFIState::Execute(self))
    {
        //�����ɾ��ʬ��
        m_pKernelModule->DestroyObject(self);
    }

    return true;
}

bool NFCDeadState::Exit(const NFGUID& self)
{


    return true;
}

bool NFCDeadState::DoRule(const NFGUID& self)
{
    return true;
}