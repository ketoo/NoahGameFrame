// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyPlugin
//
// -------------------------------------------------------------------------


#include "NFCProxyLogicPlugin.h"
#include "NFCProxyLogicModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFProxyLogicPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFProxyLogicPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFProxyLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFProxyLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFProxyLogicPlugin);
}

void NFProxyLogicPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFIProxyLogicModule, NFCProxyLogicModule)

}

void NFProxyLogicPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIProxyLogicModule, NFCProxyLogicModule)
}
