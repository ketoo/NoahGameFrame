// -------------------------------------------------------------------------
//    @FileName      :    NFMasterServerPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    CentreLogicModule
//
// -------------------------------------------------------------------------


//#include "stdafx.h"
#include "NFCMasterModule.h"
#include "NFMasterServerPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	SetConsoleTitle("NFMasterServer");

    CREATE_PLUGIN(pm, NFMasterServerPlugin)

	std::cout << "DllStartPlugin::thread id=" << GetCurrentThreadId() << std::endl;
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMasterServerPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFMasterServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMasterServerPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(NFMasterServerPlugin)
}

void NFMasterServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFCMasterModule)


}

void NFMasterServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFCMasterModule)
}