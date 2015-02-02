#include "Tutorial6Plugin.h"
#include "HelloWorld6Module.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTitle("Tutorial6");
#endif
    CREATE_PLUGIN(pm, Tutorial6Plugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial6Plugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial6Plugin::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial6Plugin::GetPluginName()
{
    GET_PLUGIN_NAME(Tutorial6Plugin)
}

void Tutorial6Plugin::Install()
{

    REGISTER_MODULE(pPluginManager, HelloWorld6Module)

}

void Tutorial6Plugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, HelloWorld6Module)
}
