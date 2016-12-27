// -------------------------------------------------------------------------
//    @FileName			:    NFTaskPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-27 08:51
//    @Module           :    NFTaskPlugin
//
// -------------------------------------------------------------------------


#include "NFCTaskModule.h"
#include "NFTaskPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFTaskPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFTaskPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFTaskPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFTaskPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFTaskPlugin);
}

void NFTaskPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFITaskModule, NFCTaskModule)

}

void NFTaskPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFITaskModule, NFCTaskModule)
}