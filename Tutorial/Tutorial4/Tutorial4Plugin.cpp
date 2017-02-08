#include "Tutorial4Plugin.h"
#include "HelloWorld4Module.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, Tutorial4Plugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial4Plugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial4Plugin::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial4Plugin::GetPluginName()
{
	return GET_CLASS_NAME(Tutorial4Plugin);
}

void Tutorial4Plugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFIHelloWorld4Module, NFCHelloWorld4Module)

}

void Tutorial4Plugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIHelloWorld4Module, NFCHelloWorld4Module)
}
