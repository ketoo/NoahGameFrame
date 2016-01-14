#include "NFDataBaseNet_ClientPlugin.h"
#include "NFCDataBaseNet_ClientModule.h"
#include "NFCDataProcessModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin( NFIPluginManager* pm )
{
    CREATE_PLUGIN( pm, NFDataBaseNet_ClientPlugin )

};

NF_EXPORT void DllStopPlugin( NFIPluginManager* pm )
{
    DESTROY_PLUGIN( pm, NFDataBaseNet_ClientPlugin )
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDataBaseNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataBaseNet_ClientPlugin::GetPluginName()
{
    return GET_CLASS_NAME( NFDataBaseNet_ClientPlugin )
}

void NFDataBaseNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCDataBaseNet_ClientModule)
    REGISTER_MODULE(pPluginManager, NFCDataProcessModule)
}

void NFDataBaseNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCDataProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFCDataBaseNet_ClientModule)
}
