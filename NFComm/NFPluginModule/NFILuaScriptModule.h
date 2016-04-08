// -------------------------------------------------------------------------
//    @FileName         £º    NFILuaScriptModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-07-08
//    @Module           £º    NFILuaScriptModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LUA_SCRIPT_MODULE_H
#define _NFI_LUA_SCRIPT_MODULE_H

#include "NFIScriptModule.h"

class NFILuaScriptModule
    : public NFIScriptModule
{
public:

};


static bool KernelModule_AddPropertyCallBack(NFINT64 kernelAddress, const NFGUID* self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NFILuaScriptModule* pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pKernelModule->GetPluginManager()->FindModule("NFCLuaScriptModule"));
    return pLuaScriptModule->AddPropertyCallBack(*self, strPropertyName, strComponentName, strFunction);
}

static bool KernelModule_AddRecordCallBack(NFINT64 kernelAddress, const NFGUID* self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NFILuaScriptModule* pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pKernelModule->GetPluginManager()->FindModule("NFCLuaScriptModule"));
    return pLuaScriptModule->AddRecordCallBack(*self, strRecordName, strComponentName, strFunction);
}

static bool KernelModule_AddEventCallBack(NFINT64 kernelAddress, const NFGUID* self, const int nEventID, const std::string& strComponentName, const std::string& strFunction)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NFILuaScriptModule* pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pKernelModule->GetPluginManager()->FindModule("NFCLuaScriptModule"));
    return pLuaScriptModule->AddEventCallBack(*self, nEventID, strComponentName, strFunction);
}

static bool KernelModule_AddHeartBeat(NFINT64 kernelAddress, const NFGUID* self, const std::string& strHeartBeatName, const std::string& strComponentName, const std::string& strFunction, const float fTime, const int nCount)
{
    NFIKernelModule* pKernelModule = (NFIKernelModule*)kernelAddress;
    NFILuaScriptModule* pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(pKernelModule->GetPluginManager()->FindModule("NFCLuaScriptModule"));
    return pLuaScriptModule->AddHeartBeatCallBack(*self, strHeartBeatName, strComponentName, strFunction, fTime, nCount);
}


#endif