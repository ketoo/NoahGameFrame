// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------


////#include "stdafx.h"
//#include "NFGameServerPlugin.h"
#include "NFCGameServerModule.h"
#include "NFCSceneProcessModule.h"
#include "NFCPropertyModule.h"
#include "NFCLevelModule.h"
#include "NFCPropertyConfigModule.h"
#include "NFCDataProcessModule.h"

#ifdef NF_DYNAMIC_PLUGIN

extern "C"  __declspec( dllexport ) void DllStartPlugin( NFIPluginManager* pm )
{
	SetConsoleTitle( "NFGameServer" );

    CREATE_PLUGIN( pm, NFGameServerPlugin )

};

extern "C" __declspec( dllexport ) void DllStopPlugin( NFIPluginManager* pm )
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

}

void NFGameServerPlugin::Uninstall()
{
	
    UNREGISTER_MODULE( pPluginManager, NFCPropertyConfigModule )
    UNREGISTER_MODULE( pPluginManager, NFCLevelModule )
    UNREGISTER_MODULE( pPluginManager, NFCPropertyModule )

    UNREGISTER_MODULE( pPluginManager, NFCDataProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCSceneProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCGameServerModule )
}
