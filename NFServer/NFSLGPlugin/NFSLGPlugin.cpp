#include "NFSLGPlugin.h"
#include "NFCSLGModule.h"
#include "NFCSLGBuildingModule.h"
#include "NFCSLGShopModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin( NFIPluginManager* pm )
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTitle( "NFSLG" );
#endif
    CREATE_PLUGIN( pm, NFSLGPlugin )

};

NF_EXPORT void DllStopPlugin( NFIPluginManager* pm )
{
    DESTROY_PLUGIN( pm, NFSLGPlugin )
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFSLGPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFSLGPlugin::GetPluginName()
{
    GET_PLUGIN_NAME( NFSLGPlugin )
}

void NFSLGPlugin::Install()
{
	REGISTER_MODULE( pPluginManager, NFCSLGModule )
	REGISTER_MODULE( pPluginManager, NFCSLGBuildingModule )
	REGISTER_MODULE( pPluginManager, NFCSLGShopModule )
}

void NFSLGPlugin::Uninstall()
{
	UNREGISTER_MODULE( pPluginManager, NFCSLGShopModule )
	UNREGISTER_MODULE( pPluginManager, NFCSLGBuildingModule )
    UNREGISTER_MODULE( pPluginManager, NFCSLGModule )
}
