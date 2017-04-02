// -------------------------------------------------------------------------
//    @FileName			:    NFLogPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFLogPlugin
//    @Desc             :
// -------------------------------------------------------------------------


#include "NFLogPlugin.h"
#include "NFCLogModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLogPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLogPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFLogPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFLogPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLogPlugin);
}

void NFLogPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFILogModule, NFCLogModule)
}

void NFLogPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFILogModule, NFCLogModule)
}