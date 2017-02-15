// -------------------------------------------------------------------------
//    @FileName			:    NFHeroPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFHeroPlugin
//
// -------------------------------------------------------------------------

#include "NFHeroPlugin.h"
#include "NFCHeroModule.h"
#include "NFCHeroPropertyModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFHeroPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFHeroPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFHeroPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFHeroPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFHeroPlugin);
}

void NFHeroPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIHeroModule, NFCHeroModule)
	REGISTER_MODULE(pPluginManager, NFIHeroPropertyModule, NFCHeroPropertyModule)
}

void NFHeroPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIHeroPropertyModule, NFCHeroPropertyModule)
	UNREGISTER_MODULE(pPluginManager, NFIHeroModule, NFCHeroModule)
}