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


#include "NFGameServerPlugin.h"
#include "NFCGameServerModule.h"
#include "NFCSceneProcessModule.h"
#include "NFCPropertyModule.h"
#include "NFCLevelModule.h"
#include "NFCPropertyConfigModule.h"
#include "NFCNPCRefreshModule.h"
#include "NFCSwitchGameServerModule.h"

class NFCSwitchGameServerModule;

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

    REGISTER_MODULE(pPluginManager, NFIGameServerModule, NFCGameServerModule)
    REGISTER_MODULE(pPluginManager, NFISceneProcessModule, NFCSceneProcessModule)
    REGISTER_MODULE(pPluginManager, NFISwitchGameServerModule, NFCSwitchGameServerModule);
    REGISTER_MODULE(pPluginManager, NFIPropertyModule, NFCPropertyModule)
    REGISTER_MODULE(pPluginManager, NFILevelModule, NFCLevelModule)
    REGISTER_MODULE(pPluginManager, NFIPropertyConfigModule, NFCPropertyConfigModule)
    REGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFCNPCRefreshModule)
}

void NFGameServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFCNPCRefreshModule)
	UNREGISTER_MODULE(pPluginManager, NFIPropertyConfigModule, NFCPropertyConfigModule)
    UNREGISTER_MODULE(pPluginManager, NFILevelModule, NFCLevelModule)
    UNREGISTER_MODULE(pPluginManager, NFIPropertyModule, NFCPropertyModule)
    UNREGISTER_MODULE(pPluginManager, NFISwitchGameServerModule, NFCSwitchGameServerModule);
    UNREGISTER_MODULE(pPluginManager, NFISceneProcessModule, NFCSceneProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFIGameServerModule, NFCGameServerModule)
}
