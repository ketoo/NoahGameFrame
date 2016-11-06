// NFDataBasePlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFNoSqlPlugin.h"
#include "NFCNoSqlModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFNoSqlPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFNoSqlPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFNoSqlPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFNoSqlPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFNoSqlPlugin)
}

void NFNoSqlPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFINoSqlModule, NFCNoSqlModule)
}

void NFNoSqlPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFINoSqlModule, NFCNoSqlModule)
}