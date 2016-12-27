// -------------------------------------------------------------------------
//    @FileName			:    NFChatPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFChatPlugin
//
// -------------------------------------------------------------------------


#include "NFCChatModule.h"
#include "NFChatPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFChatPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFChatPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFChatPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFChatPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFChatPlugin);
}

void NFChatPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIChatModule, NFCChatModule)

}

void NFChatPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIChatModule, NFCChatModule)
}