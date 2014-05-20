// NFDataBasePlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFDataNoSqlPlugin.h"
#include "NFCDataNoSqlModule.h"
#include "NFCPlatformDataModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFDataNoSqlPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFDataNoSqlPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDataNoSqlPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataNoSqlPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFDataNoSqlPlugin)
}

void NFDataNoSqlPlugin::Install()
{
    //因为有好几个进程都在用此插件，隐藏此插件不能设置Title
    //SetConsoleTitle( "NFDataBaseServer" );
	REGISTER_MODULE(pPluginManager, NFCDataNoSqlModule)
	REGISTER_MODULE(pPluginManager, NFCPlatformDataModule)
}

void NFDataNoSqlPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFCPlatformDataModule)
	UNREGISTER_MODULE(pPluginManager, NFCDataNoSqlModule)
}