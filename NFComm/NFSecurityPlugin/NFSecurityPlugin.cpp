// -------------------------------------------------------------------------
//    @FileName			:    NFSecurityPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-04-14 08:51
//    @Module           :    NFSecurityPlugin
//
// -------------------------------------------------------------------------

#include "NFCSecurityModule.h"
#include "NFSecurityPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFSecurityPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFSecurityPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFSecurityPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFSecurityPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCSecurityPlugin);
}

void NFSecurityPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFISecurityModule, NFCSecurityModule)

}

void NFSecurityPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFISecurityModule, NFCSecurityModule)
}