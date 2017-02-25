// -------------------------------------------------------------------------
//    @FileName      	:   NFCItemConsumeManagerModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCItemConsumeManagerModule
// -------------------------------------------------------------------------

#include "NFCItemConsumeManagerModule.h"
#include "NFCItemCardConsumeProcessModule.h"
#include "NFCItemEquipConsumeProcessModule.h"
#include "NFCItemGemConsumeProcessModule.h"
#include "NFCItemTokenConsumeProcessModule.h"
#include "NFCItemItemConsumeProcessModule.h"

bool NFCItemConsumeManagerModule::Init()
{
	ResgisterConsumeModule(NFMsg::EItemType::EIT_CARD, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemCardConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_EQUIP, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemEquipConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_GEM, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemGemConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_TOKEN, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemTokenConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::EItemType::EIT_ITEM, NF_SHARE_PTR<NFIItemConsumeProcessModule>(NF_NEW NFCItemItemConsumeProcessModule(pPluginManager)));

    return true;
}

bool NFCItemConsumeManagerModule::Shut()
{
    return true;
}

bool NFCItemConsumeManagerModule::Execute()
{
    return true;
}

bool NFCItemConsumeManagerModule::AfterInit()
{
    return true;
}

bool NFCItemConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NF_SHARE_PTR<NFIItemConsumeProcessModule> pModule )
{
	if (ExistElement(nModuleType))
	{
		assert(0);
		return false;
	}

    return AddElement( nModuleType, pModule );
}

NF_SHARE_PTR<NFIItemConsumeProcessModule> NFCItemConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
