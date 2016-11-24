// -------------------------------------------------------------------------
//    @FileName         :    NFCComponentManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFCComponentManager
//
// -------------------------------------------------------------------------

#ifndef NFC_COMPONENT_MANAGER_H
#define NFC_COMPONENT_MANAGER_H

#include "NFIComponentManager.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFCComponentManager : public NFIComponentManager
{
private:
    NFCComponentManager()
    {

    }

public:
    NFCComponentManager(const NFGUID& self)
    {
        mSelf = self;
    }

    virtual ~NFCComponentManager()
    {
        ClearAll();
    }

    virtual bool Init();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Shut();

    virtual bool Execute();

    virtual NFGUID Self();

protected:

    virtual bool AddComponent(const std::string& strComponentName, NF_SHARE_PTR<NFIComponent> pNewComponent);

private:
    NFGUID mSelf;
};

#endif