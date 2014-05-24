// -------------------------------------------------------------------------
//    @FileName         :    NFCComponent.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-02-17
//    @Module           :    NFCComponent
//
// -------------------------------------------------------------------------

#ifndef _NFC_COMPONENT_H_
#define _NFC_COMPONENT_H_

#include "NFIComponent.h"

class NFCComponent : public NFIComponent
{
public:
    NFCComponent(const NFIDENTID& self, const std::string& strComponentName, const std::string& strScriptName, NFIPluginManager* pLuginManager)
    {
        mSelf = self;
        mstrName = strComponentName;
        mstrScriptName = strScriptName;
        m_pPluginManager = pLuginManager;
    }

    virtual bool SetEnable(const bool bEnable);

    virtual bool Enable();

    virtual NFIDENTID Self();

    virtual const std::string& ComponentName();

    virtual const std::string& ScriptName();

private:

private:
    bool mbEnable;
    NFIDENTID mSelf;
    std::string mstrName;
    std::string mstrScriptName;
    NFIPluginManager* m_pPluginManager;
};

#endif
