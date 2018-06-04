// -------------------------------------------------------------------------
//    @FileName			:    NFTestPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-27 08:51
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------


#include "NFCTestModule.h"
#include "NFTestPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFTestPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFTestPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFTestPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFTestPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFTestPlugin);
}

void NFTestPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFITestModule, NFCTestModule)

}

void NFTestPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFITestModule, NFCTestModule)
}