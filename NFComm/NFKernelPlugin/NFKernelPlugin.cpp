/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFKernelPlugin.h"
#include "NFCKernelModule.h"
#include "NFCSceneAOIModule.h"
#include "NFCEventModule.h"
#include "NFCScheduleModule.h"
#include "NFCDataTailModule.h"

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
	REGISTER_MODULE(pPluginManager, NFIDataTailModule, NFCDataTailModule)

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
	UNREGISTER_MODULE(pPluginManager, NFIDataTailModule, NFCDataTailModule)
	UNREGISTER_MODULE(pPluginManager, NFIEventModule, NFCEventModule)
	UNREGISTER_MODULE(pPluginManager, NFIKernelModule, NFCKernelModule)
	UNREGISTER_MODULE(pPluginManager, NFISceneAOIModule, NFCSceneAOIModule)
	UNREGISTER_MODULE(pPluginManager, NFIScheduleModule, NFCScheduleModule)
}