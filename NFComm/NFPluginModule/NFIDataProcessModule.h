// -------------------------------------------------------------------------
//    @FileName         ��    NFIDataProcessModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2013-10-03
//    @Module           ��    NFIDataProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_DATAPROCESS_MODULE_H_
#define _NFI_DATAPROCESS_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFIActorDataModule.h"

class NFIDataProcessModule
    : public NFIActorDataModule
{

public:
    virtual int LoadDataFormNoSql(const NFIDENTID& self) = 0;
    virtual int SaveDataToNoSql( const NFIDENTID& self, bool bOffline = false ) = 0;
    virtual Theron::Address GetActorID(const NFIDENTID& self) = 0;
};

#endif