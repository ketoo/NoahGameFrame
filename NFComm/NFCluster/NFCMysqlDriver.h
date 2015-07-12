// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlDriver.h
//    @Author           :    eliteYang
//    @Date             :    2014-07-28
//    @Module           :    NFCMysqlDriver
//    @Desc             :
// -------------------------------------------------------------------------
#ifndef __NFC_MYSQL_DRIVER_MODULE_H__
#define __NFC_MYSQL_DRIVER_MODULE_H__

#include "NFIMysqlDriver.h"

#define  NFMYSQLTRYBEGIN try {

#define  NFMYSQLTRYEND(msg) }\
catch (mysqlpp::BadQuery er) \
{ \
    std::cout << "BadQuery [" << msg << "] Error: " << er.what() << std::endl; \
    return false; \
} \
catch (const mysqlpp::BadConversion& er)  \
{ \
    std::cout << "BadConversion [" << msg << "] Error:" << er.what() << " retrieved data size:" << er.retrieved << ", actual size:" << er.actual_size << std::endl; \
    return false; \
} \
catch (const mysqlpp::Exception& er) \
{ \
    std::cout << "mysqlpp::Exception [" << msg << "] Error:" << er.what() << std::endl; \
    return false; \
}\
catch ( ... ) \
{ \
    std::cout << "std::exception [" <<msg << "] Error:Unknown " << std::endl; \
    return false; \
}

#define  NFMYSQLTRYENDNULLRETURN(msg) }\
    catch (mysqlpp::BadQuery er) \
{ \
    std::cout << "BadQuery [" << msg << "] Error: " << er.what() << std::endl; \
    return ; \
} \
    catch (const mysqlpp::BadConversion& er)  \
{ \
    std::cout << "BadConversion [" << msg << "] Error:" << er.what() << " retrieved data size:" << er.retrieved << ", actual size:" << er.actual_size << std::endl; \
    return ; \
} \
    catch (const mysqlpp::Exception& er) \
{ \
    std::cout << "mysqlpp::Exception ["<<msg<<"] Error:" << er.what() << std::endl; \
    return ; \
}\
    catch ( ... ) \
{ \
    std::cout << "std::exception [" << msg << "] Error:Unknown " << std::endl; \
    return ; \
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class NFCMysqlDriver : public NFIMysqlDriver
{
public:
    NFCMysqlDriver(const int nReconnectTime = 60, const int nReconnectCount = -1)
    {
        mfCheckReconnect = 0.0f;
        mnDBPort        = 0;
        m_pMysqlConnect = NULL;
        mnReconnectTime = nReconnectTime;
        mnReconnectCount = nReconnectCount;
    }

    NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
    {
        mfCheckReconnect = 0.0f;
        mnDBPort        = 0;
        m_pMysqlConnect = NULL;

        mnReconnectTime = 60;
        mnReconnectCount = -1;

        Connect(strDBName, strDBHost, nDBPort, strDBUser, strDBPwd);
    }

    virtual ~NFCMysqlDriver()
    {
        CloseConnection();
    }

    virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
    {
        mstrDBName  = strDBName   ;
        mstrDBHost  = strDBHost   ;
        mnDBPort    = nDBPort     ;
        mstrDBUser  = strDBUser   ;
        mstrDBPwd   = strDBPwd    ;

        return Connect();
    }

    virtual  bool Execute(const float fLasFrametime, const float fStartedTime)
    {
        if (IsNeedReconnect() && CanReconnect(fLasFrametime))
        {
            Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);
        }

        return true;
    }

    virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult)
    {
        mysqlpp::Connection* pConection = GetConnection();
        if (pConection)
        {
            NFMYSQLTRYBEGIN
                mysqlpp::Query query = pConection->query(qstr);
                //query.execute();

                queryResult = query.store();
                query.reset();
                
            NFMYSQLTRYEND(qstr)
            return true;
        }

        return false;
    }

    virtual bool Query(const std::string& qstr)
    {
        mysqlpp::Connection* pConection = GetConnection();
        if (pConection)
        {
            NFMYSQLTRYBEGIN
                mysqlpp::Query query = pConection->query(qstr);
            query.execute();

            query.reset();

            NFMYSQLTRYEND(qstr)
                return true;
        }

        return false;
    }

    virtual mysqlpp::Connection* GetConnection()
    {
        return m_pMysqlConnect;
    }

    virtual void CloseConnection()
    {
        if (m_pMysqlConnect)
        {
            delete m_pMysqlConnect;
            m_pMysqlConnect = NULL;
        }
    }

    virtual bool Enable()
    {
        return !IsNeedReconnect();
    }

    virtual bool CanReconnect(const float fLasFrametime)
    {
        mfCheckReconnect += fLasFrametime;

        //30分钟检查断线重连
        if (mfCheckReconnect < mnReconnectTime)
        {
            return false;
        }

        if (mnReconnectCount == 0)
        {
            return false;
        }

        mfCheckReconnect = 0.0f;

        return true;
    }

    virtual bool Reconnect()
    {
        CloseConnection();
        Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);

        if (mnReconnectCount > 0)
        {
            mnReconnectCount--;
        }

        return true;
    }

protected:
    bool IsNeedReconnect()
    {
        //没有配置表
        if (mstrDBHost.length() < 1 || mstrDBUser.length() < 1)
        {
            return false;
        }

        if (NULL == m_pMysqlConnect)
        {
            return true;
        }

        if (!m_pMysqlConnect->connected())
        {
            CloseConnection();
            return true;
        }

        if (!m_pMysqlConnect->ping())
        {
            CloseConnection();
            return true;
        }

        return false;
    }

    bool Connect()
    {
        m_pMysqlConnect = new mysqlpp::Connection();
        if (NULL == m_pMysqlConnect)
        {
            return false;
        }
        NFMYSQLTRYBEGIN
            m_pMysqlConnect->set_option(new mysqlpp::MultiStatementsOption(true));
            m_pMysqlConnect->set_option(new mysqlpp::SetCharsetNameOption("utf8"));
            m_pMysqlConnect->set_option(new mysqlpp::ReconnectOption(true));
            if (!m_pMysqlConnect->connect(mstrDBName.c_str(), mstrDBHost.c_str(), mstrDBUser.c_str(), mstrDBPwd.c_str(), mnDBPort))
            {
                CloseConnection();
                // 连接失败
                return false;
            }

            // 设置超时时间为24小时
            mysqlpp::Query query = m_pMysqlConnect->query("set interactive_timeout = 24*3600");
            query.execute();
            query.reset();
        NFMYSQLTRYEND("Connect faild")
        return true;
    }  

private:
    std::string mstrDBName  ;
    std::string mstrDBHost  ; 
    int         mnDBPort    ;
    std::string mstrDBUser  ;
    std::string mstrDBPwd   ;

    mysqlpp::Connection* m_pMysqlConnect; // mysql连接对象
    float mfCheckReconnect;             //检查重连的时间

    int mnReconnectTime;
    int mnReconnectCount;
};

#endif // !__NFC_REDIS_DRIVER_MODULE_H__