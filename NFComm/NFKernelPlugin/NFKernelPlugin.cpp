// -------------------------------------------------------------------------
//    @FileName         :    NFKernelPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-03-15
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------

#include "NFKernelPlugin.h"
#include "NFCKernelModule.h"
#include "NFCSceneAOIModule.h"
#include "NFCEventModule.h"
#include "NFCScheduleModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFKernelPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFKernelPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFKernelPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFKernelPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFKernelPlugin);
}

void NFKernelPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOIModule)
	REGISTER_MODULE(pPluginManager, NFIKernelModule, NFCKernelModule)
	REGISTER_MODULE(pPluginManager, NFIEventModule, NFCEventModule)
	REGISTER_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleModule)

	/*
	REGISTER_TEST_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOITestModule)
	REGISTER_TEST_MODULE(pPluginManager, NFIKernelModule, NFCKernelTestModule)
	REGISTER_TEST_MODULE(pPluginManager, NFIEventModule, NFCEventTestModule)
	REGISTER_TEST_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleTestModule)
	*/
}

void NFKernelPlugin::Uninstall()
{
	/*
	UNREGISTER_TEST_MODULE(pPluginManager, NFIEventModule, NFCEventTestModule)
	UNREGISTER_TEST_MODULE(pPluginManager, NFIKernelModule, NFCKernelTestModule)
	UNREGISTER_TEST_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOITestModule)
	UNREGISTER_TEST_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleTestModule)
*/
	UNREGISTER_MODULE(pPluginManager, NFIEventModule, NFCEventModule)
	UNREGISTER_MODULE(pPluginManager, NFIKernelModule, NFCKernelModule)
	UNREGISTER_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOIModule)
	UNREGISTER_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleModule)
}