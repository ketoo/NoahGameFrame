// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerScriptPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08 23:51
//    @Module           :    NFGameServerScriptPlugin
//
// -------------------------------------------------------------------------


//#include "stdafx.h"
#include "NFCGameServerScriptModule.h"
#include "NFGameServerScriptPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFGameServerScriptPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGameServerScriptPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameServerScriptPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameServerScriptPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFGameServerScriptPlugin)
}

void NFGameServerScriptPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCGameServerScriptModule)

}

void NFGameServerScriptPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCGameServerScriptModule)
}