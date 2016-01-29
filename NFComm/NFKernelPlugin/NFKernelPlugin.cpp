// NFKernelPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "NFKernelPlugin.h"
#include "NFKernelModule/NFCKernelModule.h"
#include "NFSceneModule/NFCSceneModule.h"
#include "NFKernelModule/NFCUUIDModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFKernelPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFKernelPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFKernelPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFKernelPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFKernelPlugin)
}

void NFKernelPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCUUIDModule)
    REGISTER_MODULE(pPluginManager, NFCSceneModule)
    REGISTER_MODULE(pPluginManager, NFCKernelModule)
}

void NFKernelPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCKernelModule)
	UNREGISTER_MODULE(pPluginManager, NFCSceneModule)
    UNREGISTER_MODULE(pPluginManager, NFCUUIDModule)
}