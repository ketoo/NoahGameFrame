// -------------------------------------------------------------------------
//    @FileName         :    NFCComponent.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFCComponent
//
// -------------------------------------------------------------------------

#include "NFCComponent.h"

NFIDENTID NFCComponent::Self()
{
    return mSelf;
}

bool NFCComponent::SetEnable(const bool bEnable)
{
    mbEnable = bEnable;
    return true;
}

bool NFCComponent::Enable()
{
    return mbEnable;
}

const std::string& NFCComponent::ComponentName()
{
    return mstrName;
}

const std::string& NFCComponent::LanguageName()
{
    return mstrLanguageName;
}
