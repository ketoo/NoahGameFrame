// -------------------------------------------------------------------------
//    @FileName         :   NFIMountModule.h
//    @Author           :   eliteYang
//    @Date             :   2014/03/05
//    @Module           :   NFCMountModule
//    @Mail             :   elite_yang@163.com
//
// -------------------------------------------------------------------------
#ifndef _NFI_MOUNT_MODULE_H
#define _NFI_MOUNT_MODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFIMountModule
    : public NFILogicModule
{
public:
    virtual int AddMount(const NFGUID& self, const std::string& strPetID, int nRow = 0, bool bLevel = false) = 0;
    virtual int RemoveMount(const NFGUID& self, const std::string& strPetID, int nRow = 0) = 0;
    virtual void AddNewerMount( const NFGUID self, const int nLevel ) = 0;
};

#endif // !_NFI_MOUNT_MODULE_H