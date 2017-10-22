// -------------------------------------------------------------------------
//    @FileName			:    NFCScalePlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-10-23 12:22
//    @Module           :    NFCScalePlugin
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCScaleModule.h"
#include "NFScalePlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, NFScalePlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFScalePlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFScalePlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFScalePlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCScalePlugin);
}

void NFScalePlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIScaleModule, NFCScaleModule)

}

void NFScalePlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIScaleModule, NFCScaleModule)
}