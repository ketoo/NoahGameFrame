// NFLogPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFCrashPlugin.h"
#include "NFCCrashModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFCrashPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFCrashPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFCrashPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFCrashPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFCrashPlugin)
}

void NFCrashPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCCrashModule)
}

void NFCrashPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCCrashModule)
}