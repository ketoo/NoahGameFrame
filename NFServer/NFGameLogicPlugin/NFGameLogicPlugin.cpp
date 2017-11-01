// -------------------------------------------------------------------------
//    @FileName         :   NFGameServerPlugin.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2012-07-14 08:51
//    @Module           :   NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFGameLogicPlugin.h"
#include "NFCBuffModule.h"
#include "NFCPVPModule.h"
#include "NFCCommonConfigModule.h"
#include "NFCMapModule.h"
#include "NFCCreateRoleModule.h"
#include "NFCTileModule.h"

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
	REGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCCreateRoleModule)
	REGISTER_MODULE(pPluginManager, NFIPVPModule, NFCPVPModule)
    REGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCCommonConfigModule);
	REGISTER_MODULE(pPluginManager, NFIMapModule, NFCMapModule);
	REGISTER_MODULE(pPluginManager, NFITileModule, NFCTileModule);
}

void NFGameLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFITileModule, NFCTileModule);
	UNREGISTER_MODULE(pPluginManager, NFIMapModule, NFCMapModule);
    UNREGISTER_MODULE(pPluginManager, NFICommonConfigModule, NFCCommonConfigModule);
	UNREGISTER_MODULE(pPluginManager, NFIPVPModule, NFCPVPModule)
    UNREGISTER_MODULE(pPluginManager, NFICreateRoleModule, NFCCreateRoleModule)
    UNREGISTER_MODULE(pPluginManager, NFIBuffModule, NFCBuffModule)

}
