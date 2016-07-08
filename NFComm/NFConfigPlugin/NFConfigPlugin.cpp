// NFConfigPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "NFConfigPlugin.h"
#include "NFCClassModule.h"
#include "NFCElementModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFConfigPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFConfigPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////
NFConfigPlugin::NFConfigPlugin(NFIPluginManager* p)
{
    pPluginManager = p;
}

const int NFConfigPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFConfigPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFConfigPlugin)
}

void NFConfigPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIClassModule, NFCClassModule)
    REGISTER_MODULE(pPluginManager, NFIElementModule, NFCElementModule)
}

void NFConfigPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIElementModule, NFCElementModule)
    UNREGISTER_MODULE(pPluginManager, NFIClassModule, NFCClassModule)
}
