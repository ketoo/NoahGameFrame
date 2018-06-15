// -------------------------------------------------------------------------
//    @FileName			:    NFCLogModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <stdarg.h>
#include "NFCLogModule.h"
#include "easylogging++.h"
#include "NFLogPlugin.h"
#include "termcolor.hpp"

INITIALIZE_EASYLOGGINGPP

unsigned int NFCLogModule::idx = 0;

bool NFCLogModule::CheckLogFileExist(const char* filename)
{
    std::stringstream stream;
    stream << filename << "." << ++idx;
    std::fstream file;
    file.open(stream.str(), std::ios::in);
    if (file)
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
        rename(filename, stream.str().c_str());
    }
}

void NFCLogModule::copyConfFile(const char *filename)
{
    std::fstream file;
    std::ifstream fileMaster;
    std::string filenamepathMaster,fnamePath;
    fnamePath = getLogConfigFileName(filename);
    file.open(fnamePath, std::ios::in);
    if (!file.is_open()) {
        
        filenamepathMaster = getLogConfigFileName("MasterServer");
        fileMaster.open(filenamepathMaster, std::ios::in);
        fileMaster.seekg(0,std::ios::end);
        int32_t size=fileMaster.tellg();
        fileMaster.seekg(0,std::ios::beg);
        if (size <= 0)
            return;
        char *buf;
        buf = new char[size+1];
        memset(buf, 0, size + 1);
        fileMaster.read(buf, size);
        std::string conf = buf;
        delete []buf;
        int32_t pos;
        while (std::string::npos != (pos=conf.find("master_server")))
        {
            conf.replace(pos, 13, filename);
        }
        std::ofstream ofile;
        ofile.open(fnamePath, ios::trunc);
        ofile.write(conf.c_str(), conf.size());
        ofile.close();
        
    }

}

NFCLogModule::NFCLogModule(NFIPluginManager* p)
{
    pPluginManager = p;
}
string NFCLogModule::getLogConfigFileName(std::string aName)
{
    string strAppLogName = "";

#if NF_PLATFORM == NF_PLATFORM_WIN
#ifdef NF_DEBUG_MODE
    strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Debug/logconfig/" + aName + "_win.conf";
#else
    strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Release/logconfig/" + strLogConfigName + "_win.conf";
#endif

#else
#ifdef NF_DEBUG_MODE
    strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Debug/logconfig/" + strLogConfigName + ".conf";
#else
    strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Release/logconfig/" + strLogConfigName + ".conf";
#endif
#endif
    return strAppLogName;

}
bool NFCLogModule::Awake()
{
	mnLogCountTotal = 0;

	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);

	std::string strLogConfigName = pPluginManager->GetLogConfigName();
	if (strLogConfigName.empty())
	{
		strLogConfigName = pPluginManager->GetAppName();
	}
    copyConfFile(strLogConfigName.c_str());
    string strAppLogName = "";
    strAppLogName=getLogConfigFileName(strLogConfigName);
//#if NF_PLATFORM == NF_PLATFORM_WIN
//#ifdef NF_DEBUG_MODE
//	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Debug/logconfig/" + strLogConfigName + "_win.conf";
//#else
//	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Release/logconfig/" + strLogConfigName + "_win.conf";
//#endif
//
//#else
//#ifdef NF_DEBUG_MODE
//	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Debug/logconfig/" + strLogConfigName + ".conf";
//#else
//	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Release/logconfig/" + strLogConfigName + ".conf";
//#endif
//#endif

	el::Configurations conf(strAppLogName);

	el::Configuration* pConfiguration = conf.get(el::Level::Debug, el::ConfigurationType::Filename);
	const std::string& strFileName = pConfiguration->value();
	pConfiguration->setValue(pPluginManager->GetConfigPath() + strFileName);

	std::cout << "LogConfig: " << strAppLogName << std::endl;

	el::Loggers::reconfigureAllLoggers(conf);
	el::Helpers::installPreRollOutCallback(rolloutHandler);

	return true;
}

bool NFCLogModule::Init()
{
   
    return true;
}

bool NFCLogModule::Shut()
{
    el::Helpers::uninstallPreRollOutCallback();

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

bool NFCLogModule::Execute()
{
    return true;

}

bool NFCLogModule::Log(const NF_LOG_LEVEL nll, const char* format, ...)
{
    mnLogCountTotal++;

    char szBuffer[1024 * 10] = {0};

    va_list args;
    va_start(args, format);
    vsnprintf(szBuffer, sizeof(szBuffer) - 1, format, args);
    va_end(args);

    switch (nll)
    {
        case NFILogModule::NLL_DEBUG_NORMAL:
			{
				std::cout << termcolor::green;
				LOG(DEBUG) << mnLogCountTotal << " | " << pPluginManager->GetAppID()<< " | " << szBuffer;
			}
			break;
        case NFILogModule::NLL_INFO_NORMAL:
			{
				std::cout << termcolor::green;
				LOG(INFO) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << szBuffer;
			}	
			break;
        case NFILogModule::NLL_WARING_NORMAL:
			{
				std::cout << termcolor::yellow;
				LOG(WARNING) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << szBuffer;
			}
			break;
        case NFILogModule::NLL_ERROR_NORMAL:
			{
				std::cout << termcolor::red;
				LOG(ERROR) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << szBuffer;
				//LogStack();
			}
			break;
        case NFILogModule::NLL_FATAL_NORMAL:
			{
				std::cout << termcolor::red;
				LOG(FATAL) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << szBuffer;
			}
			break;
        default:
			{
				std::cout << termcolor::green;
				LOG(INFO) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << szBuffer;
			}
			break;
    }

	std::cout<<termcolor::reset;

    return true;
}

bool NFCLogModule::LogElement(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strElement, const std::string& strDesc, const char* func, int line)
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

bool NFCLogModule::LogProperty(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strProperty, const std::string& strDesc, const char* func, int line)
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

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol, const char* func, int line)
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

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strRecord, const std::string& strDesc, const char* func, int line)
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

