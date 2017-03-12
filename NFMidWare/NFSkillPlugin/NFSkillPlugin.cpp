// -------------------------------------------------------------------------
//    @FileName			:    NFSkillPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFSkillPlugin
//
// -------------------------------------------------------------------------


#include "NFCSkillModule.h"
#include "NFSkillPlugin.h"

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
	return GET_CLASS_NAME(NFCChatPlugin);
}

void NFSkillPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)

}

void NFSkillPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)
}