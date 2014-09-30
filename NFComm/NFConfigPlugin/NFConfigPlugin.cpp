// NFConfigPlugin.cpp : Defines the exported functions for the DLL application.
//

////#include "stdafx.h"
#include "NFConfigPlugin.h"
#include "NFCLogicClassModule.h"
#include "NFCElementInfoModule.h"

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
    GET_PLUGIN_NAME(NFConfigPlugin)
}

void NFConfigPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCLogicClassModule)
    REGISTER_MODULE(pPluginManager, NFCElementInfoModule)
}

void NFConfigPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCElementInfoModule)
    UNREGISTER_MODULE(pPluginManager, NFCLogicClassModule)
}
