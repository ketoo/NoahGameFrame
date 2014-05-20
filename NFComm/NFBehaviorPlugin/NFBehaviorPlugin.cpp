#include "stdafx.h"
#include "NFBehaviorPlugin.h"

NFIPlugin* pPlugin = NULL;

extern "C"  __declspec(dllexport) void DllStartPlugin(NFIPluginManager* pm)
{
    pPlugin = new NFCBehaviorPlugin();
    pm->Registered(pPlugin);

};

extern "C" __declspec(dllexport) void DllStopPlugin(NFIPluginManager* pm)
{
    delete pPlugin;
    pPlugin = NULL;
};

//////////////////////////////////////////////////////////////////////////

const int NFCBehaviorPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFCBehaviorPlugin::GetPluginName()
{
    return "NFBehaviorPlugin";
}

void NFCBehaviorPlugin::Install()
{

}

void NFCBehaviorPlugin::Uninstall()
{

}