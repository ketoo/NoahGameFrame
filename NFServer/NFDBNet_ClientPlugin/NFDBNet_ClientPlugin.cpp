// NFDBNet_ClientPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "NFDBNet_ClientPlugin.h"
#include "NFCDBToWorldModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFDBNet_ClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFDBNet_ClientPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDBNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDBNet_ClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFDBNet_ClientPlugin);
}

void NFDBNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIWorldToMasterModule, NFCDBToWorldModule)
}

void NFDBNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIWorldToMasterModule, NFCDBToWorldModule)
}