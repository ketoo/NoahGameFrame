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

    virtual int ExistRole(const std::string& strRoleName);

    virtual int CreateRole(const std::string& strAccount, const std::string& strRoleName);

    virtual int DeleteRole(const std::string& strAccount, const std::string& strRoleName);
    //操作数据库接口
    //////////////////////////////////////////////////////////////////////////

    virtual int QueryAccountProperty(const std::string& strAccount, NFIValueList& valueListKeys, NFIValueList& valueListValues);
    virtual int QueryAccountRoleList(const std::string& strAccount, NFIValueList& value);
    virtual int QueryRoleProperty(const std::string& strRoleName, NFIValueList& valueListKeys, NFIValueList& valueListValues);
    virtual int QueryRoleRecord(const std::string& strRoleName, const std::string& strRecordName,  NFIValueList& valueListKeys, NFIValueList& valueListValues);

    virtual int SetAccountProperty(const std::string& strAccount, const NFIValueList& valueListKeys, const NFIValueList& valueListValues);
    virtual int SetRoleProperty(const std::string& strRoleName, const NFIValueList& valueListKeys, const NFIValueList& valueListValues);
    virtual int SetRoleRecord(const std::string& strRoleName, const std::string& strRecordName, const NFIValueList& valueListKeys, const NFIValueList& valueListValues);

    virtual void SavePlayerData();

protected:

    NFCDataNoSqlDriver* m_pPlayerDataDriver;//player property

};

#endif