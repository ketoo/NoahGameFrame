#include "NFDataBaseNet_ClientPlugin.h"
#include "NFCDataBaseNet_ClientModule.h"

#ifdef NF_DYNAMIC_PLUGIN

extern "C"  __declspec( dllexport ) void DllStartPlugin( NFIPluginManager* pm )
{
    CREATE_PLUGIN( pm, NFDataBaseNet_ClientPlugin )

};

extern "C" __declspec( dllexport ) void DllStopPlugin( NFIPluginManager* pm )
{
    DESTROY_PLUGIN( pm, NFDataBaseNet_ClientPlugin )
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDataBaseNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataBaseNet_ClientPlugin::GetPluginName()
{
    GET_PLUGIN_NAME( NFDataBaseNet_ClientPlugin )
}

void NFDataBaseNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCDataBaseNet_ClientModule)
}

void NFDataBaseNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCDataBaseNet_ClientModule)
}