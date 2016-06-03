// -------------------------------------------------------------------------
//    @FileName			:    NFDataAgent_NosqlPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFDataAgent_NosqlPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFDataAgent_NosqlPlugin.h"
#include "NFCRankRedisModule.h"
#include "NFCPlayerRedisModule.h"
#include "NFCGuildRedisModule.h"
#include "NFCBigMapRedisModule.h"
#include "NFCCommonRedisModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFDataAgent_NosqlPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFDataAgent_NosqlPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDataAgent_NosqlPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataAgent_NosqlPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFDataAgent_NosqlPlugin)
}

void NFDataAgent_NosqlPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFCRankRedisModule)
	REGISTER_MODULE(pPluginManager, NFCPlayerRedisModule)
	REGISTER_MODULE(pPluginManager, NFCGuildRedisModule)
    REGISTER_MODULE(pPluginManager, NFCBigMapRedisModule)
    REGISTER_MODULE(pPluginManager, NFCCommonRedisModule)
}

void NFDataAgent_NosqlPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCCommonRedisModule)
    UNREGISTER_MODULE(pPluginManager, NFCBigMapRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFCGuildRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFCPlayerRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFCRankRedisModule)
}
