// -------------------------------------------------------------------------
//    @FileName			:    NFChatPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFChatPlugin
//
// -------------------------------------------------------------------------


#include "NFGuildPlugin.h"
#include "NFCGuildModule.h"
#include "NFCGuildDataModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFGuildPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGuildPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGuildPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGuildPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCChatPlugin);
}

void NFGuildPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIGuildModule, NFCGuildModule)
	REGISTER_MODULE(pPluginManager, NFIGuildDataModule, NFCGuildDataModule)

}

void NFGuildPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIGuildDataModule, NFCGuildDataModule)
	UNREGISTER_MODULE(pPluginManager, NFIGuildModule, NFCGuildModule)
}