#include "Tutorial4ActorPlugin.h"
#include "Tutorial4ActorModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTitle("Tutorial4Actor");
#endif
    CREATE_PLUGIN(pm, Tutorial4ActorPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial4ActorPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial4ActorPlugin::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial4ActorPlugin::GetPluginName()
{
    GET_PLUGIN_NAME(Tutorial4ActorPlugin)
}

void Tutorial4ActorPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, HelloWorld4ActorModule)

}

void Tutorial4ActorPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, HelloWorld4ActorModule)
}
