#include "Tutorial2.h"
#include "HelloWorld2.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, Tutorial2)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial2)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial2::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial2::GetPluginName()
{
    GET_PLUGIN_NAME(Tutorial2)
}

void Tutorial2::Install()
{
    REGISTER_MODULE(pPluginManager, HelloWorld2)
    
}

void Tutorial2::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, HelloWorld2)
}