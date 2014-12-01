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

std::shared_ptr<NFIComponent> NFCComponentManager::AddComponent(const std::string& strComponentName, const std::string& strLanguageName)
{
    std::shared_ptr<NFIComponent> pComponent = GetElement(strComponentName);
    if (pComponent.get())
    {
        return NULL;
    }

    pComponent = std::shared_ptr<NFIComponent>(NF_NEW NFCComponent(mSelf, strComponentName, strLanguageName));
    AddElement(strComponentName, pComponent);

    return pComponent;
}

std::shared_ptr<NFIComponent> NFCComponentManager::FindComponent(const std::string& strComponentName)
{
    return GetElement(strComponentName);
}

bool NFCComponentManager::SetEnable(const std::string& strComponentName, const bool bEnable)
{
    std::shared_ptr<NFIComponent> pComponent = GetElement(strComponentName);
    if (pComponent.get())
    {
        return pComponent->SetEnable(bEnable);
    }

    return false;
}

bool NFCComponentManager::Enable(const std::string& strComponentName)
{
    std::shared_ptr<NFIComponent> pComponent = GetElement(strComponentName);
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
    std::shared_ptr<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->Init();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::AfterInit()
{
    std::shared_ptr<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->AfterInit();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::BeforeShut()
{
    std::shared_ptr<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->BeforeShut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Shut()
{
    std::shared_ptr<NFIComponent> pComponent = First();
    while (pComponent.get())
    {
        pComponent->Shut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Execute(const float fLasFrametime, const float fStartedTime)
{

    std::shared_ptr<NFIComponent> pComponent = First();
    while (pComponent.get() && pComponent->Enable())
    {
        pComponent->Execute(fLasFrametime, fStartedTime);

        pComponent = Next();
    }

    return true;
}
