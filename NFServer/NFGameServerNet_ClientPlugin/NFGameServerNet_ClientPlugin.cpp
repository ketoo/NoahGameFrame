/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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



#include "NFCGameServerToWorldModule.h"
#include "NFCGameServerToDBModule.h"
#include "NFGameServerNet_ClientPlugin.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFGameServerNet_ClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGameServerNet_ClientPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameServerNet_ClientPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameServerNet_ClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameServerNet_ClientPlugin);
}

void NFGameServerNet_ClientPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIGameServerToWorldModule, NFCGameServerToWorldModule)
	REGISTER_MODULE(pPluginManager, NFIGameServerToDBModule, NFCGameServerToDBModule)

}

void NFGameServerNet_ClientPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIGameServerToDBModule, NFCGameServerToDBModule)
	UNREGISTER_MODULE(pPluginManager, NFIGameServerToWorldModule, NFCGameServerToWorldModule)
}