// -------------------------------------------------------------------------
//    @FileName      :    NFCLogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_LOG_MODULE_H_
#define _NFC_LOG_MODULE_H_

#include "NFComm/NFPluginModule/NFICrashModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include "google-breakpad/src/client/linux/handler/exception_handler.h"
#elif NF_PLATFORM == NF_PLATFORM_WIN
#include "google-breakpad/src/client/windows/handler/exception_handler.h"
#elif NF_PLATFORM == NF_PLATFORM_APPLE
#include "google-breakpad/src/client/mac/handler/exception_handler.h"
#endif

class NFCCrashModule : public NFICrashModule
{
public:
    NFCCrashModule(NFIPluginManager* p);
    virtual ~NFCCrashModule() {}

    virtual bool Init();
    virtual bool Shut();
    virtual bool BeforeShut();
    virtual bool AfterInit();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual void TestCrash();
protected:

#if NF_PLATFORM == NF_PLATFORM_WIN
    static bool DumpCallback();
#else
    static bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded);
    google_breakpad::MinidumpDescriptor* m_pDumpDescriptor;
    google_breakpad::ExceptionHandler* m_pExceptionHandler;
#endif

};

#endif
