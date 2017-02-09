// -------------------------------------------------------------------------
//    @FileName			:    NFCDeadState.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCDeadState
//    @Desc             :
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
        //¼¸ÃëºóÉ¾³ýÊ¬Ìå
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