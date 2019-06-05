/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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


#include "NFGameLogicPlugin.h"
#include "NFBuffModule.h"
#include "NFGamePVPModule.h"
//#include "NFMapModule.h"
#include "NFCreateRoleModule.h"
//#include "NFTileModule.h"
//#include "NFSurvivalModule.h"
//#include "NFTileMapModule.h"
//#include "NFHomeModule.h"
#include "NFSyncModule.h"
#include "NFScenePropsModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin( NFIPluginManager* pm )
{
    CREATE_PLUGIN( pm, NFGameLogicPlugin )

};

NF_EXPORT void DllStopPlugin( NFIPluginManager* pm )
{
    DESTROY_PLUGIN( pm, NFGameLogicPlugin )
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameLogicPlugin);
}

void NFGameLogicPlugin::Install()
{
	
	REGISTER_MODULE(pPluginManager, NFIScenePropsModule, NFScenePropsModule)
	REGISTER_MODULE(pPluginManager, NFIBuffModule, NFBuffModule)
	REGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCreateRoleModule)
	REGISTER_MODULE(pPluginManager, NFIGamePVPModule, NFGamePVPModule)
	REGISTER_MODULE(pPluginManager, NFISyncModule, NFSyncModule)
	//REGISTER_MODULE(pPluginManager, NFIMapModule, NFMapModule);
	//REGISTER_MODULE(pPluginManager, NFITileModule, NFTileModule);
	//REGISTER_MODULE(pPluginManager, NFISurvivalModule, NFSurvivalModule);
	//REGISTER_MODULE(pPluginManager, NFITileMapModule, NFTileMapModule);
	//REGISTER_MODULE(pPluginManager, NFIHomeModule, NFHomeModule);
}

void NFGameLogicPlugin::Uninstall()
{
	//UNREGISTER_MODULE(pPluginManager, NFIHomeModule, NFHomeModule);
	//UNREGISTER_MODULE(pPluginManager, NFITileMapModule, NFTileMapModule);
	//UNREGISTER_MODULE(pPluginManager, NFISurvivalModule, NFSurvivalModule);
	//UNREGISTER_MODULE(pPluginManager, NFITileModule, NFTileModule);
	//UNREGISTER_MODULE(pPluginManager, NFIMapModule, NFMapModule);
	UNREGISTER_MODULE(pPluginManager, NFIScenePropsModule, NFScenePropsModule)
	UNREGISTER_MODULE(pPluginManager, NFISyncModule, NFSyncModule)
	UNREGISTER_MODULE(pPluginManager, NFIGamePVPModule, NFGamePVPModule)
    UNREGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCreateRoleModule)
    UNREGISTER_MODULE(pPluginManager, NFIBuffModule, NFBuffModule)

}
