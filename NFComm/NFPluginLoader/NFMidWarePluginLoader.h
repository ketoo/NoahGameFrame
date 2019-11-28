#ifndef NF_MIDWARE_PLUGIN_MANAGER_H
#define NF_MIDWARE_PLUGIN_MANAGER_H

#ifndef NF_DYNAMIC_PLUGIN
#include "NFComm/NFPluginModule/NFIPluginManager.h"
//MIDWARE
#include "NFMidWare/NFAIPlugin/NFAIPlugin.h"
#include "NFMidWare/NFAOIPlugin/NFAOIPlugin.h"
#include "NFMidWare/NFChatPlugin/NFChatPlugin.h"
#include "NFMidWare/NFFriendPlugin/NFFriendPlugin.h"
#include "NFMidWare/NFTeamPlugin/NFTeamPlugin.h"
#include "NFMidWare/NFClanPlugin/NFClanPlugin.h"
#include "NFMidWare/NFHeroPlugin/NFHeroPlugin.h"
#include "NFMidWare/NFItemBagPlugin/NFItemBagPlugin.h"
#include "NFMidWare/NFMailPlugin/NFMailPlugin.h"
#include "NFMidWare/NFRankPlugin/NFRankPlugin.h"
#include "NFMidWare/NFShopPlugin/NFShopPlugin.h"
#include "NFMidWare/NFSkillPlugin/NFSkillPlugin.h"
#include "NFMidWare/NFTaskPlugin/NFTaskPlugin.h"
#include "NFMidWare/NFUserGiftPlugin/NFUserGiftPlugin.h"
//TUTORIAL
#include "Tutorial/Tutorial1/Tutorial1.h"
#include "Tutorial/Tutorial2/Tutorial2.h"
#include "Tutorial/Tutorial3/Tutorial3Plugin.h"
#include "Tutorial/Tutorial4/Tutorial4Plugin.h"
#include "Tutorial/Tutorial5/Tutorial5.h"
#include "Tutorial/Tutorial6/Tutorial6.h"
#include "Tutorial/Tutorial7/Tutorial7.h"

class NFMidWarePluginLoader
{
public:
   static void LoadStaticPlugin(NFIPluginManager* pPluginManager)
   {
		 //MIDWARE
		CREATE_PLUGIN(pPluginManager, NFChatPlugin)
		CREATE_PLUGIN(pPluginManager, NFFriendPlugin)
		CREATE_PLUGIN(pPluginManager, NFClanPlugin)
		CREATE_PLUGIN(pPluginManager, NFHeroPlugin)
		CREATE_PLUGIN(pPluginManager, NFItemBagPlugin)
		CREATE_PLUGIN(pPluginManager, NFMailPlugin)
		CREATE_PLUGIN(pPluginManager, NFRankPlugin)
		CREATE_PLUGIN(pPluginManager, NFShopPlugin)
		CREATE_PLUGIN(pPluginManager, NFSkillPlugin)
		CREATE_PLUGIN(pPluginManager, NFTaskPlugin)
		CREATE_PLUGIN(pPluginManager, NFUserGiftPlugin)
		CREATE_PLUGIN(pPluginManager, NFTeamPlugin)

		//TUTORIAL
		CREATE_PLUGIN(pPluginManager, Tutorial1)
		CREATE_PLUGIN(pPluginManager, Tutorial2)
		CREATE_PLUGIN(pPluginManager, Tutorial3Plugin)
		CREATE_PLUGIN(pPluginManager, Tutorial4Plugin)
		CREATE_PLUGIN(pPluginManager, Tutorial5)
		CREATE_PLUGIN(pPluginManager, Tutorial6)
		CREATE_PLUGIN(pPluginManager, Tutorial7)  
   }
};

#endif

#endif
