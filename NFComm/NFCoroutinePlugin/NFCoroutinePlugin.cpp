// -------------------------------------------------------------------------
//    @FileName         :    NFConfigPlugin.h
//    @Author           :    GaoYi
//    @Date             :    2017-01-08
//    @Module           :    NFGameConfigPlugin
//
// -------------------------------------------------------------------------

#include "NFCoroutinePlugin.h"
#include "NFCCoroutineModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFCoroutinePlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFCoroutinePlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////
NFCoroutinePlugin::NFCoroutinePlugin(NFIPluginManager* p)
{
	pPluginManager = p;
}

const int NFCoroutinePlugin::GetPluginVersion()
{
	return 0;
}

const std::string NFCoroutinePlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCoroutinePlugin);
}

void NFCoroutinePlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFICoroutineModule, NFCCoroutineModule)
}

void NFCoroutinePlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFICoroutineModule, NFCCoroutineModule)
}



