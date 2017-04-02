// -------------------------------------------------------------------------
//    @FileName			:    NFWorldLogicPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFWorldLogicPlugin
//
// -------------------------------------------------------------------------

#include "NFCWorldLogicModule.h"
#include "NFWorldLogicPlugin.h"

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFWorldLogicPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldLogicPlugin)
};

//////////////////////////////////////////////////////////////////////////

const int NFWorldLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFWorldLogicPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFWorldLogicPlugin);
}

void NFWorldLogicPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIWorldLogicModule, NFCWorldLogicModule)
}

void NFWorldLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIWorldLogicModule, NFCWorldLogicModule)
}
