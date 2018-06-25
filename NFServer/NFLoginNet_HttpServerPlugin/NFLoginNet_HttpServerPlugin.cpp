// -------------------------------------------------------------------------
//    @FileName			:    NFLoginNet_HttpServerPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-05-01
//    @Module           :    NFLoginNet_HttpServerPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFLoginNet_HttpServerPlugin.h"
#include "NFCLoginNet_HttpServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

	CREATE_PLUGIN(pm, NFLoginNet_HttpServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFLoginNet_HttpServerPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFLoginNet_HttpServerPlugin::GetPluginVersion()
{
	return 0;
}

const std::string NFLoginNet_HttpServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLoginNet_HttpServerPlugin);
}

void NFLoginNet_HttpServerPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFILoginNet_HttpServerModule, NFCLoginNet_HttpServerModule)
}

void NFLoginNet_HttpServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFILoginNet_HttpServerModule, NFCLoginNet_HttpServerModule)
}