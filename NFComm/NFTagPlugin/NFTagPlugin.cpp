// -------------------------------------------------------------------------
//    @FileName      :    NFTagPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08 23:51
//    @Module           :    NFTagPlugin
//
// -------------------------------------------------------------------------


#include "NFCTagModule.h"
#include "NFTagPlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFTagPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFTagPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFTagPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFTagPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFTagPlugin)
}

void NFTagPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCTagModule)

}

void NFTagPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCTagModule)
}