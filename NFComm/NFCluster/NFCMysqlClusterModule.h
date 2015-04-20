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

#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCMysqlClusterModule
    : public NFILogicModule
{
public:

    NFCMysqlClusterModule(NFIPluginManager* p);
    virtual ~NFCMysqlClusterModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

private:

    NFCMysqlDriver* m_pDataBaseDriver;

  
    otl_connect motlConnect;

    std::string mstrMasterName;
    std::string mstrUID;
    std::string mstrPWD;

};

#endif