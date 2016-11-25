// -------------------------------------------------------------------------
//    @FileName			:    NFGameServerNet_ClientPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFGameServerNet_ClientPlugin
//
// -------------------------------------------------------------------------


#include "NFCGameServerToWorldModule.h"
#include "NFGameServerNet_ClientPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFGameServerNet_ClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGameServerNet_ClientPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameServerNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameServerNet_ClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameServerNet_ClientPlugin);
}

void NFGameServerNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIGameServerToWorldModule, NFCGameServerToWorldModule)

}

void NFGameServerNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIGameServerToWorldModule, NFCGameServerToWorldModule)
}