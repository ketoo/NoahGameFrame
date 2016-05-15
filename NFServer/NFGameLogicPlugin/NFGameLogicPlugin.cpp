// -------------------------------------------------------------------------
//    @FileName         :   NFGameServerPlugin.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2012-07-14 08:51
//    @Module           :   NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFGameLogicPlugin.h"
#include "NFCGameLogicModule.h"
#include "NFCBuffModule.h"
#include "NFCItemModule.h"
#include "NFCPackModule.h"
#include "NFCSkillModule.h"
#include "NFCBulletSkillConsumeProcessModule.h"
#include "NFCBriefSkillConsumeProcessModule.h"
#include "NFCSkillConsumeManagerModule.h"
#include "NFCPotionItemConsumeProcessModule.h"
#include "NFCCardItemConsumeProcessModule.h"
#include "NFCItemConsumeManagerModule.h"
#include "NFCNPCRefreshModule.h"
#include "NFCRebornItemConsumeProcessModule.h"
#include "NFCHeroModule.h"
#include "NFCEquipModule.h"
#include "NFCSLGShopModule.h"
#include "NFCSLGBuildingModule.h"
#include "NFCPVPModule.h"
#include "NFCMailModule.h"
#include "NFCRankModule.h"
#include "NFCCommonConfigModule.h"
#include "NFCGmModule.h"
#include "NFCTileModule.h"
#include "NFCHeroItemConsumeProcessModule.h"
#include "NFCNormalConsumeProcessModule.h"
#include "NFCGSSwichServerModule.h"

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
    return GET_CLASS_NAME( NFGameLogicPlugin )
}

void NFGameLogicPlugin::Install()
{

    REGISTER_MODULE(pPluginManager, NFCGameLogicModule)

    REGISTER_MODULE(pPluginManager, NFCBuffModule)
    REGISTER_MODULE(pPluginManager, NFCItemModule)
    REGISTER_MODULE(pPluginManager, NFCPackModule)
    REGISTER_MODULE(pPluginManager, NFCSkillModule)

    REGISTER_MODULE(pPluginManager, NFCRebornItemConsumeProcessModule)
    REGISTER_MODULE(pPluginManager, NFCItemConsumeManagerModule)
    REGISTER_MODULE(pPluginManager, NFCPotionItemConsumeProcessModule)
    REGISTER_MODULE(pPluginManager, NFCCardItemConsumeProcessModule)
    REGISTER_MODULE(pPluginManager, NFCHeroItemConsumeProcessModule)
    REGISTER_MODULE(pPluginManager, NFCNormalConsumeProcessModule)
    //Continue to ad other item types of consumption

    REGISTER_MODULE(pPluginManager, NFCSkillConsumeManagerModule)
    REGISTER_MODULE(pPluginManager, NFCBriefSkillConsumeProcessModule)
    REGISTER_MODULE(pPluginManager, NFCBulletSkillConsumeProcessModule)
    //Continue to add other skill types of consumption

	REGISTER_MODULE(pPluginManager, NFCHeroModule)
    REGISTER_MODULE(pPluginManager, NFCNPCRefreshModule)
	REGISTER_MODULE(pPluginManager, NFCEquipModule)
	REGISTER_MODULE(pPluginManager, NFCSLGShopModule)
	REGISTER_MODULE(pPluginManager, NFCSLGBuildingModule)
	REGISTER_MODULE(pPluginManager, NFCPVPModule)
	REGISTER_MODULE(pPluginManager, NFCMailModule);
	REGISTER_MODULE(pPluginManager, NFCRankModule);
    REGISTER_MODULE(pPluginManager, NFCCommonConfigModule);         
    REGISTER_MODULE(pPluginManager, NFCGmModule);
	REGISTER_MODULE(pPluginManager, NFCTileModule);
	REGISTER_MODULE(pPluginManager, NFCGSSwichServerModule);
}

void NFGameLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFCGSSwichServerModule);
	UNREGISTER_MODULE(pPluginManager, NFCTileModule);
    UNREGISTER_MODULE(pPluginManager, NFCGmModule);
    UNREGISTER_MODULE(pPluginManager, NFCCommonConfigModule);
	UNREGISTER_MODULE(pPluginManager, NFCRankModule);
    UNREGISTER_MODULE(pPluginManager, NFCMailModule);
    
	UNREGISTER_MODULE(pPluginManager, NFCPVPModule)
	UNREGISTER_MODULE(pPluginManager, NFCSLGBuildingModule)
	UNREGISTER_MODULE(pPluginManager, NFCSLGShopModule)
	UNREGISTER_MODULE(pPluginManager, NFCEquipModule)
    UNREGISTER_MODULE(pPluginManager, NFCNPCRefreshModule)
	UNREGISTER_MODULE(pPluginManager, NFCHeroModule)

    UNREGISTER_MODULE(pPluginManager, NFCBulletSkillConsumeProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFCBriefSkillConsumeProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFCSkillConsumeManagerModule)

    UNREGISTER_MODULE(pPluginManager, NFCNormalConsumeProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFCHeroItemConsumeProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFCCardItemConsumeProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFCPotionItemConsumeProcessModule)
    UNREGISTER_MODULE(pPluginManager, NFCItemConsumeManagerModule)
    UNREGISTER_MODULE(pPluginManager, NFCRebornItemConsumeProcessModule)

    UNREGISTER_MODULE(pPluginManager, NFCSkillModule)
    UNREGISTER_MODULE(pPluginManager, NFCPackModule)
    UNREGISTER_MODULE(pPluginManager, NFCItemModule)
    UNREGISTER_MODULE(pPluginManager, NFCBuffModule)

    UNREGISTER_MODULE(pPluginManager, NFCGameLogicModule)
}
