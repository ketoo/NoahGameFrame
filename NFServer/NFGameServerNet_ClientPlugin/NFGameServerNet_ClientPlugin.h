// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerNet_ClientPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFGameServerNet_ClientPlugin
//
// -------------------------------------------------------------------------

#ifndef _NF_GAMESERVER_NETCLIENTPLUGIN_MODULE_H_
#define _NF_GAMESERVER_NETCLIENTPLUGIN_MODULE_H_
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFGAMESERVERNET_CLIENTPLUGIN_EXPORTS
#define NFGAMESERVERNET_CLIENTPLUGIN_API __declspec(dllexport)
#else
#define NFGAMESERVERNET_CLIENTPLUGIN_API __declspec(dllimport)
#endif

class NFGameServerNet_ClientPlugin : public NFIPlugin
{
public:
    NFGameServerNet_ClientPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif