// -------------------------------------------------------------------------
//    @FileName			:    NFAIServerNet_ServerPlugin.h
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFAIServerNet_ServerPlugin
//
// -------------------------------------------------------------------------

#ifndef NF_AISERVERNET_SERVERPLUGIN_H
#define NF_AISERVERNET_SERVERPLUGIN_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFGAMESERVERNET_SERVERPLUGIN_EXPORTS
#define NFGAMESERVERNET_SERVERPLUGIN_API __declspec(dllexport)
#else
#define NFGAMESERVERNET_SERVERPLUGIN_API __declspec(dllimport)
#endif

class NFAIServerNet_ServerPlugin : public NFIPlugin
{
public:
	NFAIServerNet_ServerPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif