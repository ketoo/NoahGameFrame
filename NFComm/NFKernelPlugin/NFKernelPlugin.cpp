/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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
#include "NFKernelModule.h"
#include "NFSceneModule.h"
#include "NFEventModule.h"
#include "NFScheduleModule.h"
#include "NFDataTailModule.h"
#include "NFCellModule.h"
#include "NFThreadPoolModule.h"
#include "NFELOModule.h"

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
    REGISTER_MODULE(pPluginManager, NFISceneModule, NFSceneModule)
	REGISTER_MODULE(pPluginManager, NFIKernelModule, NFKernelModule)
	REGISTER_MODULE(pPluginManager, NFIEventModule, NFEventModule)
	REGISTER_MODULE(pPluginManager, NFIScheduleModule, NFScheduleModule)
	REGISTER_MODULE(pPluginManager, NFIDataTailModule, NFDataTailModule)
	REGISTER_MODULE(pPluginManager, NFICellModule, NFCellModule)
	REGISTER_MODULE(pPluginManager, NFIThreadPoolModule, NFThreadPoolModule)
	REGISTER_MODULE(pPluginManager, NFIELOModule, NFELOModule)

	/*
	REGISTER_TEST_MODULE(pPluginManager, NFIKernelModule, NFKernelTestModule)
	REGISTER_TEST_MODULE(pPluginManager, NFIEventModule, NFEventTestModule)
	REGISTER_TEST_MODULE(pPluginManager, NFIScheduleModule, NFScheduleTestModule)
	*/
}

void NFKernelPlugin::Uninstall()
{
	/*
	UNREGISTER_TEST_MODULE(pPluginManager, NFIEventModule, NFEventTestModule)
	UNREGISTER_TEST_MODULE(pPluginManager, NFIKernelModule, NFKernelTestModule)
	UNREGISTER_TEST_MODULE(pPluginManager, NFIScheduleModule, NFScheduleTestModule)
*/
	UNREGISTER_MODULE(pPluginManager, NFIELOModule, NFELOModule)
	UNREGISTER_MODULE(pPluginManager, NFIThreadPoolModule, NFThreadPoolModule)
	UNREGISTER_MODULE(pPluginManager, NFICellModule, NFCellModule)
	UNREGISTER_MODULE(pPluginManager, NFIDataTailModule, NFDataTailModule)
	UNREGISTER_MODULE(pPluginManager, NFIEventModule, NFEventModule)
	UNREGISTER_MODULE(pPluginManager, NFIKernelModule, NFKernelModule)
	UNREGISTER_MODULE(pPluginManager, NFISceneModule, NFSceneModule)
	UNREGISTER_MODULE(pPluginManager, NFIScheduleModule, NFScheduleModule)

}