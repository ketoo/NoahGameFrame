// -------------------------------------------------------------------------
//    @FileName         :    NFCComponentManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFCComponentManager
//
// -------------------------------------------------------------------------

#include "NFCComponentManager.h"

NFGUID NFCComponentManager::Self()
{
    return mSelf;
}

bool NFCComponentManager::AddComponent(const std::string& strComponentName, NF_SHARE_PTR<NFIComponent> pNewComponent)
{
    return AddElement(strComponentName, pNewComponent);
}

bool NFCComponentManager::Init()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (nullptr != pComponent)
    {
        pComponent->Init();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::AfterInit()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent)
    {
        pComponent->AfterInit();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::BeforeShut()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent)
    {
        pComponent->BeforeShut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Shut()
{
    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent)
    {
        pComponent->Shut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Execute()
{

    NF_SHARE_PTR<NFIComponent> pComponent = First();
    while (pComponent.get() && pComponent->Enable())
    {
        pComponent->Execute();

        pComponent = Next();
    }

    return true;
}
