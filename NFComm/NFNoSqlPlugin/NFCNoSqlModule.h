// -------------------------------------------------------------------------
//    @FileName			:    NFCNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFCNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_DATANOSQL_MODULE_H
#define NFC_DATANOSQL_MODULE_H

#include "NFCNoSqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFCNoSqlModule
    : public NFINoSqlModule
{
public:

    NFCNoSqlModule(NFIPluginManager* p);
    virtual ~NFCNoSqlModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

	virtual bool ConnectSql(const std::string& strIP);
	virtual bool ConnectSql(const std::string& strIP, const int nPort, const std::string& strPass);

    virtual NFINoSqlDriver* GetDriver()
    {
        return m_pNoSqlDriver;
    }

protected:

    NFINoSqlDriver* m_pNoSqlDriver;//player property

};

#endif