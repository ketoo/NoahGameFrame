// -------------------------------------------------------------------------
//    @FileName         :    NFCComponentManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFCComponentManager
//
// -------------------------------------------------------------------------

#include "NFCComponentManager.h"

NFIDENTID NFCComponentManager::Self()
{
    return mSelf;
}

NF_SHARE_PTR<NFIComponent> NFCComponentManager::AddComponent(const std::string& strComponentName, const std::string& strLanguageName)
{
    NF_SHARE_PTR<NFIComponent> pComponent = GetElement(strComponentName);
    if (pComponent.get())
    {
        return NULL;
    }

    pComponent = NF_SHARE_PTR<NFIComponent>(NF_NEW NFCComponent(mSelf, strComponentName, strLanguageName));
    AddElement(strComponentName, pComponent);

    return pComponent;
}

NF_SHARE_PTR<NFIComponent> NFCComponentManager::FindComponent(const std::string& strComponentName)
{
    return GetElement(strComponentName);
}

bool NFCComponentManager::SetEnable(const std::string& strComponentName, const bool bEnable)
{
    NF_SHARE_PTR<NFIComponent> pComponent = GetElement(strComponentName);
    if (pComponent.get())
    {
        return pComponent->SetEnable(bEnable);
    }

    return false;
}

bool NFCComponentManager::Enable(const std::string& strComponentName)
{
    NF_SHARE_PTR<NFIComponent> pComponent = GetElement(strComponentName);
    if (pComponent.get())
    {
        return pComponent->Enable();
    }

    return false;
}

bool NFCComponentManager::DestroyAllComponent()
{
    ClearAll();

    return true;
}

bool NFCComponentManager::Init()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->Init();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::AfterInit()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->AfterInit();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::BeforeShut()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->BeforeShut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Shut()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->Shut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Execute(const float fLasFrametime, const float fStartedTime)
{

    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent.get() && pComponent->Enable())
    {
        pComponent->Execute(fLasFrametime, fStartedTime);

        pComponent = Next();
    }

    return true;
}
