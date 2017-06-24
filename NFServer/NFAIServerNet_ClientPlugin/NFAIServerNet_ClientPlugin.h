// -------------------------------------------------------------------------
//    @FileName			:    NFAIServerNet_ClientPlugin.h
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFAIServerNet_ClientPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_AISERVER_NETCLIENTPLUGIN_MODULE_H
#define NF_AISERVER_NETCLIENTPLUGIN_MODULE_H
///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFGAMESERVERNET_CLIENTPLUGIN_EXPORTS
#define NFGAMESERVERNET_CLIENTPLUGIN_API __declspec(dllexport)
#else
#define NFGAMESERVERNET_CLIENTPLUGIN_API __declspec(dllimport)
#endif

class NFAIServerNet_ClientPlugin : public NFIPlugin
{
public:
	NFAIServerNet_ClientPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif