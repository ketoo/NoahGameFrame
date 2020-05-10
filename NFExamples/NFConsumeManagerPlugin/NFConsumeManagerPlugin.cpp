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


#include "NFConsumeManagerPlugin.h"
#include "NFCooldownModule.h"
#include "NFItemConsumeManagerModule.h"
#include "NFSkillConsumeManagerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFItemPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFItemPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFConsumeManagerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFConsumeManagerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFConsumeManagerPlugin);
}

void NFConsumeManagerPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFICooldownModule, NFCooldownModule)
	REGISTER_MODULE(pPluginManager, NFIItemConsumeManagerModule, NFItemConsumeManagerModule)
	REGISTER_MODULE(pPluginManager, NFISkillConsumeManagerModule, NFSkillConsumeManagerModule)
}

void NFConsumeManagerPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFICooldownModule, NFCooldownModule)
	UNREGISTER_MODULE(pPluginManager, NFISkillConsumeManagerModule, NFSkillConsumeManagerModule)
	UNREGISTER_MODULE(pPluginManager, NFIItemConsumeManagerModule, NFItemConsumeManagerModule)
}