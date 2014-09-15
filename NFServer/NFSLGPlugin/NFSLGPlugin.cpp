// -------------------------------------------------------------------------
//    @FileName      :    NFSLGPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-14 08:51
//    @Module           :    NFSLGPlugin
//
// -------------------------------------------------------------------------

#include "NFSLGPlugin.h"

#ifdef NF_DYNAMIC_PLUGIN

extern "C"  __declspec( dllexport ) void DllStartPlugin( NFIPluginManager* pm )
{
    CREATE_PLUGIN( pm, NFSLGPlugin )

};

extern "C" __declspec( dllexport ) void DllStopPlugin( NFIPluginManager* pm )
{
    DESTROY_PLUGIN( pm, NFSLGPlugin )
};

#endif
//////////////////////////////////////////////////////////////////////////

const int NFSLGPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFSLGPlugin::GetPluginName()
{
    GET_PLUGIN_NAME( NFSLGPlugin )
}

void NFSLGPlugin::Install()
{


}

void NFSLGPlugin::Uninstall()
{

}
