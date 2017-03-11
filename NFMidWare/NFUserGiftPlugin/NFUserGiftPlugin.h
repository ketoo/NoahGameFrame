// -------------------------------------------------------------------------
//    @FileName			:    NFUserGiftPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFUserGiftPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_USERGIFT_PLUGIN_H
#define NF_USERGIFT_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFUserGiftPlugin : public NFIPlugin
{
public:
	NFUserGiftPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif