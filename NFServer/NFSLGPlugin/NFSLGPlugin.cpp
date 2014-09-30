#include "NFSLGPlugin.h"
#include "NFCSLGModule.h"

#ifdef NF_DYNAMIC_PLUGIN

extern "C"  __declspec( dllexport ) void DllStartPlugin( NFIPluginManager* pm )
{
	SetConsoleTitle( "NFSLG" );

    CREATE_PLUGIN( pm, NFSLGPlugin )

};

extern "C" __declspec( dllexport ) void DllStopPlugin( NFIPluginManager* pm )
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
}

void NFSLGPlugin::Uninstall()
{
    UNREGISTER_MODULE( pPluginManager, NFCSLGModule )
}
