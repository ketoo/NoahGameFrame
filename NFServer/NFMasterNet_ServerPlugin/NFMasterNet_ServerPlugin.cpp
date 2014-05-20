// NFLoginNet_ServerPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFMasterNet_ServerPlugin.h"
#include "NFCMasterNet_ServerModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFMasterNet_ServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMasterNet_ServerPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFMasterNet_ServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMasterNet_ServerPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFMasterNet_ServerPlugin)
}

void NFMasterNet_ServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCMasterNet_ServerModule)
}

void NFMasterNet_ServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCMasterNet_ServerModule)
}