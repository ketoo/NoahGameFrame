// -------------------------------------------------------------------------
//    @FileName			:    NFGameServerNet_ServerPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFGameServerNet_ServerPlugin
//
// -------------------------------------------------------------------------


#include "NFCProxyServerNet_ServerModule.h"
#include "NFProxyServerNet_ServerPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFProxyServerNet_ServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFProxyServerNet_ServerPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFProxyServerNet_ServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFProxyServerNet_ServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFProxyServerNet_ServerPlugin);
}

void NFProxyServerNet_ServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIProxyServerNet_ServerModule, NFCProxyServerNet_ServerModule)

}

void NFProxyServerNet_ServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIProxyServerNet_ServerModule, NFCProxyServerNet_ServerModule)
}