// -------------------------------------------------------------------------
//    @FileName         £º    NFIScriptModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-07-08
//    @Module           £º    NFIScriptModule
//    @Desc             :     every script plugin need to export two interfaces(NFCScriptVarList and most of NFCScriptKernel)
// -------------------------------------------------------------------------

#ifndef NFI_SCRIPT_MODULE_H
#define NFI_SCRIPT_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIScriptModule
    : public NFILogicModule
{

public:

};

#endif
