// -------------------------------------------------------------------------
//    @FileName			:    NFMailPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFMailPlugin
//
// -------------------------------------------------------------------------


#include "NFCMailModule.h"
#include "NFMailPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFMailPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMailPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFMailPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMailPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFMailPlugin);
}

void NFMailPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIMailModule, NFCMailModule)

}

void NFMailPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIMailModule, NFCMailModule)
}