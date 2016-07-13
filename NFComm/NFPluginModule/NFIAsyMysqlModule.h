// -------------------------------------------------------------------------
//    @FileName      	:    NFIAsyMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIAsyMysqlModule
//
//
// -------------------------------------------------------------------------

#ifndef NFI_ASY_MYSQL_MODULE_H
#define NFI_ASY_MYSQL_MODULE_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

typedef std::function<void(const NFGUID& self, const int nRet, const std::string& strUseData)> MYSQL_RETURN_FUNCTOR;
typedef std::function<void(const NFGUID& self, const int nRet, const int nValue, const std::string& strUseData)> MYSQL_RETURN_INT_FUNCTOR;
typedef std::function<void(const NFGUID& self, const int nRet, const std::vector<std::string>& valueVec, const std::string& strUseData)> MYSQL_RETURN_VECVALUE_FUNCTOR;
typedef std::function<void(const NFGUID& self, const int nRet, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const std::string& strUseData)> MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR;

struct SMysqlParam;

class NFIAsyMysqlModule
    : public NFIModule
{
public:
    enum ACOTERMYSQLEVENT
    {
        ACOTERMYSQLEVENT_INISERVER = 2,
        ACOTERMYSQLEVENT_KEEPALIVESERVER = 3,
        ACOTERMYSQLEVENT_UPDATA = 4,
        ACOTERMYSQLEVENT_QUERY = 5,
        ACOTERMYSQLEVENT_SELECT = 6,
        ACOTERMYSQLEVENT_DELETE = 7,
        ACOTERMYSQLEVENT_EXISTS = 8,
        ACOTERMYSQLEVENT_KEYS = 9,
    };

public:
    template<typename BaseType>
    int Updata(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec,
               BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::string&),  const std::string& strUseData)
    {
        const MYSQL_RETURN_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        return Updata(self, strRecordName, strKey, fieldVec, valueVec, FunReturnRsp, strUseData);
    }

    template<typename BaseType>
    int Query(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec,
              BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::vector<std::string>&, const std::vector<std::string>&, const std::string&),  const std::string& strUseData)
    {
        const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR mFunReturnVeckKeyValueRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

        return Query(self, strRecordName, strKey, fieldVec, mFunReturnVeckKeyValueRsp, strUseData);
    }

    template<typename BaseType>
    int Delete(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec,
               BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::string&),  const std::string& strUseData)
    {
        const MYSQL_RETURN_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        return Delete(self, strRecordName, strKey, FunReturnRsp, strUseData);
    }

    template<typename BaseType>
    int Exists(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec,
               BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const int, const std::string&),  const std::string& strUseData)
    {
        const MYSQL_RETURN_INT_FUNCTOR mFunReturnIntRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        return Exists(self, strRecordName, strKey, mFunReturnIntRsp, strUseData);
    }

    template<typename BaseType>
    int Keys(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec,
             BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::vector<std::string>&, const std::string&),  const std::string& strUseData)
    {
        const MYSQL_RETURN_VECVALUE_FUNCTOR mFunReturnVecValueRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        return Keys(self, strRecordName, strKey, mFunReturnVecValueRsp, strUseData);
    }

    virtual bool CloseActorPool() = 0;
    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

protected:
	virtual bool StartActorPool(const int nCount) = 0;
    virtual bool Updata(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData) = 0;
    virtual bool Query(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData) = 0;

    virtual bool Delete(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData) = 0;
    virtual bool Exists(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_INT_FUNCTOR& mFunReturnIntRsp, const std::string& strUseData) = 0;
    virtual bool Keys(const NFGUID& self, const std::string& strRecordName, const std::string& strKeyName, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnVecValueRsp, const std::string& strUseData) = 0;

};

#endif
