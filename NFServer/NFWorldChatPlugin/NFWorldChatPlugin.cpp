// -------------------------------------------------------------------------
//    @FileName      :    NFWorldChatPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFWorldChatPlugin
//
// -------------------------------------------------------------------------


#include "NFCWorldChatModule.h"
#include "NFWorldChatPlugin.h"
#include "NFCWorldChatGroupModule.h"
#include "NFCChatGroupBroadcastModule.h"

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
#endif // NF_PLATFORM
    CREATE_PLUGIN(pm, NFWorldChatPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldChatPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFWorldChatPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFWorldChatPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFWorldChatPlugin)
}

void NFWorldChatPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFCWorldChatModule)
    REGISTER_MODULE(pPluginManager, NFCWorldChatGroupModule)
    REGISTER_MODULE(pPluginManager, NFCChatGroupBroadcastModule)


}

void NFWorldChatPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCChatGroupBroadcastModule)
    UNREGISTER_MODULE(pPluginManager, NFCWorldChatGroupModule)
    UNREGISTER_MODULE(pPluginManager, NFCWorldChatModule)
}
