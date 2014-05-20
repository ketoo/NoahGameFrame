// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerNet_ServerPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFGameServerNet_ServerPlugin
//
// -------------------------------------------------------------------------

#ifndef _NF_GAMESERVERNET_SERVERPLUGIN_H_
#define _NF_GAMESERVERNET_SERVERPLUGIN_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFGAMESERVERNET_SERVERPLUGIN_EXPORTS
#define NFGAMESERVERNET_SERVERPLUGIN_API __declspec(dllexport)
#else
#define NFGAMESERVERNET_SERVERPLUGIN_API __declspec(dllimport)
#endif

class NFGameServerNet_ServerPlugin : public NFIPlugin
{
public:
    NFGameServerNet_ServerPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif