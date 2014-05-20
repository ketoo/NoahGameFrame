// -------------------------------------------------------------------------
//    文件名      ：    NFCDeadState.cpp
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-08 23:26
//    功能描述    ：
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "../NFCAIModule.h"

bool NFCDeadState::Enter(const NFIDENTID& self)
{
    if (!NFIState::Enter(self))
    {
        m_pHateModule->ClearHate(self);
        m_pKernelModule->SetPropertyInt(self, "StateType", (int)NFObjectStateType::NOST_DEAD);
    }

    return true;
}

bool NFCDeadState::Execute(const NFIDENTID& self)
{
    if (!NFIState::Execute(self))
    {
        //几秒后删除尸体
        m_pKernelModule->DestroyObject(self);
    }

    return true;
}

bool NFCDeadState::Exit(const NFIDENTID& self)
{


    return true;
}

bool NFCDeadState::DoRule(const NFIDENTID& self)
{
    return true;
}