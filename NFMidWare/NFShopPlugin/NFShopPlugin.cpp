// -------------------------------------------------------------------------
//    @FileName			:    NFShopPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFShopPlugin
//
// -------------------------------------------------------------------------


#include "NFCShopModule.h"
#include "NFShopPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFShopPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFShopPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFShopPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFShopPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCShopPlugin);
}

void NFShopPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIShopModule, NFCShopModule)

}

void NFShopPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIShopModule, NFCShopModule)
}