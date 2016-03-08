// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyMysqlClusterModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-31
//    @Module           :    NFCAsyMysqlClusterModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ASY_MYSQL_CLUSTER_MODULE_H
#define NFC_ASY_MYSQL_CLUSTER_MODULE_H

#include "NFCMysqlDriver.h"
#include "NFComm/NFMessageDefine/NFMsgMysql.pb.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIAsyClusterModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"

struct SMysqlParam
{
    enum EMysqlOPRType
    {
        EMYSQLOPRTYPE_NONE      = 0,
        EMYSQLOPRTYPE_UPDATA    = 1,
        EMYSQLOPRTYPE_QUERY     = 2,
        EMYSQLOPRTYPE_SELECT    = 3,
        EMYSQLOPRTYPE_DELETE    = 4,
        EMYSQLOPRTYPE_EXISTS    = 5,
        EMYSQLOPRTYPE_KEYS      = 6,

    };
    SMysqlParam()
    {
        nRet = 0;
        nReqID = 0;
        bExit = 0;
        eType = EMYSQLOPRTYPE_NONE;
    }

public:
    bool PackParam(std::string& strData);
    bool UnPackParam(const std::string& strData);

    std::string strRecordName;
    std::string strKey;
    std::vector<std::string> fieldVec;
    std::vector<std::string> valueVec;
    int bExit;
    int  nRet;
    int  nReqID;
    EMysqlOPRType eType;
    MYSQL_RETURN_FUNCTOR mFunReturnRsp;
    MYSQL_RETURN_INT_FUNCTOR mFunReturnIntRsp;
    MYSQL_RETURN_VECVALUE_FUNCTOR mFunReturnVecValueRsp;
    MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR mFunReturnVeckKeyValueRsp;
    std::string mstrUseData;
    NFGUID self;
};

class NFCMysqlComponent : public NFIComponent
{
public:
    NFCMysqlComponent(NFGUID self, const std::string& strName): NFIComponent(self, strName)
    {
    }

    NFCMysqlComponent(NFIPluginManager* pPluginManager): NFIComponent(NFGUID(), "")
    {
    }

    virtual ~NFCMysqlComponent()
    {

    }

    virtual bool Init();
    virtual bool AfterInit();
    virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg);

    int OnASyUseMysqlEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyAddMysqlServerEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyKeepServerAliveEvent(const NFGUID& self, const int event, std::string& arg);


protected:
    virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance();

    NF_SHARE_PTR<NFIMysqlDriverManager> m_pMysqlDriverManager;
};


class NFCAsyMysqlClusterModule
    : public NFIAsyClusterModule
{
public:
    enum ACOTERMYSQLEVENT
    {
        ACOTERMYSQLEVENT_USEDB        = 1,
        ACOTERMYSQLEVENT_INISERVER       = 2,
        ACOTERMYSQLEVENT_KEEPALIVESERVER = 3,
    };
public:

    NFCAsyMysqlClusterModule(NFIPluginManager* p);
    virtual ~NFCAsyMysqlClusterModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool Updata(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData);
    virtual bool Query(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData);
    virtual bool Select(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData);

    virtual bool Delete(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData);
    virtual bool Exists(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_INT_FUNCTOR& mFunReturnIntRsp, const std::string& strUseData);
    virtual bool Keys(const NFGUID& self, const std::string& strRecordName, const std::string& strKeyName, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnVecValueRsp, const std::string& strUseData);

    virtual bool StartActorPool(const int nCount);
    virtual bool CloseActorPool();

    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1);

protected:
    int ApplyRequest(NF_SHARE_PTR<SMysqlParam> pParam);
    int RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);

    int OnUseMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnAddMysqlServerAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnKeepServerAliveAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);

    int GetActor();
    bool KeepAliveMysqlServer();
private:
    static std::string strDefaultKey;
    static std::string strDefaultTable;

private:
    NFMapEx<int, int> mActorList; //actorid <-->Used
    NFMapEx<int, SMysqlParam> mReqList;// reqID <-->Param
    int nCurReqID;
    int mnSuitIndex;
    NFINT64 mnLastCheckTime;

};

#endif