// NFKernelPlugin.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "NFKernelPlugin.h"
#include "NFKernelModule/NFCKernelModule.h"
#include "NFKernelModule/NFCConcurrentModule.h"
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
    GET_PLUGIN_NAME(NFKernelPlugin)
}

void NFKernelPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCUUIDModule)
    //REGISTER_MODULE(pPluginManager, NFCConcurrentModule)
    REGISTER_MODULE(pPluginManager, NFCKernelModule)
}

void NFKernelPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCKernelModule)
    //UNREGISTER_MODULE(pPluginManager, NFCConcurrentModule)
    UNREGISTER_MODULE(pPluginManager, NFCUUIDModule)
}