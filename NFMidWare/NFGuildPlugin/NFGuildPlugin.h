// -------------------------------------------------------------------------
//    @FileName			:    NFGuildPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFGuildPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_CHAT_PLUGIN_H
#define NF_CHAT_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFGuildPlugin : public NFIPlugin
{
public:
	NFGuildPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif