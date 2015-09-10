#include "Tutorial5Plugin.h"
#include "HelloWorld5Module.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTitle("Tutorial5");
#endif
    CREATE_PLUGIN(pm, Tutorial5Plugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial5Plugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial5Plugin::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial5Plugin::GetPluginName()
{
    GET_CLASS_NAME(Tutorial5Plugin)
}

void Tutorial5Plugin::Install()
{

    REGISTER_MODULE(pPluginManager, HelloWorld5Module)

}

void Tutorial5Plugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, HelloWorld5Module)
}
