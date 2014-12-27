// NFLogPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFLogPlugin.h"
#include "NFCLogModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLogPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLogPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFLogPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFLogPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFLogPlugin)
}

void NFLogPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCLogModule)
}

void NFLogPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCLogModule)
}