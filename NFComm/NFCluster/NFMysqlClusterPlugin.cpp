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

const int NFMysqlClusterPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMysqlClusterPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFMysqlClusterPlugin)
}

void NFMysqlClusterPlugin::Install()
{
    //��Ϊ�кü������̶����ô˲�������ش˲����������Title
    //SetConsoleTitle( "NFDataBaseServer" );
    REGISTER_MODULE(pPluginManager, "NFCDataBaseModule", NFCDataBaseModule)
}

void NFMysqlClusterPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCDataBaseModule", NFCDataBaseModule)
}