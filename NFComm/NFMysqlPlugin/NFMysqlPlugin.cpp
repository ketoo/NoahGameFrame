//------------------------------------------------------------------------ -
//    @FileName			:    NFMysqlPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include "NFMysqlPlugin.h"
#include "NFCMysqlModule.h"
#include "NFCAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFMysqlPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMysqlPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

const int NFMysqlPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFMysqlPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFMysqlPlugin);
}

void NFMysqlPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIMysqlModule, NFCMysqlModule)
    REGISTER_MODULE(pPluginManager, NFIAsyMysqlModule, NFCAsyMysqlModule)
}

void NFMysqlPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIAsyMysqlModule, NFCAsyMysqlModule)
    UNREGISTER_MODULE(pPluginManager, NFIMysqlModule, NFCMysqlModule)
}