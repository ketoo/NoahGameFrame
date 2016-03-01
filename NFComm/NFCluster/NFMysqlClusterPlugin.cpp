#include "NFMysqlClusterPlugin.h"
#include "NFCMysqlClusterModule.h"
#include "NFCAsyMysqlClusterModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFMysqlClusterPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMysqlClusterPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

const int NFMysqlClusterPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMysqlClusterPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFMysqlClusterPlugin)
}

       void NFMysqlClusterPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCMysqlClusterModule)
    REGISTER_MODULE(pPluginManager, NFCAsyMysqlClusterModule)
}

void NFMysqlClusterPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCAsyMysqlClusterModule)
    UNREGISTER_MODULE(pPluginManager, NFCMysqlClusterModule)
}