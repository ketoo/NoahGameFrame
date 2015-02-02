#include "NFCCrashModule.h"
#include "boost/filesystem.hpp"

NFCCrashModule::NFCCrashModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

bool NFCCrashModule::Init()
{
    return true;
}

bool NFCCrashModule::Shut()
{
    return true;
}

bool NFCCrashModule::BeforeShut()
{
    return true;
}

bool NFCCrashModule::AfterInit()
{
    // start crash handler
#if NF_PLATFORM == NF_PLATFORM_WIN
    // TODO:因为google-breakpad要求unicode，所以win下multi-bytes程序无法使用
    std::cout << "google-breakpad is not suitable for multibytes application in windows!" << std::endl;
#else
    //m_pDumpDescriptor = NF_SHARE_PTR<google_breakpad::MinidumpDescriptor>(NF_NEW google_breakpad::MinidumpDescriptor("./dump"));
    //m_pExceptionHandler = NF_SHARE_PTR<google_breakpad::ExceptionHandler>(NF_NEW google_breakpad::ExceptionHandler(*m_pDumpDescriptor, NULL, DumpCallback, NULL, true, -1));
#endif
    return true;
}

bool NFCCrashModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

void NFCCrashModule::TestCrash()
{
    volatile int* pInt = (int*)(NULL);
    *pInt = 1;
}

#if NF_PLATFORM == NF_PLATFORM_WIN

#else
bool NFCCrashModule::DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded)
{
    printf("Dump path: %s \n", descriptor.path());
    return succeeded;
}
#endif
