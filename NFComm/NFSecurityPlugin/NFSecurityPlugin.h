// -------------------------------------------------------------------------
//    @FileName			:    NFSecurityPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-04-14 08:51
//    @Module           :    NFSecurityPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_SECURITY_PLUGIN_H
#define NF_SECURITY_PLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFSecurityPlugin : public NFIPlugin
{
public:
	NFSecurityPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif