// -------------------------------------------------------------------------
//    @FileName			:    NFAIPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFAIPlugin
//    @Desc             :
// -------------------------------------------------------------------------


#include "NFAIPlugin.h"
#include "NFCAIModule.h"
#include "NFCHateModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFAIPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFAIPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFAIPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFAIPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFAIPlugin);
}

void NFAIPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIAIModule, NFCAIModule)
	REGISTER_MODULE(pPluginManager, NFIHateModule, NFCHateModule)
}

void NFAIPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIHateModule, NFCHateModule)
	UNREGISTER_MODULE(pPluginManager, NFIAIModule, NFCAIModule)
}