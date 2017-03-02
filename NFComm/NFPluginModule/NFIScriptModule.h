// -------------------------------------------------------------------------




//    @Desc             :     every script plugin need to export two interfaces(NFCScriptVarList and most of NFCScriptKernel)
// -------------------------------------------------------------------------

#ifndef NFI_SCRIPT_MODULE_H
#define NFI_SCRIPT_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFCore/NFDataList.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIScriptModule
    : public NFIModule
{

public:

};

#endif
