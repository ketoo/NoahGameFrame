// -------------------------------------------------------------------------
//    @FileName			:    NFAOIPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFAOIPlugin
//
// -------------------------------------------------------------------------


#include "NFCAOIModule.h"
#include "NFAOIPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFAOIPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFAOIPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFAOIPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFAOIPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFAOIPlugin);
}

void NFAOIPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIAOIModule, NFCAOIModule)

}

void NFAOIPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIAOIModule, NFCAOIModule)
}