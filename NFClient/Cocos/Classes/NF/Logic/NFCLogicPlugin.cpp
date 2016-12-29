// -------------------------------------------------------------------------
//    @FileName			:    NFCLogicPlugin.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-26
//    @Module           :    NFCLogicPlugin
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCLogicPlugin.h"

#include "NFCLoginLogic.h"
#include "NFCPlayerLogic.h"
#include "NFCPropertyLogic.h"
#include "NFCRecordLogic.h"
#include "NFCNetLogic.h"
#include "NFCChatLogic.h"

const int NFCLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFCLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCLogicPlugin);
}

void NFCLogicPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFCNetLogic, NFCNetLogic);
	REGISTER_MODULE(pPluginManager, NFCLoginLogic, NFCLoginLogic);
	REGISTER_MODULE(pPluginManager, NFCPlayerLogic, NFCPlayerLogic);
	REGISTER_MODULE(pPluginManager, NFCPropertyLogic, NFCPropertyLogic);
	REGISTER_MODULE(pPluginManager, NFCRecordLogic, NFCRecordLogic);
	REGISTER_MODULE(pPluginManager, NFCChatLogic, NFCChatLogic);
}

void NFCLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFCNetLogic, NFCNetLogic);
	UNREGISTER_MODULE(pPluginManager, NFCLoginLogic, NFCLoginLogic);
	UNREGISTER_MODULE(pPluginManager, NFCPlayerLogic, NFCPlayerLogic);
	UNREGISTER_MODULE(pPluginManager, NFCPropertyLogic, NFCPropertyLogic);
	UNREGISTER_MODULE(pPluginManager, NFCRecordLogic, NFCRecordLogic);
	UNREGISTER_MODULE(pPluginManager, NFCChatLogic, NFCChatLogic);
}
