// -------------------------------------------------------------------------
//    @FileName			:    NFHeroPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFHeroPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_HERO_PLUGIN_H
#define NF_HERO_PLUGIN_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFHeroPlugin : public NFIPlugin
{
public:
	NFHeroPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif