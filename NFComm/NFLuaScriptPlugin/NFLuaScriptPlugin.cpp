// -------------------------------------------------------------------------
//    @FileName      :    NFLuaScriptPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08 23:51
//    @Module           :    NFLuaScriptPlugin
//
// -------------------------------------------------------------------------


#include "NFCLuaScriptModule.h"
#include "NFLuaScriptPlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLuaScriptPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLuaScriptPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFLuaScriptPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFLuaScriptPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFLuaScriptPlugin)
}

void NFLuaScriptPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCLuaScriptModule)

}

void NFLuaScriptPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCLuaScriptModule)
}