//#include "stdafx.h"

#include "NFClientRenderPlugin.h"
#include "NFCClientRenderModule.h"

NFIPlugin* pPlugin = NULL;
NFIPluginManager* pPluginManager = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPluginManager = pm;

    CREATE_PLUGIN(pm, NFClientRenderPlugin, pPlugin)

};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, pPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFClientRenderPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFClientRenderPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFClientRenderPlugin)
}

void NFClientRenderPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, "NFCClientRenderModule", NFCClientRenderModule)
}

void NFClientRenderPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, "NFCClientRenderModule", NFCClientRenderModule)
}