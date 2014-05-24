// -------------------------------------------------------------------------
//    @FileName         :    NFCComponentManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFCComponentManager
//
// -------------------------------------------------------------------------

#ifndef _NFC_COMPONENT_MANAGER_H_
#define _NFC_COMPONENT_MANAGER_H_

#include "NFCComponent.h"
#include "NFIComponentManager.h"
#include "NFIdentID.h"

class NFCComponentManager : public NFIComponentManager
{
public:
    NFCComponentManager(const NFIDENTID& self)
    {
        mSelf = self;
    }
    
    virtual ~NFCComponentManager()
    {
        DestroyAllComponent();
    }

    virtual NFIComponent* AddComponent(const std::string& strComponentName, const std::string& strScriptName);
    virtual NFIComponent* FindComponent(const std::string& strComponentName);
    virtual bool DestroyComponent(const std::string& strComponentName);
    virtual bool DestroyAllComponent();

    virtual bool SetEnable(const std::string& strComponentName, const bool bEnable);

    virtual bool Enable(const std::string& strComponentName);

    virtual NFIDENTID Self();

private:
    NFIDENTID mSelf;
};

#endif