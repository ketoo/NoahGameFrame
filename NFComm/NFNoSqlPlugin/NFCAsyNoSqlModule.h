// -------------------------------------------------------------------------
//    @FileName			:    NFCAsyNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCAsyNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ASY_NOSQL_MODULE_H
#define NFC_ASY_NOSQL_MODULE_H

#include "NFCNoSqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIAsyNoSqlModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCAsyNoSqlModule
    : public NFIAsyNoSqlModule
{
public:

	NFCAsyNoSqlModule(NFIPluginManager* p);
    virtual ~NFCAsyNoSqlModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();


protected:

};

#endif