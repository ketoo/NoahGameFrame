// NFLoginNet_ClientPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "NFLoginNet_ClientPlugin.h"
#include "NFCLoginToMasterModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFLoginNet_ClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLoginNet_ClientPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFLoginNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFLoginNet_ClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLoginNet_ClientPlugin);
}

void NFLoginNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFILoginToMasterModule, NFCLoginToMasterModule)
}

void NFLoginNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFILoginToMasterModule, NFCLoginToMasterModule)
}