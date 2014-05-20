// NFLoginNet_ServerPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFLoginNet_ServerPlugin.h"
#include "NFCLoginNet_ServerModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLoginNet_ServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLoginNet_ServerPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFLoginNet_ServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFLoginNet_ServerPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFLoginNet_ServerPlugin)
}

void NFLoginNet_ServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCLoginNet_ServerModule)
}

void NFLoginNet_ServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCLoginNet_ServerModule)
}