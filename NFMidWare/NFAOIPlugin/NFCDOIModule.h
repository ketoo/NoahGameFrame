// -------------------------------------------------------------------------
//    @FileName			:    NFCDataAOIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-06-24
//    @Module           :    NFCDOIModule
//    @Desc             :    Why DOI(DATA OF INTEREST)? Some people who do not build the real ship with you so the game server shouldn't BC the data to you. For instance, there have 9 people around you and fighting, but u are not fighting with them so u would not receive the data change message about hp and mp of that people.
// -------------------------------------------------------------------------

#ifndef NFC_DOI_MODULE_H
#define NFC_DOI_MODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIAOIModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
////////////////////////////////////////////////////////////////////////////

#endif