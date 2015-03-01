// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------


////#include "stdafx.h"
#include "NFGameServerPlugin.h"
#include "NFCGameServerModule.h"
#include "NFCSceneProcessModule.h"
#include "NFCPropertyModule.h"
#include "NFCLevelModule.h"
#include "NFCPropertyConfigModule.h"
#include "NFCDataProcessModule.h"
#include "NFCPVPModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin( NFIPluginManager* pm )
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTitle( "NFGameServer" );
#endif // NF_PLATFORM
    CREATE_PLUGIN( pm, NFGameServerPlugin )

};

NF_EXPORT void DllStopPlugin( NFIPluginManager* pm )
{
    DESTROY_PLUGIN( pm, NFGameServerPlugin )
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameServerPlugin::GetPluginName()
{
    GET_PLUGIN_NAME( NFGameServerPlugin )
}

void NFGameServerPlugin::Install()
{

    REGISTER_MODULE( pPluginManager, NFCGameServerModule )
    REGISTER_MODULE( pPluginManager, NFCSceneProcessModule )
    REGISTER_MODULE( pPluginManager, NFCDataProcessModule )

    REGISTER_MODULE( pPluginManager, NFCPropertyModule )
    REGISTER_MODULE( pPluginManager, NFCLevelModule )
    REGISTER_MODULE( pPluginManager, NFCPropertyConfigModule )

    REGISTER_MODULE( pPluginManager, NFCPVPModule )
}

void NFGameServerPlugin::Uninstall()
{
    UNREGISTER_MODULE( pPluginManager, NFCPVPModule )

    UNREGISTER_MODULE( pPluginManager, NFCPropertyConfigModule )
    UNREGISTER_MODULE( pPluginManager, NFCLevelModule )
    UNREGISTER_MODULE( pPluginManager, NFCPropertyModule )

    UNREGISTER_MODULE( pPluginManager, NFCDataProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCSceneProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCGameServerModule )
}
