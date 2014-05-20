// NFWorldNet_ServerPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFWorldNet_ServerPlugin.h"
#include "NFCWorldNet_ServerModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFWorldNet_ServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldNet_ServerPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFWorldNet_ServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFWorldNet_ServerPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFWorldNet_ServerPlugin)
}

void NFWorldNet_ServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCWorldNet_ServerModule)
}

void NFWorldNet_ServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCWorldNet_ServerModule)
}