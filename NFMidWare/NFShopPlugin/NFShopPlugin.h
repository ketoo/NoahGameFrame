// -------------------------------------------------------------------------
//    @FileName			:    NFShopPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFShopPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_SHOP_PLUGIN_H
#define NF_SHOP_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFShopPlugin : public NFIPlugin
{
public:
	NFShopPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif