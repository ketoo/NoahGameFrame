// -------------------------------------------------------------------------
//    @FileName			:    NFItemBagPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-03
//    @Module           :    NFItemBagPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_ITEM_BAG_PLUGIN_H
#define NF_ITEM_BAG_PLUGIN_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFItemBagPlugin : public NFIPlugin
{
public:
	NFItemBagPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif