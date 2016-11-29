// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyMysqlModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-31
//    @Module           :    NFCAsyMysqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ASY_MYSQL_MODULE_H
#define NFC_ASY_MYSQL_MODULE_H

#include "NFCMysqlDriver.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

struct SMysqlBaseParam
{
public:
    SMysqlBaseParam()
    {
        nRet = 0;
        nReqID = 0;
        bExit = 0;
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
    std::string mstrUseData;
    NFGUID self;
    NFIAsyMysqlModule::ACOTERMYSQLEVENT eType;

    virtual int GetType() { return eType; };
};

struct SMysqlUpDataParam : public SMysqlBaseParam
{
    SMysqlUpDataParam()
    {
        eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_UPDATA;
    }
public:
    MYSQL_RETURN_FUNCTOR mFunReturnRsp;
};

struct SMysqlQueryParam : public SMysqlBaseParam
{
    SMysqlQueryParam()
    {
        eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_QUERY;
    }

public:
    MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR mFunReturnVeckKeyValueRsp;
};

struct SMysqlSelectParam : public SMysqlBaseParam
{
    SMysqlSelectParam()
    {
        eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_SELECT;
    }

public:
    MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR mFunReturnVeckKeyValueRsp;
};

struct SMysqlDeleteParam : public SMysqlBaseParam
{
    SMysqlDeleteParam()
    {
        eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_DELETE;
    }

public:
    MYSQL_RETURN_FUNCTOR mFunReturnRsp;
};

struct SMysqlExistsParam : public SMysqlBaseParam
{
    SMysqlExistsParam()
    {
        eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_EXISTS;
    }

public:
    MYSQL_RETURN_INT_FUNCTOR mFunReturnIntRsp;
};

struct SMysqlKeysParam : public SMysqlBaseParam
{
    SMysqlKeysParam()
    {
        eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_KEYS;
    }

public:
    MYSQL_RETURN_VECVALUE_FUNCTOR mFunReturnVecValueRsp;
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

    int OnASyUpdataMysqlEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyQueryMysqlEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASySelectMysqlEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyDeleteMysqlEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyExistsMysqlEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyKeysMysqlEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyAddMysqlServerEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyKeepServerAliveEvent(const NFGUID& self, const int event, std::string& arg);

protected:
    virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance();

    NF_SHARE_PTR<NFIMysqlDriverManager> m_pMysqlDriverManager;
};


class NFCAsyMysqlModule
    : public NFIAsyMysqlModule
{
public:

    NFCAsyMysqlModule(NFIPluginManager* p);
    virtual ~NFCAsyMysqlModule();

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
    int ApplyRequest(NF_SHARE_PTR<SMysqlBaseParam> pParam, const int nEvetID);
    int RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);

    int OnUpDataMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnQueryMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnSelectMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnDeleteMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnExistsMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnUpKeyMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnAddMysqlServerAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
    int OnKeepServerAliveAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);

    int GetActor();
    bool KeepAliveMysqlServer();

    template<typename ClassParam>
    ClassParam* UnpackResult(const std::string& strMsgData);

private:
    static std::string strDefaultKey;
    static std::string strDefaultTable;

private:
    NFMapEx<int, int> mActorList; //actorid <-->Used
    NFMapEx<int, SMysqlBaseParam> mReqList;// reqID <-->Param
    int nCurReqID;
    int mnSuitIndex;
    NFINT64 mnLastCheckTime;

	NFIActorModule* m_pActorModule;
};

#endif