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


#include "NFItemBagPlugin.h"
#include "NFCItemModule.h"
#include "NFCPackModule.h"
#include "NFCEquipModule.h"
#include "NFCEquipPropertyModule.h"
#include "NFCItemConsumeManagerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFItemBagPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFItemBagPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFItemBagPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFItemBagPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFItemBagPlugin);
}

void NFItemBagPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIItemConsumeManagerModule, NFCItemConsumeManagerModule)
	REGISTER_MODULE(pPluginManager, NFIItemModule, NFCItemModule)
	REGISTER_MODULE(pPluginManager, NFIPackModule, NFCPackModule)
	REGISTER_MODULE(pPluginManager, NFIEquipModule, NFCEquipModule)
	REGISTER_MODULE(pPluginManager, NFIEquipPropertyModule, NFCEquipPropertyModule);
}

void NFItemBagPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIEquipPropertyModule, NFCEquipPropertyModule);
	UNREGISTER_MODULE(pPluginManager, NFIEquipModule, NFCEquipModule)
	UNREGISTER_MODULE(pPluginManager, NFIPackModule, NFCPackModule)
	UNREGISTER_MODULE(pPluginManager, NFIItemModule, NFCItemModule)
	UNREGISTER_MODULE(pPluginManager, NFIItemConsumeManagerModule, NFCItemConsumeManagerModule)
}