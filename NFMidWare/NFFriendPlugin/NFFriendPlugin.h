// -------------------------------------------------------------------------
//    @FileName			:    NFFriendPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFFriendPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_FRIEND_PLUGIN_H
#define NF_FRIEND_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFFriendPlugin : public NFIPlugin
{
public:
	NFFriendPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif