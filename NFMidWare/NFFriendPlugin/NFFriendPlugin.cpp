// -------------------------------------------------------------------------
//    @FileName			:    NFFriendPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFFriendPlugin
//
// -------------------------------------------------------------------------

#include "NFCFriendModule.h"
#include "NFFriendPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFFriendPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFFriendPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFFriendPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFFriendPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCFriendPlugin);
}

void NFFriendPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIFriendModule, NFCFriendModule)

}

void NFFriendPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIFriendModule, NFCFriendModule)
}