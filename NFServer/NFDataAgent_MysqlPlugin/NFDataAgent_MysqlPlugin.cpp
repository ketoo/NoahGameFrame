// -------------------------------------------------------------------------
//    @FileName			:    NFDataAgent_MysqlPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFDataAgent_MysqlPlugin
//
// -------------------------------------------------------------------------
#include "NFDataAgent_MysqlPlugin.h"
#include "NFCPlayerMysqlModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFDataAgent_MysqlPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFDataAgent_MysqlPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDataAgent_MysqlPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDataAgent_MysqlPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFDataBaseNet_ClientPlugin)
}

void NFDataAgent_MysqlPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCPlayerMysqlModule)
}

void NFDataAgent_MysqlPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCPlayerMysqlModule)
}
