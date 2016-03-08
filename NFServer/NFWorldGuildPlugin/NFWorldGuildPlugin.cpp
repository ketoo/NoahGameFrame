// -------------------------------------------------------------------------
//    @FileName      :    NFWorldGuildPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFWorldGuildPlugin
//
// -------------------------------------------------------------------------


#include "NFCWorldGuildModule.h"
#include "NFWorldGuildPlugin.h"
#include "NFCWorldGuildDataModule.h"
#include "NFCWorldGuildBroadcastModule.h"

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
#endif // NF_PLATFORM
    CREATE_PLUGIN(pm, NFWorldGuildPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldGuildPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFWorldGuildPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFWorldGuildPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFWorldGuildPlugin)
}

       void NFWorldGuildPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFCWorldGuildModule)
    REGISTER_MODULE(pPluginManager, NFCWorldGuildDataModule)
    REGISTER_MODULE(pPluginManager, NFCWorldGuildBroadcastModule)


}

void NFWorldGuildPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCWorldGuildBroadcastModule)
    UNREGISTER_MODULE(pPluginManager, NFCWorldGuildDataModule)
    UNREGISTER_MODULE(pPluginManager, NFCWorldGuildModule)
}
