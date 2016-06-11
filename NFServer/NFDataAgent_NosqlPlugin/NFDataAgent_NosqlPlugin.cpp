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
#include "NFCMailRedisModule.h"

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
	REGISTER_MODULE(pPluginManager, NFIRankRedisModule, NFCRankRedisModule)
	REGISTER_MODULE(pPluginManager, NFIPlayerRedisModule, NFCPlayerRedisModule)
	REGISTER_MODULE(pPluginManager, NFIGuildRedisModule, NFCGuildRedisModule)
    REGISTER_MODULE(pPluginManager, NFIBigMapRedisModule, NFCBigMapRedisModule)
	REGISTER_MODULE(pPluginManager, NFICommonRedisModule, NFCCommonRedisModule)
	REGISTER_MODULE(pPluginManager, NFIMailRedisModule, NFCMailRedisModule)
}

void NFDataAgent_NosqlPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIMailRedisModule, NFCMailRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFICommonRedisModule, NFCCommonRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIBigMapRedisModule, NFCBigMapRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIGuildRedisModule, NFCGuildRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIPlayerRedisModule, NFCPlayerRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIRankRedisModule, NFCRankRedisModule)
}
