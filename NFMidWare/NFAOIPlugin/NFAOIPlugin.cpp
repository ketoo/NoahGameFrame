/*
    Copyright 2009 - 2019 NoahGameFrame/NoahFrame
    
   This source file is part of NoahGameFrame/NoahFrame.
   NoahGameFrame/NoahFrame is open-source software and you can redistribute it and/or modify
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


#include "NFAOIModule.h"
#include "NFAOIPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFAOIPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFAOIPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFAOIPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFAOIPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFAOIPlugin);
}

void NFAOIPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIAOIModule, NFAOIModule)

}

void NFAOIPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIAOIModule, NFAOIModule)
}