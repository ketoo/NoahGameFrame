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

class NFCMysqlClusterModule
    : public NFIMysqlClusterModule
{
public:

    NFCMysqlClusterModule();
    virtual ~NFCMysqlClusterModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

private:

    NFCDataBaseDriver* m_pDataBaseDriver;

  
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