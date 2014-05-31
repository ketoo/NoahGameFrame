// -------------------------------------------------------------------------
//    @FileName      :    NFPythonScriptPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08 23:51
//    @Module           :    NFPythonScriptPlugin
//
// -------------------------------------------------------------------------


#include "NFCPythonScriptModule.h"
#include "NFPythonScriptPlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFPythonScriptPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFPythonScriptPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFPythonScriptPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFPythonScriptPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFPythonScriptPlugin)
}

void NFPythonScriptPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCPythonScriptModule)

}

void NFPythonScriptPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCPythonScriptModule)
}