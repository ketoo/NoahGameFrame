// -------------------------------------------------------------------------
//    @FileName			:    NFUrlPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08 23:51
//    @Module           :    NFUrlPlugin
//
// -------------------------------------------------------------------------


#include "NFCUrlClientModule.h"
#include "NFUrlPlugin.h"
#include "NFCUrlCodeModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFUrlPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFUrlPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFUrlPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFUrlPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFUrlPlugin)
}

void NFUrlPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIUrlClientModule, NFCUrlClientModule)
    REGISTER_MODULE(pPluginManager, NFIUrlCodeModule, NFCUrlCodeModule)
}

void NFUrlPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIUrlCodeModule, NFCUrlCodeModule)
    UNREGISTER_MODULE(pPluginManager, NFIUrlClientModule, NFCUrlClientModule)
}