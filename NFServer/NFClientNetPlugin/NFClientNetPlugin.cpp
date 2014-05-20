// NFClientNetPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFCClientLoginNetModule.h"
#include "NFCClientGameNetModule.h"
#include "NFClientNetPlugin.h"

NFIPlugin* pPlugin = NULL;
NFIPluginManager* pPluginManager = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPluginManager = pm;

    CREATE_PLUGIN(pm, NFClientNetPlugin, pPlugin)

};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, pPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFClientNetPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFClientNetPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFClientNetPlugin)
}

void NFClientNetPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, "NFCClientLoginNetModule", NFCClientLoginNetModule)
    REGISTER_MODULE(pPluginManager, "NFCClientGameNetModule", NFCClientGameNetModule)
}

void NFClientNetPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCClientGameNetModule", NFCClientGameNetModule)
    UNREGISTER_MODULE(pPluginManager, "NFCClientLoginNetModule", NFCClientLoginNetModule)
}