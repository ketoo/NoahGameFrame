// -------------------------------------------------------------------------
//    @FileName      :    NFLoginLogicPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    LoginLogicPluginModule
//
// -------------------------------------------------------------------------


//#include "stdafx.h"
#include "NFLoginLogicPlugin.h"
#include "NFCLoginLogicModule.h"

//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTitle("NFLoginServer");
#endif
    CREATE_PLUGIN(pm, NFLoginLogicPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLoginLogicPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFLoginLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFLoginLogicPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFLoginLogicPlugin)
}

void NFLoginLogicPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFCLoginLogicModule)

}

void NFLoginLogicPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCLoginLogicModule)
}
