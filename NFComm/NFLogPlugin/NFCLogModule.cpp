// -------------------------------------------------------------------------
//    @FileName      :    NFCLogModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "NFComm/NFPluginModule/NFIActorManager.h"
#include "NFComm/NFPluginModule/NFIActor.h"
//#include "NFStackWalker.h"
#include "NFCLogModule.h"
#include <boost/filesystem.hpp>
#include <stdarg.h>
#include "easylog/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

unsigned int NFCLogModule::idx = 0;

bool NFCLogModule::CheckLogFileExist(const char* filename)
{
    std::stringstream stream;
    stream << filename << "." << ++idx;
    if (boost::filesystem::exists(stream.str()))
    {
        return CheckLogFileExist(filename);
    }

    return false;
}

void NFCLogModule::rolloutHandler(const char* filename, std::size_t size)
{
    std::stringstream stream;
    if (!CheckLogFileExist(filename))
    {
        stream << filename << "." << idx;
        boost::filesystem::rename(filename, stream.str().c_str());
    }
}

NFCLogModule::NFCLogModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

bool NFCLogModule::Init()
{

// #ifdef NF_USE_ACTOR
//     NFIActor* pActor = (NFIActor*)(pPluginManager);
//     if (pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
// #endif
    {
        el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
        el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
#if NF_PLATFORM == NF_PLATFORM_WIN
        el::Configurations conf("log_win.conf");
#else
        el::Configurations conf("log.conf");
#endif
        el::Loggers::reconfigureAllLoggers(conf);
        el::Helpers::installPreRollOutCallback(rolloutHandler);
    }

    return true;
}

bool NFCLogModule::Shut()
{
#ifdef NF_USE_ACTOR
    //NFIActor* pActor = (NFIActor*)(pPluginManager);
    //if (pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
    {
        el::Helpers::uninstallPreRollOutCallback();
    }
#else
    el::Helpers::uninstallPreRollOutCallback();
#endif

    return true;
}

bool NFCLogModule::BeforeShut()
{
    return true;

}

bool NFCLogModule::AfterInit()
{
    return true;

}

bool NFCLogModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;

}

bool NFCLogModule::Log(const NF_LOG_LEVEL nll, const char* format, ...)
{
    char szBuffer[1024 * 10] = {0};

    va_list args;
    va_start(args, format);
    vsnprintf(szBuffer, sizeof(szBuffer) - 1, format, args);
    va_end(args);

#ifdef NF_USE_ACTOR
// #ifdef NF_DYNAMIC_PLUGIN
// 	NFIActor* pActor = (NFIActor*)(pPluginManager);
// 	if (!pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
// 	{
// 		NFIActorMessage message;
// 		message.eType = NFIActorMessage::EACTOR_LOG_MSG;
// 		message.data = szBuffer;
// 
// 		const Theron::Address address = pActor->GetActorManager()->GetAddress(NFIActorManager::EACTOR_MAIN);
// 		if (address != Theron::Address::Null())
// 		{
// 			pActor->GetFramework().Send(message, pActor->GetAddress(), address);
// 		}
// 
// 		return true;
// 	}
// #endif
#endif


    switch (nll)
    {
    case NFILogModule::NLL_DEBUG_NORMAL:
        LOG(DEBUG) << szBuffer;
        break;
    case NFILogModule::NLL_INFO_NORMAL:
        LOG(INFO) << szBuffer;
        break;
    case NFILogModule::NLL_WARING_NORMAL:
        LOG(WARNING) << szBuffer;
        break;
    case NFILogModule::NLL_ERROR_NORMAL:
        {
            LOG(ERROR) << szBuffer;
            //LogStack();
        }
        break;
    case NFILogModule::NLL_FATAL_NORMAL:
        LOG(FATAL) << szBuffer;
        break;
    default:
        LOG(INFO) << szBuffer;
        break;
    }

    return true;
}

bool NFCLogModule::LogElement(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strElement, const std::string& strDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "[ELEMENT] Indent[%s] Element[%s] %s %s %d", ident.ToString().c_str(), strElement.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[ELEMENT] Indent[%s] Element[%s] %s", ident.ToString().c_str(), strElement.c_str(), strDesc.c_str());
    }

    return true;
}

bool NFCLogModule::LogProperty(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strProperty, const std::string& strDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "[PROPERTY] Indent[%s] Property[%s] %s %s %d", ident.ToString().c_str(), strProperty.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[PROPERTY] Indent[%s] Property[%s] %s", ident.ToString().c_str(), strProperty.c_str(), strDesc.c_str());
    }

    return true;

}

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "[RECORD] Indent[%s] Record[%s] Row[%d] Col[%d] %s %s %d", ident.ToString().c_str(), strRecord.c_str(), nRow, nCol, strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[RECORD] Indent[%s] Record[%s] Row[%d] Col[%d] %s", ident.ToString().c_str(), strRecord.c_str(), nRow, nCol, strDesc.c_str());
    }

    return true;

}

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "[RECORD] Indent[%s] Record[%s] %s %s %d", ident.ToString().c_str(), strRecord.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[RECORD] Indent[%s] Record[%s] %s", ident.ToString().c_str(), strRecord.c_str(), strDesc.c_str());
    }

    return true;
}

