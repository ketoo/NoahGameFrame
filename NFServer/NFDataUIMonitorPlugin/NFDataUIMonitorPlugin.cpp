// NFClientNetPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFCDataUIMonitorModule.h"
#include "NFDataUIMonitorPlugin.h"

NFIPlugin* pPlugin = NULL;
NFIPluginManager* pPluginManager = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPluginManager = pm;

    CREATE_PLUGIN(pm, NFDataUIMonitorPlugin, pPlugin)

};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, pPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFDataUIMonitorPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataUIMonitorPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFDataUIMonitorPlugin)
}

void NFDataUIMonitorPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, "NFCDataUIMonitorModule", NFCDataUIMonitorModule)

    SetConsoleTitle("NFDataUIMonitorPlugin");
}

void NFDataUIMonitorPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCDataUIMonitorModule", NFCDataUIMonitorModule)
}