// -------------------------------------------------------------------------
//    @FileName			:    NFItemBagPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-03 08:51
//    @Module           :    NFItemBagPlugin
//
// -------------------------------------------------------------------------

#include "NFItemBagPlugin.h"
#include "NFCItemModule.h"
#include "NFCPackModule.h"
#include "NFCItemCardConsumeProcessModule.h"
#include "NFCItemEquipConsumeProcessModule.h"
#include "NFCItemGemConsumeProcessModule.h"
#include "NFCItemItemConsumeProcessModule.h"
#include "NFCItemTokenConsumeProcessModule.h"

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
	REGISTER_MODULE(pPluginManager, NFIItemCardConsumeProcessModule, NFCItemCardConsumeProcessModule);
	REGISTER_MODULE(pPluginManager, NFIItemEquipConsumeProcessModule, NFCItemEquipConsumeProcessModule);
	REGISTER_MODULE(pPluginManager, NFIItemGemConsumeProcessModule, NFCItemGemConsumeProcessModule);
	REGISTER_MODULE(pPluginManager, NFIItemItemConsumeProcessModule, NFCItemItemConsumeProcessModule);
	REGISTER_MODULE(pPluginManager, NFIItemTokenConsumeProcessModule, NFCItemTokenConsumeProcessModule);

	REGISTER_MODULE(pPluginManager, NFIItemModule, NFCItemModule)
	REGISTER_MODULE(pPluginManager, NFIPackModule, NFCPackModule)

}

void NFItemBagPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIPackModule, NFCPackModule)
	UNREGISTER_MODULE(pPluginManager, NFIItemModule, NFCItemModule)


	UNREGISTER_MODULE(pPluginManager, NFIItemTokenConsumeProcessModule, NFCItemTokenConsumeProcessModule);
	UNREGISTER_MODULE(pPluginManager, NFIItemItemConsumeProcessModule, NFCItemItemConsumeProcessModule);
	UNREGISTER_MODULE(pPluginManager, NFIItemGemConsumeProcessModule, NFCItemGemConsumeProcessModule);
	UNREGISTER_MODULE(pPluginManager, NFIItemEquipConsumeProcessModule, NFCItemEquipConsumeProcessModule);
	UNREGISTER_MODULE(pPluginManager, NFIItemCardConsumeProcessModule, NFCItemCardConsumeProcessModule);
}