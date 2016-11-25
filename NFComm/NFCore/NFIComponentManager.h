// -------------------------------------------------------------------------
//    @FileName         :    NFIComponentManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponentManager
//
// -------------------------------------------------------------------------

#ifndef NFI_COMPONENT_MANAGER_H
#define NFI_COMPONENT_MANAGER_H

#include <memory>
#include "NFMap.hpp"
#include "NFIComponent.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIModule.h"

class _NFExport NFIComponentManager : public NFIModule, public NFMapEx<std::string, NFIComponent>
{
public:
    virtual ~NFIComponentManager() {}

    template <typename T>
    bool AddComponent()
    {
        if (!TIsDerived<T, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return false;
        }

        NFIComponent* pComponent = NF_NEW T();
        return AddComponent(pComponent->GetComponentName(), NF_SHARE_PTR<NFIComponent>(pComponent));
    }

    template <typename T>
    NF_SHARE_PTR<T> FindComponent(const std::string& strName)
    {
        if (!TIsDerived<T, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return NF_SHARE_PTR<T>();
        }

        NF_SHARE_PTR<NFIComponent> pComponent = First();
        while (nullptr != pComponent)
        {
            if (pComponent->GetComponentName() == strName)
            {
                NF_SHARE_PTR<T> pT = std::dynamic_pointer_cast<T>(pComponent);
                if (nullptr != pT)
                {
                    return pT;
                }
                else
                {
                    return NF_SHARE_PTR<T>();
                }
            }

            pComponent = Next();
        }

        return NF_SHARE_PTR<T>();
    }

    virtual NFGUID Self() = 0;

    virtual bool AddComponent(const std::string& strComponentName, NF_SHARE_PTR<NFIComponent> pNewComponent) = 0;

private:
};

#endif
