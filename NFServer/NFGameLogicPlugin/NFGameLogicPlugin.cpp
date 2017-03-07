// -------------------------------------------------------------------------
//    @FileName         :   NFGameServerPlugin.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2012-07-14 08:51
//    @Module           :   NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFGameLogicPlugin.h"
#include "NFCBuffModule.h"
#include "NFCSkillModule.h"
#include "NFCEquipModule.h"
#include "NFCPVPModule.h"
#include "NFCMailModule.h"
#include "NFCRankModule.h"
#include "NFCCommonConfigModule.h"
#include "NFCGmModule.h"
#include "NFCGSSwichServerModule.h"
#include "NFCMapModule.h"
#include "NFCEquipPropertyModule.h"
#include "NFCTaskModule.h"
#include "NFCCreateRoleModule.h"
#include "NFCTileModule.h"
#include "NFCTeamPVPModule.h"

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

    REGISTER_MODULE(pPluginManager, NFIBuffModule, NFCBuffModule)
	REGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)
	REGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCCreateRoleModule)

	REGISTER_MODULE(pPluginManager, NFIEquipModule, NFCEquipModule)
	REGISTER_MODULE(pPluginManager, NFIPVPModule, NFCPVPModule)
	REGISTER_MODULE(pPluginManager, NFIMailModule, NFCMailModule);
	REGISTER_MODULE(pPluginManager, NFIRankModule, NFCRankModule);
    REGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCCommonConfigModule);
    REGISTER_MODULE(pPluginManager, NFIGmModule, NFCGmModule);
	REGISTER_MODULE(pPluginManager, NFIGSSwichServerModule, NFCGSSwichServerModule);
	REGISTER_MODULE(pPluginManager, NFIMapModule, NFCMapModule);
	REGISTER_MODULE(pPluginManager, NFIEquipPropertyModule, NFCEquipPropertyModule);
	REGISTER_MODULE(pPluginManager, NFITeamPVPModule, NFCTeamPVPModule);
	REGISTER_MODULE(pPluginManager, NFITileModule, NFCTileModule);
}

void NFGameLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFITileModule, NFCTileModule);
	UNREGISTER_MODULE(pPluginManager, NFITeamPVPModule, NFCTeamPVPModule);
	UNREGISTER_MODULE(pPluginManager, NFIEquipPropertyModule, NFCEquipPropertyModule);
	UNREGISTER_MODULE(pPluginManager, NFIMapModule, NFCMapModule);
	UNREGISTER_MODULE(pPluginManager, NFIGSSwichServerModule, NFCGSSwichServerModule);
    UNREGISTER_MODULE(pPluginManager, NFIGmModule, NFCGmModule);
    UNREGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCCommonConfigModule);
	UNREGISTER_MODULE(pPluginManager, NFIRankModule, NFCRankModule);
    UNREGISTER_MODULE(pPluginManager, NFIMailModule, NFCMailModule);
    
	UNREGISTER_MODULE(pPluginManager, NFIPVPModule, NFCPVPModule)
	UNREGISTER_MODULE(pPluginManager, NFIEquipModule, NFCEquipModule)

    UNREGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCCreateRoleModule)
	UNREGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)
    UNREGISTER_MODULE(pPluginManager, NFIBuffModule, NFCBuffModule)

}
