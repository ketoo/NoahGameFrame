// -------------------------------------------------------------------------
//    @FileName			:    NFUserGiftPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFUserGiftPlugin
//
// -------------------------------------------------------------------------


#include "NFCUserGiftModule.h"
#include "NFUserGiftPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFUserGiftPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFUserGiftPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFUserGiftPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFUserGiftPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFUserGiftPlugin);
}

void NFUserGiftPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIUserGiftModule, NFCUserGiftModule)

}

void NFUserGiftPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIUserGiftModule, NFCUserGiftModule)
}