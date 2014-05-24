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

NFIComponent* NFCComponentManager::AddComponent( const std::string& strComponentName)
{
    NFIComponent* pComponent = GetElement(strComponentName);
    if (pComponent)
    {
        return NULL;
    }

    pComponent = new NFCComponent(mSelf, strComponentName, m_pPluginManager);
    AddElement(strComponentName, pComponent);

    return pComponent;
}

NFIComponent* NFCComponentManager::FindComponent( const std::string& strComponentName )
{
    return GetElement(strComponentName);
}

bool NFCComponentManager::SetEnable( const std::string& strComponentName, const bool bEnable )
{
    NFIComponent* pComponent = GetElement(strComponentName);
    if (pComponent)
    {
        return pComponent->SetEnable(bEnable);
    }

    return false;
}

bool NFCComponentManager::Enable( const std::string& strComponentName )
{
    NFIComponent* pComponent = GetElement(strComponentName);
    if (pComponent)
    {
        return pComponent->Enable();
    }

    return false;
}

bool NFCComponentManager::DestroyAllComponent()
{
    NFIComponent* pComponent = First();
    while (pComponent)
    {
        delete pComponent;
        pComponent = NULL;

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Init()
{
    NFIComponent* pComponent = First();
    while (pComponent)
    {
        pComponent->Init();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::AfterInit()
{
    NFIComponent* pComponent = First();
    while (pComponent)
    {
        pComponent->AfterInit();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::BeforeShut()
{
    NFIComponent* pComponent = First();
    while (pComponent)
    {
        pComponent->BeforeShut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Shut()
{
    NFIComponent* pComponent = First();
    while (pComponent)
    {
        pComponent->Shut();

        pComponent = Next();
    }

    return true;
}

bool NFCComponentManager::Execute( const float fLasFrametime, const float fStartedTime )
{
    NFIComponent* pComponent = First();
    while (pComponent)
    {
        if (pComponent->Enable())
        {
            pComponent->Execute(fLasFrametime, fStartedTime);
        }

        pComponent = Next();
    }

    return true;
}
