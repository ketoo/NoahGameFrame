// -------------------------------------------------------------------------
//    @FileName			:    NFRankPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFRankPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_RANK_PLUGIN_H
#define NF_RANK_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFRankPlugin : public NFIPlugin
{
public:
	NFRankPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif