// -------------------------------------------------------------------------
//    @FileName      :    NFJavaScriptPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08 23:51
//    @Module           :    NFJavaScriptPlugin
//
// -------------------------------------------------------------------------


#include "NFCJavaScriptModule.h"
#include "NFJavaScriptPlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFJavaScriptPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFJavaScriptPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFJavaScriptPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFJavaScriptPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFJavaScriptPlugin)
}

void NFJavaScriptPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCJavaScriptModule)

}

void NFJavaScriptPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCJavaScriptModule)
}