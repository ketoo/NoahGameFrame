// -------------------------------------------------------------------------
//    @FileName      :    NFWorldGuildPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFWorldGuildPlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLD_GUILD_PLUGIN_MODULE_H
#define NFC_WORLD_GUILD_PLUGIN_MODULE_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class NFWorldGuildPlugin : public NFIPlugin
{
public:
    NFWorldGuildPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif