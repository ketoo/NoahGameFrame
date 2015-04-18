// -------------------------------------------------------------------------
//    @FileName      :    NFCLogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_LOG_MODULE_H_
#define _NFC_LOG_MODULE_H_

//#include <strsafe.h>
#include "NFComm/NFPluginModule/NFILogModule.h"
//#include "NFComm/NFPluginModule/NFIConcurrentModule.h"

// template <typename ValueType>
// class NFCLogActor : public NFIActor<ValueType>
// {
// public:
//
//  NFCLogActor<ValueType>(Theron::Framework &framework) : NFIActor<ValueType>(framework)
//  {
//      RegisterHandler(this, &NFCLogActor::Handler);
//
//
//
//  }
//
//      virtual void Handler(const ValueType& message, const Theron::Address from)
//      {
//          int nID = GetCurrentThreadId();
//          std::cout << "NFCLogActorID:" << nID << std::endl;
//
//          Send(message, from);
//      }
//
// private:
// };
//
// template <typename ValueType>
// class NFCLogReceiver : public NFIReceiver<ValueType>
// {
// public:
//     NFCLogReceiver()
//     {
//         // Register the message handler.
//         RegisterHandler(this, &NFCLogReceiver::Recive);
//     }
//
//     virtual void Recive(const ValueType& message, const Theron::Address from)
//     {
//      //做完了，自己投递结果消息到主线�?
//         int nID = GetCurrentThreadId();
//         std::cout << "ReciveID:" << nID << std::endl;
//     }
// };

class NFCLogModule
    : public NFILogModule
{
public:

    NFCLogModule(NFIPluginManager* p);
    virtual ~NFCLogModule() {}

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    ///////////////////////////////////////////////////////////////////////
    virtual void LogStack();

    virtual bool LogElement(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strElement, const std::string& strDesc, const char* func = "", int line = 0);
    virtual bool LogProperty(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strProperty, const std::string& strDesc, const char* func = "", int line = 0);
    virtual bool LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol, const char* func = "", int line = 0);
    virtual bool LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc, const char* func = "", int line = 0);
    virtual bool LogObject(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strDesc, const char* func = "", int line = 0);

    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const char* func = "", int line = 0);
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const int nDesc, const char* func = "", int line = 0);
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const std::string& strDesc, const char* func = "", int line = 0);
    virtual bool LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::ostringstream& stream, const char* func = "", int line = 0);

	virtual bool LogDebugFunctionDump(const NFIDENTID ident, const int nMsg, const std::string& strArg, const char* func = "", const int line = 0);
    virtual bool ChangeLogLevel(const std::string& strLevel);
protected:
    friend class NFCKernelModule;

    virtual bool Log(const NF_LOG_LEVEL nll, const char* format, ...);

    static bool CheckLogFileExist(const char* filename);
    static void rolloutHandler(const char* filename, std::size_t size);

    //NFIConcurrentModule* m_pConcurrentModule;
    //NFIActor* m_pActor;
    //NFIActor<std::string>* m_pActor;
    //NFIActor<std::string>* m_pActor1;
    //NFIActor<std::string>* m_pActor2;
    //NFIActor<std::string>* m_pActor3;
    //NFIReceiver<std::string>* m_pReceiver;

private:
    static unsigned int idx;
};

#endif