bool NFCLogModule::LogObject(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "[OBJECT] Indent[%s] %s %s %d", ident.ToString().c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[OBJECT] Indent[%s] %s", ident.ToString().c_str(), strDesc.c_str());
    }

    return true;

}

void NFCLogModule::LogStack()
{

    //To Add
    /*
    #ifdef NF_DEBUG_MODE
    time_t t = time(0);
    char szDmupName[MAX_PATH];
    tm* ptm = localtime(&t);

    sprintf(szDmupName, "%d_%d_%d_%d_%d_%d.dmp",  ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    // 创建Dump文件
    HANDLE hDumpFile = CreateFile(szDmupName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Dump信息
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    //dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    // 写入Dump文件内容
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);

    #endif
    */
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const std::string& strDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "Indent[%s] %s %s %s %d", ident.ToString().c_str(), strInfo.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "Indent[%s] %s %s", ident.ToString().c_str(), strInfo.c_str(), strDesc.c_str());
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const int nDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "Indent[%s] %s %d %s %d", ident.ToString().c_str(), strInfo.c_str(), nDesc, func, line);
    }
    else
    {
        Log(nll, "Indent[%s] %s %d", ident.ToString().c_str(), strInfo.c_str(), nDesc);
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::ostringstream& stream, const char* func, int line)
{
    switch (nll)
    {
    case NFILogModule::NLL_DEBUG_NORMAL:
        {
            if (0 == line)
            {
                LOG(DEBUG) << "Indent[" << ident.ToString() << "] " << stream.str();
            }
            else
            {
                LOG(DEBUG) << "Indent[" << ident.ToString() << "] " << stream.str() << " " << func << " " << line;
            }
        }
        break;
    case NFILogModule::NLL_INFO_NORMAL:
        {
            if (0 == line)
            {
                LOG(INFO) << "Indent[" << ident.ToString() << "] " << stream.str();
            }
            else
            {
                LOG(INFO) << "Indent[" << ident.ToString() << "] " << stream.str() << " " << func << " " << line;
            }
        }
        break;
    case NFILogModule::NLL_WARING_NORMAL:
        {
            if (0 == line)
            {
                LOG(WARNING) << "Indent[" << ident.ToString() << "] " << stream.str();
            }
            else
            {
                LOG(WARNING) << "Indent[" << ident.ToString() << "] " << stream.str() << " " << func << " " << line;
            }
        }
        break;
    case NFILogModule::NLL_ERROR_NORMAL:
        {
            if (0 == line)
            {
                LOG(ERROR) << "Indent[" << ident.ToString() << "] " << stream.str();
            }
            else
            {
                LOG(ERROR) << "Indent[" << ident.ToString() << "] " << stream.str() << " " << func << " " << line;
            }
        }
        break;
    default:
        break;
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const char* func /*= ""*/, int line /*= 0*/)
{
    if (line > 0)
    {
        Log(nll, "Indent[%s] %s %s %d", ident.ToString().c_str(), strInfo.c_str(), func, line);
    }
    else
    {
        Log(nll, "Indent[%s] %s", ident.ToString().c_str(), strInfo.c_str());
    }

    return true;
}

bool NFCLogModule::LogDebugFunctionDump(const NFIDENTID ident, const int nMsg, const std::string& strArg,  const char* func /*= ""*/, const int line /*= 0*/ )
{
    //#ifdef NF_DEBUG_MODE
    LogNormal(NFILogModule::NLL_WARING_NORMAL, ident, strArg + "MsgID:", nMsg, func, line);
    //#endif
    return true;
}

bool NFCLogModule::ChangeLogLevel(const std::string& strLevel)
{
    el::Level logLevel = el::LevelHelper::convertFromString(strLevel.c_str());
    el::Logger* pLogger = el::Loggers::getLogger("default");
    if (NULL == pLogger)
    {
        return false;
    }

    el::Configurations* pConfigurations = pLogger->configurations();
    el::base::TypedConfigurations* pTypeConfigurations = pLogger->typedConfigurations();
    if (NULL == pConfigurations)
    {
        return false;
    }

    // log级别为debug, info, warning, error, fatal(级别逐渐提高)
    // 当传入为info时，则高于(包含)info的级别会输出
    // !!!!!! NOTICE:故意没有break，请千万注意 !!!!!!
    switch (logLevel)
    {
    case el::Level::Fatal:
        {
            el::Configuration errorConfiguration(el::Level::Error, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&errorConfiguration);
        }
    case el::Level::Error:
        {
            el::Configuration warnConfiguration(el::Level::Warning, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&warnConfiguration);
        }
    case el::Level::Warning:
        {
            el::Configuration infoConfiguration(el::Level::Info, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&infoConfiguration);
        }
    case el::Level::Info:
        {
            el::Configuration debugConfiguration(el::Level::Debug, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&debugConfiguration);
            
        }
    case el::Level::Debug:
        break;
    default:
        break;
    }

    el::Loggers::reconfigureAllLoggers(*pConfigurations);
    LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "[Log] Change log level", strLevel, __FUNCTION__, __LINE__);
    return true;
}
