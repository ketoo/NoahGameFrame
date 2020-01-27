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


#include "NFGameServerPlugin.h"
#include "NFGameServerModule.h"
#include "NFSceneProcessModule.h"
#include "NFPropertyModule.h"
#include "NFPropertyConfigModule.h"
#include "NFNPCRefreshModule.h"
#include "NFSyncModule.h"
#include "NFCreateRoleModule.h"


#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFGameServerPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGameServerPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameServerPlugin);
}

void NFGameServerPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFIGameServerModule, NFGameServerModule)
    REGISTER_MODULE(pPluginManager, NFISceneProcessModule, NFSceneProcessModule)
    REGISTER_MODULE(pPluginManager, NFIPropertyModule, NFPropertyModule)
    REGISTER_MODULE(pPluginManager, NFIPropertyConfigModule, NFPropertyConfigModule)
	REGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFNPCRefreshModule)
	REGISTER_MODULE(pPluginManager, NFISyncModule, NFSyncModule)
	REGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCreateRoleModule)
}

void NFGameServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCreateRoleModule)
	UNREGISTER_MODULE(pPluginManager, NFISyncModule, NFSyncModule)
	UNREGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFNPCRefreshModule)
	UNREGISTER_MODULE(pPluginManager, NFIPropertyConfigModule, NFPropertyConfigModule)
    UNREGISTER_MODULE(pPluginManager, NFIPropertyModule, NFPropertyModule)
    UNREGISTER_MODULE(pPluginManager, NFISceneProcessModule, NFSceneProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFIGameServerModule, NFGameServerModule)
}
