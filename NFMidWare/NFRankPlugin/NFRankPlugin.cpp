// -------------------------------------------------------------------------
//    @FileName			:    NFChatPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFChatPlugin
//
// -------------------------------------------------------------------------


#include "NFCRankModule.h"
#include "NFRankPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFRankPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFRankPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFRankPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFRankPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFRankPlugin);
}

void NFRankPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIRankModule, NFCRankModule)

}

void NFRankPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIRankModule, NFCRankModule)
}