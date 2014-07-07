// -------------------------------------------------------------------------
//    @FileName      :    NFGameServerPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------


////#include "stdafx.h"
#include "NFGameServerPlugin.h"
#include "NFCGameLogicModule.h"
#include "NFCSceneProcessModule.h"
#include "NFCBuffModule.h"
#include "NFCItemModule.h"
#include "NFCPackModule.h"
#include "NFCPropertyModule.h"
#include "NFCSkillModule.h"
#include "NFCLevelModule.h"
#include "NFCPropertyConfigModule.h"
#include "NFCBulletSkillConsumeProcessModule.h"
#include "NFCBriefSkillConsumeProcessModule.h"
#include "NFCSkillConsumeManagerModule.h"
#include "NFCPotionItemConsumeProcessModule.h"
#include "NFCCardItemConsumeProcessModule.h"
#include "NFCItemConsumeManagerModule.h"
#include "NFCNPCRefreshModule.h"
#include "NFCRebornItemConsumeProcessModule.h"
#include "NFCDataProcessModule.h"

#ifdef NF_DYNAMIC_PLUGIN

extern "C"  __declspec( dllexport ) void DllStartPlugin( NFIPluginManager* pm )
{
	SetConsoleTitle( "NFGameServer" );

    CREATE_PLUGIN( pm, NFGameServerPlugin )

};

extern "C" __declspec( dllexport ) void DllStopPlugin( NFIPluginManager* pm )
{
    DESTROY_PLUGIN( pm, NFGameServerPlugin )
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFGameServerPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFGameServerPlugin::GetPluginName()
{
    GET_PLUGIN_NAME( NFGameServerPlugin )
}

void NFGameServerPlugin::Install()
{

    REGISTER_MODULE( pPluginManager, NFCGameLogicModule )
    REGISTER_MODULE( pPluginManager, NFCSceneProcessModule )
    REGISTER_MODULE( pPluginManager, NFCDataProcessModule )

    REGISTER_MODULE( pPluginManager, NFCBuffModule )
    REGISTER_MODULE( pPluginManager, NFCItemModule )
    REGISTER_MODULE( pPluginManager, NFCPackModule )
    REGISTER_MODULE( pPluginManager, NFCPropertyModule )
    REGISTER_MODULE( pPluginManager, NFCSkillModule )
    REGISTER_MODULE( pPluginManager, NFCNPCRefreshModule )

    REGISTER_MODULE( pPluginManager, NFCLevelModule )
    REGISTER_MODULE( pPluginManager, NFCPropertyConfigModule )

    REGISTER_MODULE( pPluginManager, NFCRebornItemConsumeProcessModule )
    REGISTER_MODULE( pPluginManager, NFCItemConsumeManagerModule )
    REGISTER_MODULE( pPluginManager, NFCPotionItemConsumeProcessModule )
    REGISTER_MODULE( pPluginManager, NFCCardItemConsumeProcessModule )
    //Continue to add other item types of consumption

    REGISTER_MODULE( pPluginManager, NFCSkillConsumeManagerModule )
    REGISTER_MODULE( pPluginManager, NFCBriefSkillConsumeProcessModule )
    REGISTER_MODULE( pPluginManager, NFCBulletSkillConsumeProcessModule )
    //Continue to add other skill types of consumption

}

void NFGameServerPlugin::Uninstall()
{
	
    UNREGISTER_MODULE( pPluginManager, NFCBulletSkillConsumeProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCBriefSkillConsumeProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCSkillConsumeManagerModule )

    UNREGISTER_MODULE( pPluginManager, NFCCardItemConsumeProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCPotionItemConsumeProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCItemConsumeManagerModule )
    UNREGISTER_MODULE( pPluginManager, NFCRebornItemConsumeProcessModule )

    UNREGISTER_MODULE( pPluginManager, NFCPropertyConfigModule )
    UNREGISTER_MODULE( pPluginManager, NFCLevelModule )

    UNREGISTER_MODULE( pPluginManager, NFCNPCRefreshModule )
    UNREGISTER_MODULE( pPluginManager, NFCSkillModule )
    UNREGISTER_MODULE( pPluginManager, NFCPropertyModule )
    UNREGISTER_MODULE( pPluginManager, NFCPackModule )
    UNREGISTER_MODULE( pPluginManager, NFCItemModule )
    UNREGISTER_MODULE( pPluginManager, NFCBuffModule )

    UNREGISTER_MODULE( pPluginManager, NFCDataProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCSceneProcessModule )
    UNREGISTER_MODULE( pPluginManager, NFCGameLogicModule )
}
