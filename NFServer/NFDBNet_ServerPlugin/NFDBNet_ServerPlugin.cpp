// NFWorldNet_ServerPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "NFDBNet_ServerPlugin.h"
#include "NFCDBNet_ServerModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFDBNet_ServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFDBNet_ServerPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDBNet_ServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDBNet_ServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFDBNet_ServerPlugin);
}

void NFDBNet_ServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIDBNet_ServerModule, NFCDBNet_ServerModule)
}

void NFDBNet_ServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIDBNet_ServerModule, NFCDBNet_ServerModule)
}