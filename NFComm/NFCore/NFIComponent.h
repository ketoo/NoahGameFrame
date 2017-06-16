// -------------------------------------------------------------------------
//    @FileName         :    NFIComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFIComponent
//
// -----------------------------------------------------------------------

#ifndef NFI_COMPONENT_H
#define NFI_COMPONENT_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIModule.h"

class _NFExport NFIComponent : public NFIModule
{
private:
    NFIComponent()
    {
    }

public:
    NFIComponent(NFGUID self, const std::string& strName)
    {
        mbHasInit = false;
        mbEnable = true;
        mSelf = self;
        mstrName = strName;
    }

    virtual ~NFIComponent() {}

    template <typename T>
    NF_SHARE_PTR<T> CreateNewInstance()
    {
        NF_SHARE_PTR<NFIComponent> pComponent = CreateNewInstance();
        if (nullptr != pComponent)
        {
            if (TIsDerived<T, NFIComponent>::Result)
            {
                NF_SHARE_PTR<T> pT = std::dynamic_pointer_cast<T>(pComponent);
                if (nullptr != pT)
                {
                    return pT;
                }
            }
        }

        return NF_SHARE_PTR<T>();
    }

    virtual bool SetEnable(const bool bEnable)
    {
        mbEnable = bEnable;
        return mbEnable;
    }

    virtual bool Enable()
    {
        return mbEnable;
    }

    virtual bool SetHasInit(const bool bEnable)
    {
        mbHasInit = bEnable;
        return mbHasInit;
    }

    virtual bool HasInit()
    {
        return mbHasInit;
    }

    virtual NFGUID Self()
    {
        return NULL_OBJECT;
    }

    virtual const std::string& GetComponentName() const
    {
        return mstrName;
    };

    //for actor
    virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg)
    {
        return 0;
    }

protected:
    virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance()
    {
        return nullptr;
    };

private:
    bool mbEnable;
    bool mbHasInit;
    NFGUID mSelf;
    std::string mstrName;
};

#endif
