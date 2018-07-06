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


#include "NFDBLogicPlugin.h"
#include "NFCRankRedisModule.h"
#include "NFCPlayerRedisModule.h"
#include "NFCGuildRedisModule.h"
#include "NFCBigMapRedisModule.h"
#include "NFCCommonRedisModule.h"
#include "NFCMailRedisModule.h"
#include "NFCAccountRedisModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFDBLogicPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFDBLogicPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFDBLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFDBLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFDBLogicPlugin);
}

void NFDBLogicPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIRankRedisModule, NFCRankRedisModule)
	REGISTER_MODULE(pPluginManager, NFIPlayerRedisModule, NFCPlayerRedisModule)
	REGISTER_MODULE(pPluginManager, NFIGuildRedisModule, NFCGuildRedisModule)
    REGISTER_MODULE(pPluginManager, NFIBigMapRedisModule, NFCBigMapRedisModule)
	REGISTER_MODULE(pPluginManager, NFICommonRedisModule, NFCCommonRedisModule)
	REGISTER_MODULE(pPluginManager, NFIMailRedisModule, NFCMailRedisModule)
	REGISTER_MODULE(pPluginManager, NFIAccountRedisModule, NFCAccountRedisModule)
}

void NFDBLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIAccountRedisModule, NFCAccountRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIMailRedisModule, NFCMailRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFICommonRedisModule, NFCCommonRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIBigMapRedisModule, NFCBigMapRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIGuildRedisModule, NFCGuildRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIPlayerRedisModule, NFCPlayerRedisModule)
	UNREGISTER_MODULE(pPluginManager, NFIRankRedisModule, NFCRankRedisModule)
}
