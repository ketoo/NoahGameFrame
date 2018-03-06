// -------------------------------------------------------------------------
//    @FileName			:    NFProxyServerNet_ClientPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFProxyServerNet_ClientPlugin
//
// -------------------------------------------------------------------------


#include "NFCProxyServerToWorldModule.h"
#include "NFProxyServerNet_ClientPlugin.h"
#include "NFCProxyServerToGameModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFProxyServerNet_ClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFProxyServerNet_ClientPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFProxyServerNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFProxyServerNet_ClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFProxyServerNet_ClientPlugin);
}

void NFProxyServerNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIProxyServerToWorldModule, NFCProxyServerToWorldModule)
    REGISTER_MODULE(pPluginManager, NFIProxyServerToGameModule, NFCProxyServerToGameModule)
}

void NFProxyServerNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIProxyServerToGameModule, NFCProxyServerToGameModule)
    UNREGISTER_MODULE(pPluginManager, NFIProxyServerToWorldModule, NFCProxyServerToWorldModule)
}