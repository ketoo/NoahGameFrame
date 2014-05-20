// NFEventTriggerPlugin.cpp : Defines the exported functions for the DLL application.
//#include "stdafx.h"
#include "NFEventProcessPlugin.h"
#include "NFCEventProcessModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFEventProcessPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFEventProcessPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFEventProcessPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFEventProcessPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFEventProcessPlugin)
}

void NFEventProcessPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCEventProcessModule)
}

void NFEventProcessPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCEventProcessModule)
}
