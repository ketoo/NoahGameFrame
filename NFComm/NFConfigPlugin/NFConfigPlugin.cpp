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


#include "NFConfigPlugin.h"
#include "NFClassModule.h"
#include "NFElementModule.h"
#include "NFRecordWrapperModule.h"
#include "NFCommonConfigModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFConfigPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFConfigPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////
NFConfigPlugin::NFConfigPlugin(NFIPluginManager* p)
{
    pPluginManager = p;
}

const int NFConfigPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFConfigPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFConfigPlugin);
}

void NFConfigPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIClassModule, NFClassModule)
    REGISTER_MODULE(pPluginManager, NFIElementModule, NFElementModule)
	REGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCommonConfigModule);
	REGISTER_MODULE(pPluginManager, NFIRecordWrapper,NFRecordReadWrapperModule);

}

void NFConfigPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIElementModule, NFElementModule)
    UNREGISTER_MODULE(pPluginManager, NFIClassModule, NFClassModule)
	UNREGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCommonConfigModule);
    UNREGISTER_MODULE(pPluginManager, NFIRecordWrapper,NFRecordReadWrapperModule);
}
