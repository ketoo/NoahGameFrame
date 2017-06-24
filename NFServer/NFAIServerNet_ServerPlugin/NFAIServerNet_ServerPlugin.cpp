// -------------------------------------------------------------------------
//    @FileName			:    NFAIServerNet_ServerPlugin.cpp
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFAIServerNet_ServerPlugin
//
// -------------------------------------------------------------------------


#include "NFCAIServerNet_ServerModule.h"
#include "NFAIServerNet_ServerPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFAIServerNet_ServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFAIServerNet_ServerPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFAIServerNet_ServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFAIServerNet_ServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFAIServerNet_ServerPlugin);
}

void NFAIServerNet_ServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIAIServerNet_ServerModule, NFCAIServerNet_ServerModule)

}

void NFAIServerNet_ServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIAIServerNet_ServerModule, NFCAIServerNet_ServerModule)
}