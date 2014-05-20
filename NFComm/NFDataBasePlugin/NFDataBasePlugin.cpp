// NFDataBasePlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFDataBasePlugin.h"
#include "NFCDataBaseModule.h"

NFIPlugin* pPlugin = NULL;
NFIPluginManager* pPluginManager = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPluginManager = pm;

    CREATE_PLUGIN(pm, NFDataBasePlugin, pPlugin)
};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, pPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFDataBasePlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataBasePlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFDataBasePlugin)
}

void NFDataBasePlugin::Install()
{
    //��Ϊ�кü������̶����ô˲�������ش˲����������Title
    //SetConsoleTitle( "NFDataBaseServer" );
    REGISTER_MODULE(pPluginManager, "NFCDataBaseModule", NFCDataBaseModule)
}

void NFDataBasePlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCDataBaseModule", NFCDataBaseModule)
}