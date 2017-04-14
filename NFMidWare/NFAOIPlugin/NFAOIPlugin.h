// -------------------------------------------------------------------------
//    @FileName			:    NFAOIPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFAOIPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_AOI_PLUGIN_H
#define NF_AOI_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFAOIPlugin : public NFIPlugin
{
public:
	NFAOIPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif