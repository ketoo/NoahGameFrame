// NFClientNetPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFCClientLogictModule.h"
#include "NFClientLogicPlugin.h"

NFIPlugin* pPlugin = NULL;
NFIPluginManager* pPluginManager = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPluginManager = pm;

    CREATE_PLUGIN(pm, NFClientLogicPlugin, pPlugin)

};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, pPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFClientLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFClientLogicPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFClientLogicPlugin)
}

void NFClientLogicPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, "NFCClientLogictModule", NFCClientLogictModule)

    SetConsoleTitle("NFClientLogic");
}

void NFClientLogicPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCClientLogictModule", NFCClientLogictModule)
}