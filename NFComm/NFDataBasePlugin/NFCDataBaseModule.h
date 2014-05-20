// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//
//    ���ݸ�ʽ
//      Property:   HP,1;Mp,2;Exp,3;
//          ����,����ֵ;����,����ֵ;����,����ֵ;����,����ֵ;
//
//      Record:     Friends,20,5;1,2,3,4,5;11,22,33,44,55;
//          ����,����,����;��1��ֵ(���еĸ���);��2��ֵ(���еĸ���);
//
// -------------------------------------------------------------------------

#ifndef _NFC_DATABASE_MODULE_H_
#define _NFC_DATABASE_MODULE_H_

#include "NFCDataBaseDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIDataBaseModule.h"
#include "NFComm/NFPluginModule/NFIShareMemoryModule.h"

class NFCDataBaseModule
    : public NFIDataBaseModule
{
public:

    NFCDataBaseModule();
    virtual ~NFCDataBaseModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

    virtual int ConfirmAccountInfo(const std::string& strAccount, const std::string& strPassword);

    virtual int HasAccount(const std::string& strAccount);

    virtual int CreateAccount(const std::string& strAccount, const std::string& strPassword);

    virtual int HasRole(const std::string& strRoleName);

    virtual int CreateRole(const std::string& strAccount, const std::string& strRoleName);

    virtual int DeleteRole(const std::string& strAccount, const std::string& strRoleName);
    //�������ݿ�ӿ�
    //////////////////////////////////////////////////////////////////////////
    /*
        property : value ÿ��Ԫ�ؾ���һ������������ֵ�����ݣ��м��ö��ŷֿ�,�������ݿ��ʱ�������ڲ��Լ����Ǹ��ֺð����Էֿ�
        record : value ÿ������  ����,col,row,col0*row0,col1*row1,col2*row2;һ�ű�һ���ֺ�
    */

    virtual int QueryAccountProperty(const std::string& strAccount, NFIValueList& value);
    virtual int QueryAccountRecord(const std::string& strAccount, NFIValueList& value);
    virtual int QueryAccountRoleList(const std::string& strAccount, NFIValueList& value);
    virtual int QueryRoleProperty(const std::string& strRoleName, NFIValueList& value);
    virtual int QueryRoleRecord(const std::string& strRoleName, NFIValueList& value);

    virtual int SetAccountProperty(const std::string& strAccount, const NFIValueList& value);
    virtual int SetAccountRecord(const std::string& strAccount, const NFIValueList& value);
    virtual int SetRoleProperty(const std::string& strRoleName, const NFIValueList& value);
    virtual int SetRoleRecord(const std::string& strRoleName, const NFIValueList& value);

protected:

    void CheckTable();

    void AddPropertyToRole(const std::string& strProperty);
    void AddPropertyToAccount(const std::string& strProperty);
    void AddRecordToRole(const std::string& strProperty);
    void AddRecordToAccount(const std::string& strProperty);

    int HasPropertyInRole(const std::string& strProperty);
    int HasPropertyInAccount(const std::string& strProperty);

private:

    NFCDataBaseDriver* m_pDataBaseDriver;

    static NFIShareMemoryModule* m_pShareMemoryModule;

    otl_connect motlConnect;

    //���ݿ�������ļ�
    NFConfig mConfig;
    std::string mstrDataBaseConfigFile;

    std::string mstrMasterName;
    std::string mstrUID;
    std::string mstrPWD;
    std::string mstrDSN;

    std::string mstrAccountTableName;
    std::string mstrPlayerTableName;
    std::string mstrGlobalTableName;

};

#endif