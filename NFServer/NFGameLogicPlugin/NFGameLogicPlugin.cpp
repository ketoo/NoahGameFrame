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
#include "NFCHeroItemConsumeProcessModule.h"
#include "NFCNormalConsumeProcessModule.h"
#include "NFCGSSwichServerModule.h"
#include "NFCMapModule.h"
#include "NFCHeroPropertyModule.h"
#include "NFCEquipPropertyModule.h"
#include "NFCTaskModule.h"
#include "NFCChatModule.h"

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

    REGISTER_MODULE(pPluginManager, NFIGameLogicModule, NFCGameLogicModule)

    REGISTER_MODULE(pPluginManager, NFIBuffModule, NFCBuffModule)
    REGISTER_MODULE(pPluginManager, NFIItemModule, NFCItemModule)
    REGISTER_MODULE(pPluginManager, NFIPackModule, NFCPackModule)
    REGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)

    //REGISTER_MODULE(pPluginManager, NFIRebornItemConsumeProcessModule, NFCRebornItemConsumeProcessModule)
    //REGISTER_MODULE(pPluginManager, NFIItemConsumeManagerModule, NFCItemConsumeManagerModule)
    //REGISTER_MODULE(pPluginManager, NFIPotionItemConsumeProcessModule, NFCPotionItemConsumeProcessModule)
    //REGISTER_MODULE(pPluginManager, NFICardItemConsumeProcessModule, NFCCardItemConsumeProcessModule)
    //REGISTER_MODULE(pPluginManager, NFIHeroItemConsumeProcessModule, NFCHeroItemConsumeProcessModule)
    //REGISTER_MODULE(pPluginManager, NFINormalConsumeProcessModule, NFCNormalConsumeProcessModule)
    //Continue to ad other item types of consumption

    //REGISTER_MODULE(pPluginManager, NFISkillConsumeManagerModule, NFCSkillConsumeManagerModule)
    //REGISTER_MODULE(pPluginManager, NFIBriefSkillConsumeProcessModule, NFCBriefSkillConsumeProcessModule)
    //REGISTER_MODULE(pPluginManager, NFIBulletSkillConsumeProcessModule, NFCBulletSkillConsumeProcessModule)
    //Continue to add other skill types of consumption

	REGISTER_MODULE(pPluginManager, NFIHeroModule, NFCHeroModule)
    REGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFCNPCRefreshModule)
	REGISTER_MODULE(pPluginManager, NFIEquipModule, NFCEquipModule)
	REGISTER_MODULE(pPluginManager, NFISLGShopModule, NFCSLGShopModule)
	REGISTER_MODULE(pPluginManager, NFISLGBuildingModule, NFCSLGBuildingModule)
	REGISTER_MODULE(pPluginManager, NFIPVPModule, NFCPVPModule)
	REGISTER_MODULE(pPluginManager, NFIMailModule, NFCMailModule);
	REGISTER_MODULE(pPluginManager, NFIRankModule, NFCRankModule);
    REGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCCommonConfigModule);
    REGISTER_MODULE(pPluginManager, NFIGmModule, NFCGmModule);
	REGISTER_MODULE(pPluginManager, NFIGSSwichServerModule, NFCGSSwichServerModule);
	REGISTER_MODULE(pPluginManager, NFIMapModule, NFCMapModule);
	REGISTER_MODULE(pPluginManager, NFIHeroPropertyModule, NFCHeroPropertyModule);
    REGISTER_MODULE(pPluginManager, NFIEquipPropertyModule, NFCEquipPropertyModule);
    //REGISTER_MODULE(pPluginManager, NFITaskModule, NFCTaskModule);
    //REGISTER_MODULE(pPluginManager, NFIChatModule, NFCChatModule);
}

void NFGameLogicPlugin::Uninstall()
{
    //UNREGISTER_MODULE(pPluginManager, NFCChatModule);
    //UNREGISTER_MODULE(pPluginManager, NFCTaskModule);
    UNREGISTER_MODULE(pPluginManager, NFIEquipPropertyModule, NFCEquipPropertyModule);
	UNREGISTER_MODULE(pPluginManager, NFIHeroPropertyModule, NFCHeroPropertyModule);
	UNREGISTER_MODULE(pPluginManager, NFIMapModule, NFCMapModule);
	UNREGISTER_MODULE(pPluginManager, NFIGSSwichServerModule, NFCGSSwichServerModule);
    UNREGISTER_MODULE(pPluginManager, NFIGmModule, NFCGmModule);
    UNREGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCCommonConfigModule);
	UNREGISTER_MODULE(pPluginManager, NFIRankModule, NFCRankModule);
    UNREGISTER_MODULE(pPluginManager, NFIMailModule, NFCMailModule);
    
	UNREGISTER_MODULE(pPluginManager, NFIPVPModule, NFCPVPModule)
	UNREGISTER_MODULE(pPluginManager, NFISLGBuildingModule, NFCSLGBuildingModule)
	UNREGISTER_MODULE(pPluginManager, NFISLGShopModule, NFCSLGShopModule)
	UNREGISTER_MODULE(pPluginManager, NFIEquipModule, NFCEquipModule)
    UNREGISTER_MODULE(pPluginManager, NFINPCRefreshModule, NFCNPCRefreshModule)
	UNREGISTER_MODULE(pPluginManager, NFIHeroModule, NFCHeroModule)

    //UNREGISTER_MODULE(pPluginManager, NFIBulletSkillConsumeProcessModule, NFCBulletSkillConsumeProcessModule)
    //UNREGISTER_MODULE(pPluginManager, NFIBriefSkillConsumeProcessModule, NFCBriefSkillConsumeProcessModule)
    //UNREGISTER_MODULE(pPluginManager, NFISkillConsumeManagerModule, NFCSkillConsumeManagerModule)

    //UNREGISTER_MODULE(pPluginManager, NFINormalConsumeProcessModule, NFCNormalConsumeProcessModule)
    //UNREGISTER_MODULE(pPluginManager, NFIHeroItemConsumeProcessModule, NFCHeroItemConsumeProcessModule)
    //UNREGISTER_MODULE(pPluginManager, NFICardItemConsumeProcessModule, NFCCardItemConsumeProcessModule)
    //UNREGISTER_MODULE(pPluginManager, NFIPotionItemConsumeProcessModule, NFCPotionItemConsumeProcessModule)
    //UNREGISTER_MODULE(pPluginManager, NFIItemConsumeManagerModule, NFCItemConsumeManagerModule)
    //UNREGISTER_MODULE(pPluginManager, NFIRebornItemConsumeProcessModule, NFCRebornItemConsumeProcessModule)

    UNREGISTER_MODULE(pPluginManager, NFISkillModule, NFCSkillModule)
    UNREGISTER_MODULE(pPluginManager, NFIPackModule, NFCPackModule)
    UNREGISTER_MODULE(pPluginManager, NFIItemModule, NFCItemModule)
    UNREGISTER_MODULE(pPluginManager, NFIBuffModule, NFCBuffModule)

    UNREGISTER_MODULE(pPluginManager, NFIGameLogicModule, NFCGameLogicModule)
}
