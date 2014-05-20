// NFWorldNet_ClientPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFWorldNet_ClientPlugin.h"
#include "NFCWorldNet_ClientModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFWorldNet_ClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldNet_ClientPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFWorldNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFWorldNet_ClientPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFWorldNet_ClientPlugin)
}

void NFWorldNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCWorldNet_ClientModule)
}

void NFWorldNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCWorldNet_ClientModule)
}