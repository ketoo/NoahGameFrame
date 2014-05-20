// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerNet_ServerPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :   NFGameServerNet_ServerPlugin
//
// -------------------------------------------------------------------------

#ifndef _NF_GAMESERVER_SCRIPT_LUGIN_H_
#define _NF_GAMESERVER_SCRIPT_LUGIN_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFGAMESERVER_SCRIPT_PLUGIN_EXPORTS
#define NFGAMESERVER_SCRIPT_PLUGIN_API __declspec(dllexport)
#else
#define NFGAMESERVER_SCRIPT_PLUGIN_API __declspec(dllimport)
#endif

class NFGameServerScriptPlugin : public NFIPlugin
{
public:
    NFGameServerScriptPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif