// -------------------------------------------------------------------------
//    @FileName			:    NFAIServerNet_ClientPlugin.cpp
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFAIServerNet_ClientPlugin
//
// -------------------------------------------------------------------------


#include "NFCAIServerToWorldModule.h"
#include "NFAIServerNet_ClientPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFAIServerNet_ClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFAIServerNet_ClientPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFAIServerNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFAIServerNet_ClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFAIServerNet_ClientPlugin);
}

void NFAIServerNet_ClientPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIAIServerToWorldModule, NFCAIServerToWorldModule)

}

void NFAIServerNet_ClientPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIAIServerToWorldModule, NFCAIServerToWorldModule)
}