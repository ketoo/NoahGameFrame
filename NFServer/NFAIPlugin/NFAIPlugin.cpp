// NFAIPlugin.cpp : Defines the exported functions for the DLL application.
//

////#include "stdafx.h"
#include "NFAIPlugin.h"
#include "NFCAIModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFAIPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFAIPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFAIPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFAIPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFAIPlugin)
}

void NFAIPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCAIModule)
}

void NFAIPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCAIModule)
}