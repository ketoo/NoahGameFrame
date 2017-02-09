// -------------------------------------------------------------------------
//    @FileName			:    NFGameServerPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------


#include "NFGameServerPlugin.h"
#include "NFCGameServerModule.h"
#include "NFCSceneProcessModule.h"
#include "NFCPropertyModule.h"
#include "NFCLevelModule.h"
#include "NFCPropertyConfigModule.h"
#include "NFCNPCRefreshModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFGameServerPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGameServerPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameServerPlugin);
}

void NFGameServerPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFIGameServerModule, NFCGameServerModule)
    REGISTER_MODULE(pPluginManager, NFISceneProcessModule, NFCSceneProcessModule)

    REGISTER_MODULE(pPluginManager, NFIPropertyModule, NFCPropertyModule)
    REGISTER_MODULE(pPluginManager, NFILevelModule, NFCLevelModule)
    REGISTER_MODULE(pPluginManager, NFIPropertyConfigModule, NFCPropertyConfigModule)
    REGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFCNPCRefreshModule)
}

void NFGameServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFCNPCRefreshModule)
	UNREGISTER_MODULE(pPluginManager, NFIPropertyConfigModule, NFCPropertyConfigModule)
    UNREGISTER_MODULE(pPluginManager, NFILevelModule, NFCLevelModule)
    UNREGISTER_MODULE(pPluginManager, NFIPropertyModule, NFCPropertyModule)

    UNREGISTER_MODULE(pPluginManager, NFISceneProcessModule, NFCSceneProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFIGameServerModule, NFCGameServerModule)
}
