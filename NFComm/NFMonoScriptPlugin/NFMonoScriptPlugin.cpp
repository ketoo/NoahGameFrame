// -------------------------------------------------------------------------
//    @FileName      :    NFMonoScriptPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08 23:51
//    @Module           :    NFMonoScriptPlugin
//
// -------------------------------------------------------------------------


#include "NFCMonoScriptModule.h"
#include "NFMonoScriptPlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFMonoScriptPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMonoScriptPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFMonoScriptPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMonoScriptPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFMonoScriptPlugin)
}

void NFMonoScriptPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCMonoScriptModule)

}

void NFMonoScriptPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCMonoScriptModule)
}