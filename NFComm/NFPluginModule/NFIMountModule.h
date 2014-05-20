// -------------------------------------------------------------------------
//    @FileName         :   NFIMountModule.h
//    @Author           :   eliteYang
//    @Date             :   2014/03/05
//    @Module           :   NFCMountModule
//    @Mail             :   elite_yang@163.com
//
// -------------------------------------------------------------------------
#ifndef _NFI_MOUNT_MODULE_H_
#define _NFI_MOUNT_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIMountModule
    : public NFILogicModule
{
public:
    virtual int AddMount(const NFIDENTID& self, const std::string& strPetID, int nRow = 0, bool bLevel = false) = 0;
    virtual int RemoveMount(const NFIDENTID& self, const std::string& strPetID, int nRow = 0) = 0;
    virtual void AddNewerMount( const NFIDENTID self, const int nLevel ) = 0;
};

#endif // !_NFI_MOUNT_MODULE_H_