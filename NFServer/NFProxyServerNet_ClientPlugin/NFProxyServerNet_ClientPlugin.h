// -------------------------------------------------------------------------
//    @FileName      :    NFProxyServerNet_ClientPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFProxyServerNet_ClientPlugin
//
// -------------------------------------------------------------------------

#ifndef _NF_PROXYSERVER_NETCLIENTPLUGIN_MODULE_H_
#define _NF_PROXYSERVER_NETCLIENTPLUGIN_MODULE_H_
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFPROXYSERVERNET_CLIENTPLUGIN_EXPORTS
#define NFPROXYSERVERNET_CLIENTPLUGIN_API __declspec(dllexport)
#else
#define NFPROXYSERVERNET_CLIENTPLUGIN_API __declspec(dllimport)
#endif

class NFProxyServerNet_ClientPlugin : public NFIPlugin
{
public:
    NFProxyServerNet_ClientPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif