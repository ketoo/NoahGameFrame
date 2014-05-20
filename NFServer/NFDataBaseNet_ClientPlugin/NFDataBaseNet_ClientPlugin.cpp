#include "NFDataBaseNet_ClientPlugin.h"
#include "NFCDataBaseNet_ClientModule.h"


NFIPlugin* pPlugin = NULL;
NFIPluginManager* pPluginManager = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPluginManager = pm;

    CREATE_PLUGIN(pm, NFDataBaseNet_ClientPlugin, pPlugin)

};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, pPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFDataBaseNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataBaseNet_ClientPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFDataBaseNet_ClientPlugin)
}

void NFDataBaseNet_ClientPlugin::Install()
{
    SetConsoleTitle("NFDataBaseServer");

    REGISTER_MODULE(pPluginManager, "NFCDataBaseNet_ClientModule", NFCDataBaseNet_ClientModule)
}

void NFDataBaseNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCDataBaseNet_ClientModule", NFCDataBaseNet_ClientModule)
}