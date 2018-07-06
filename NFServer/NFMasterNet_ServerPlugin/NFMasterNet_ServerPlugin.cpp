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
#include "NFMasterNet_ServerPlugin.h"
#include "NFCMasterNet_ServerModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFMasterNet_ServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMasterNet_ServerPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int NFMasterNet_ServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMasterNet_ServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFMasterNet_ServerPlugin);
}

void NFMasterNet_ServerPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIMasterNet_ServerModule, NFCMasterNet_ServerModule)
}

void NFMasterNet_ServerPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIMasterNet_ServerModule, NFCMasterNet_ServerModule)
}