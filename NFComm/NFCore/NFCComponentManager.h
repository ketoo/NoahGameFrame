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
        ClearAll();
    }

    virtual bool Init();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Shut();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual std::shared_ptr<NFIComponent> AddComponent(const std::string& strComponentName, const std::string& strLanguageName);
    virtual std::shared_ptr<NFIComponent> FindComponent(const std::string& strComponentName);

    virtual bool SetEnable(const std::string& strComponentName, const bool bEnable);

    virtual bool Enable(const std::string& strComponentName);

    virtual NFIDENTID Self();

protected:
    virtual bool DestroyAllComponent();

private:
    NFIDENTID mSelf;
};

#endif