bool NFCLogModule::LogObject(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strDesc, const char* func, int line)
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
    
    HANDLE hDumpFile = CreateFile(szDmupName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    //dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);

    #endif
    */
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strInfo, const std::string& strDesc, const char* func, int line)
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

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strInfo, const int64_t nDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "Indent[%s] %s %lld %s %d", ident.ToString().c_str(), strInfo.c_str(), nDesc, func, line);
    }
    else
    {
        Log(nll, "Indent[%s] %s %lld", ident.ToString().c_str(), strInfo.c_str(), nDesc);
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::ostringstream& stream, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "Indent[%s] %s %s %d", ident.ToString().c_str(), stream.str().c_str(), func, line);
    }
    else
    {
        Log(nll, "Indent[%s] %s", ident.ToString().c_str(), stream.str().c_str());
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strInfo, const char* func /*= ""*/, int line /*= 0*/)
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

bool NFCLogModule::LogDebugFunctionDump(const NFGUID ident, const int nMsg, const std::string& strArg,  const char* func /*= ""*/, const int line /*= 0*/)
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
    if (NULL == pConfigurations)
    {
        return false;
    }

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
    LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "[Log] Change log level", strLevel, __FUNCTION__, __LINE__);
    return true;
}

bool NFCLogModule::LogDebug(const std::string& strLog, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "%s %s %d", strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "%s", strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogInfo(const std::string& strLog, const  char* func, int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "%s %s %d", strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "%s", strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogWarning(const std::string& strLog, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "%s %s %d", strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "%s", strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogError(const std::string& strLog, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "%s %s %d", strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "%s", strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogFatal(const std::string& strLog, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "%s %s %d", strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "%s", strLog.c_str());
     }

     return true;
}


bool NFCLogModule::LogDebug(const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "%s %s %d", stream.str().c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "%s", stream.str().c_str());
     }

     return true;
}

bool NFCLogModule::LogInfo(const std::ostringstream& stream, const  char* func, int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "%s %s %d", stream.str().c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "%s", stream.str().c_str());
     }

     return true;
}

bool NFCLogModule::LogWarning(const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "%s %s %d", stream.str().c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "%s", stream.str().c_str());
     }

     return true;
}

bool NFCLogModule::LogError(const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "%s %s %d", stream.str().c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "%s", stream.str().c_str());
     }

     return true;
}

bool NFCLogModule::LogFatal(const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "%s %s %d", stream.str().c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "%s", stream.str().c_str());
     }

     return true;
}


bool NFCLogModule::LogDebug(const NFGUID ident, const std::string& strLog, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogInfo(const NFGUID ident, const std::string& strLog, const  char* func, int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogWarning(const NFGUID ident, const std::string& strLog, const char* func , int line)
{
    if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogError(const NFGUID ident, const std::string& strLog, const char* func , int line)
{
     if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), strLog.c_str());
     }

     return true;
}

bool NFCLogModule::LogFatal(const NFGUID ident, const std::string& strLog, const char* func , int line)
{
     if (line > 0)
     {
         Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), strLog.c_str(), func, line);
     }
     else
     {
         Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), strLog.c_str());
     }

     return true;
}


bool NFCLogModule::LogDebug(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
    {
        Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), stream.str().c_str(), func, line);
    }
    else
    {
        Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), stream.str().c_str());
    }

    return true;
}

bool NFCLogModule::LogInfo(const NFGUID ident, const std::ostringstream& stream, const  char* func, int line)
{
    if (line > 0)
    {
        Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), stream.str().c_str(), func, line);
    }
    else
    {
        Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), stream.str().c_str());
    }

    return true;
}

bool NFCLogModule::LogWarning(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
    {
        Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), stream.str().c_str(), func, line);
    }
    else
    {
        Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), stream.str().c_str());
    }

    return true;
}

bool NFCLogModule::LogError(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
    {
        Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), stream.str().c_str(), func, line);
    }
    else
    {
        Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), stream.str().c_str());
    }

    return true;
}

bool NFCLogModule::LogFatal(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
{
    if (line > 0)
    {
        Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "Indent[%s] %s %s %d", ident.ToString().c_str(), stream.str().c_str(), func, line);
    }
    else
    {
        Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, "Indent[%s] %s", ident.ToString().c_str(), stream.str().c_str());
    }

    return true;
}
