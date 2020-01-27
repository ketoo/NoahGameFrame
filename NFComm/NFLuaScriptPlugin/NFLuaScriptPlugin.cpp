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


#include "NFLuaScriptModule.h"
#include "NFLuaWebSocketModule.h"
#include "NFLuaPBModule.h"
#include "NFLuaScriptPlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLuaScriptPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLuaScriptPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFLuaScriptPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFLuaScriptPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLuaScriptPlugin);
}

void NFLuaScriptPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFILuaScriptModule, NFLuaScriptModule)
	REGISTER_MODULE(pPluginManager, NFILuaPBModule, NFLuaPBModule)
	REGISTER_MODULE(pPluginManager, NFILuaWebSocketModule, NFLuaWebSocketModule)

}

void NFLuaScriptPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFILuaWebSocketModule, NFLuaWebSocketModule)
	UNREGISTER_MODULE(pPluginManager, NFILuaPBModule, NFLuaPBModule)
	UNREGISTER_MODULE(pPluginManager, NFILuaScriptModule, NFLuaScriptModule)
}