// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//
//    数据格式
//      Property:   HP,1;Mp,2;Exp,3;
//          属性,属性值;属性,属性值;属性,属性值;属性,属性值;
//
//      Record:     Friends,20,5;1,2,3,4,5;11,22,33,44,55;
//          表名,表行,表列;行1的值(行列的个数);行2的值(行列的个数);
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

    //数据库表配置文件
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