// -------------------------------------------------------------------------
//    @FileName			:    NFSkillPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFSkillPlugin
//
// -------------------------------------------------------------------------

#include "NFSkillPlugin.h"
#include "NFCSkillModule.h"
#include "NFCSkillConsumeManagerModule.h"
#include "NFCSkillCooldownModule.h"
//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFSkillPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFSkillPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFSkillPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFSkillPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFSkillPlugin);
}

void NFSkillPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFISkillConsumeManagerModule, NFCSkillConsumeManagerModule)
	REGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)
	REGISTER_MODULE(pPluginManager, NFISkillCooldownModule, NFCSkillCooldownModule)

}

void NFSkillPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFISkillCooldownModule, NFCSkillCooldownModule)
	UNREGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)
	UNREGISTER_MODULE(pPluginManager, NFISkillConsumeManagerModule, NFCSkillConsumeManagerModule)
}