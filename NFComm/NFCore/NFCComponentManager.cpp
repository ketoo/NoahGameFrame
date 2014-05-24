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

NFIComponent* NFCComponentManager::AddComponent( const std::string& strComponentName, const std::string& strScriptName )
{
    NFIComponent* pComponent = GetElement(strComponentName);
    if (pComponent)
    {
        return NULL;
    }

    pComponent = new NFCComponent(mSelf, strComponentName, strScriptName);
    AddElement(strComponentName, pComponent);

    pComponent->Init();
    pComponent->AfterInit();

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

bool NFCComponentManager::DestroyComponent( const std::string& strComponentName )
{
    //和其他系统一样，不能在自己运行的过程中删除自己
    NFIComponent* pComponent = RemoveElement(strComponentName);
    if (pComponent)
    {
        pComponent->BeforeShut();
        pComponent->Shut();

        delete pComponent;

        return true;
    }

    return false;
}

bool NFCComponentManager::DestroyAllComponent()
{
    NFIComponent* pComponent = First();
    while (pComponent)
    {
        pComponent->BeforeShut();
        pComponent->Shut();

        delete pComponent;
        pComponent = NULL;

        pComponent = Next();
    }

    return true;
}
