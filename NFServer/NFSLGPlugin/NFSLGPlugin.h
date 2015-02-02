// -------------------------------------------------------------------------
//    @FileName         :    NFSLGPlugin.h
//    @Author           :    eliteYang
//    @Date             :    2014/9/9
//    @Module           :    NFSLGPlugin
//
// -------------------------------------------------------------------------

#ifndef __NFC_SLG_PLUGIN_H__
#define __NFC_SLG_PLUGIN_H__

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFSLGPlugin : public NFIPlugin
{
public:
    NFSLGPlugin( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif // !__NFC_SLG_PLUGIN_H__
