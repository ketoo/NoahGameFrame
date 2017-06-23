// -------------------------------------------------------------------------
//    @FileName         :    NFNavigationPlugin.cpp
//    @Author           :    Bluesky
//    @Date             :    2016-06-22
//    @Module           :    NFNavigationPlugin
//
// -------------------------------------------------------------------------

#include "NFCNavigationModule.h"
#include "NFNavigationPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFNavigationPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFNavigationPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFNavigationPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFNavigationPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFNavigationPlugin);
}

void NFNavigationPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFINavigationModule, NFCNavigationModule)
}

void NFNavigationPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFINavigationModule, NFCNavigationModule)
}