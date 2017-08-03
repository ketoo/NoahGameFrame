// -------------------------------------------------------------------------
//    @FileName			:    NFCUIPlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    NFCUIPlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCUIPlugin.h"

#include "NFCUIManager.h"

const int NFCUIPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFCUIPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCUIPlugin);
}

void NFCUIPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFCUIManager, NFCUIManager)
}

void NFCUIPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFCUIManager, NFCUIManager)
}
