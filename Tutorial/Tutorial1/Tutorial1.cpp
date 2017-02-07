#include "Tutorial1.h"
#include "HelloWorld1.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, Tutorial1)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial1)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial1::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial1::GetPluginName()
{
	return GET_CLASS_NAME(Tutorial1);
}

void Tutorial1::Install()
{
    REGISTER_MODULE(pPluginManager, NFIHelloWorld1, NFCHelloWorld1)
}

void Tutorial1::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIHelloWorld1, NFCHelloWorld1)
}