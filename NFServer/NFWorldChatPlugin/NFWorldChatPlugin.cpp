// -------------------------------------------------------------------------
//    @FileName      :    NFWorldChatPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFWorldChatPlugin
//
// -------------------------------------------------------------------------


//#include "stdafx.h"
#include "NFCWorldChatModule.h"
#include "NFWorldChatPlugin.h"

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
    GET_CLASS_NAME(NFWorldChatPlugin)
}

void NFWorldChatPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFCWorldChatModule)


}

void NFWorldChatPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCWorldChatModule)
}
