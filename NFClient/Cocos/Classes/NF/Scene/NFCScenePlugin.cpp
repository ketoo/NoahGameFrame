// -------------------------------------------------------------------------
//    @FileName			:    NFCScenePlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    NFCScenePlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCScenePlugin.h"

#include "NFCSceneManager.h"

const int NFCScenePlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFCScenePlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCScenePlugin);
}

void NFCScenePlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFCSceneManager, NFCSceneManager)
}

void NFCScenePlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFCSceneManager, NFCSceneManager)
}
