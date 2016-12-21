#include "NFActorPlugin.h"
#include "NFCActorModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFActorPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFActorPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFActorPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFActorPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFActorPlugin);
}

void NFActorPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIActorModule, NFCActorModule)
}

void NFActorPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIActorModule, NFCActorModule)
}