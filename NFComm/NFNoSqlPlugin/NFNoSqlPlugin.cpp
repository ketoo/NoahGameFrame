//------------------------------------------------------------------------ -
//    @FileName			:    NFNoSqlPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFNoSqlPlugin
//
// -------------------------------------------------------------------------

#include "NFNoSqlPlugin.h"
#include "NFCNoSqlModule.h"
#include "NFCAsyNoSqlModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFNoSqlPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFNoSqlPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFNoSqlPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFNoSqlPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFNoSqlPlugin);
}

void NFNoSqlPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFINoSqlModule, NFCNoSqlModule)
	REGISTER_MODULE(pPluginManager, NFIAsyNoSqlModule, NFCAsyNoSqlModule)
}

void NFNoSqlPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIAsyNoSqlModule, NFCAsyNoSqlModule)
	UNREGISTER_MODULE(pPluginManager, NFINoSqlModule, NFCNoSqlModule)
}