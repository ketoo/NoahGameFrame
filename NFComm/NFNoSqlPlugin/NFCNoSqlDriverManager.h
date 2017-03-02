// -------------------------------------------------------------------------
//    @FileName			:    NFCNoSqlDriverManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCNoSqlDriverManager
//
// -------------------------------------------------------------------------

#ifndef NFC_NOSQL_DRIVER_MANAGER_H
#define NFC_NOSQL_DRIVER_MANAGER_H

#include "NFCNoSqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlDriverManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCNoSqlDriverManager
    : public NFINoSqlDriverManager
{
public:

	NFCNoSqlDriverManager();
    virtual ~NFCNoSqlDriverManager();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();


protected:
	NFINT64 mLastCheckTime;

	NFCConsistentHashMapEx<std::string, NFINoSqlDriver> mxNoSqlDriver;

};

#endif