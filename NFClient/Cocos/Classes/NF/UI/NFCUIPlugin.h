// -------------------------------------------------------------------------
//    @FileName			:    NFCUIPlugin.h
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    NFCUIPlugin
//
// -------------------------------------------------------------------------

#ifndef UI_PLUGIN_H
#define UI_PLUGIN_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


class NFCUIPlugin : public NFIPlugin
{
public:
    NFCUIPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif