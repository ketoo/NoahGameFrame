//------------------------------------------------------------------------ -
//    @FileName			:    NFNetPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-15
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFNetPlugin.h"
#include "NFCNetModule.h"
#include "NFCNetClientModule.h"
#include "NFCHttpServerModule.h"
#include "NFCWebsocketModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFNetPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFNetPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

const int NFNetPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFNetPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFNetPlugin);
}

void NFNetPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFINetModule, NFCNetModule)
    REGISTER_MODULE(pPluginManager, NFINetClientModule, NFCNetClientModule)
	REGISTER_MODULE(pPluginManager, NFIHttpServerModule, NFCHttpServerModule)
	REGISTER_MODULE(pPluginManager, NFIWebsocketModule, NFCWebsocketModule)
}

void NFNetPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIWebsocketModule, NFCWebsocketModule)
    UNREGISTER_MODULE(pPluginManager, NFINetClientModule, NFCNetClientModule)
    UNREGISTER_MODULE(pPluginManager, NFINetModule, NFCNetModule)
	UNREGISTER_MODULE(pPluginManager, NFIHttpServerModule, NFCHttpServerModule)
}