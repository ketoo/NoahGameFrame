// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFCDataNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_DATANOSQL_MODULE_H_
#define _NFC_DATANOSQL_MODULE_H_

#include "NFCDataNoSqlDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"

class NFCDataNoSqlModule
    : public NFIDataNoSqlModule
{
public:

    NFCDataNoSqlModule(NFIPluginManager* p);
    virtual ~NFCDataNoSqlModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

    virtual NFIDataNoSqlDriver* GetDriver()
    {
        return m_pPlayerDataDriver;
    }

protected:

    NFCDataNoSqlDriver* m_pPlayerDataDriver;//player property

};

#endif