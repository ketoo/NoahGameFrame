/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <stdarg.h>
#include "NFLogModule.h"
#include "NFLogPlugin.h"
#include "termcolor.hpp"
#include "NFComm/NFCore/easylogging++.h"

#if NF_PLATFORM != NF_PLATFORM_WIN
#include "NFComm/NFCore/NFException.hpp"
#endif

INITIALIZE_EASYLOGGINGPP

unsigned int NFLogModule::idx = 0;

bool NFLogModule::CheckLogFileExist(const char* filename)
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

void NFLogModule::rolloutHandler(const char* filename, std::size_t size)
{
    std::stringstream stream;
    if (!CheckLogFileExist(filename))
    {
        stream << filename << "." << idx;
        rename(filename, stream.str().c_str());
    }
}

std::string NFLogModule::GetConfigPath(const std::string & fileName)
{
	std::string strAppLogName;
#if NF_PLATFORM == NF_PLATFORM_WIN
#ifdef NF_DEBUG_MODE
	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Debug/logconfig/" + fileName + "_win.conf";
#else
	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Release/logconfig/" + fileName + "_win.conf";
#endif

#else
#ifdef NF_DEBUG_MODE
	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Debug/logconfig/" + fileName + ".conf";
#else
	strAppLogName = pPluginManager->GetConfigPath() + "NFDataCfg/Release/logconfig/" + fileName + ".conf";
#endif
#endif

	return strAppLogName;
}

NFLogModule::NFLogModule(NFIPluginManager* p)
{
    pPluginManager = p;

	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
}

bool NFLogModule::Awake()
{
	mnLogCountTotal = 0;

	std::string strLogConfigName = pPluginManager->GetLogConfigName();
	if (strLogConfigName.empty())
	{
		strLogConfigName = pPluginManager->GetAppName();
	}

	string strAppLogName = GetConfigPath(strLogConfigName);

	el::Configurations conf(strAppLogName);

	el::Configuration* pConfiguration = conf.get(el::Level::Debug, el::ConfigurationType::Filename);
	if (pConfiguration == nullptr)
	{
		conf = el::Configurations(GetConfigPath("Default"));
		pConfiguration = conf.get(el::Level::Debug, el::ConfigurationType::Filename);
	}

	const std::string& fileName = pConfiguration->value();
	pConfiguration->setValue(pPluginManager->GetConfigPath() + fileName);

	std::cout << "LogConfig: " << strAppLogName << std::endl;

	el::Loggers::reconfigureAllLoggers(conf);
	el::Helpers::installPreRollOutCallback(rolloutHandler);

	return true;
}

bool NFLogModule::Init()
{
	m_pKernelModule = this->pPluginManager->FindModule<NFIKernelModule>();

    return true;
}

bool NFLogModule::Shut()
{
    el::Helpers::uninstallPreRollOutCallback();

    return true;
}

bool NFLogModule::BeforeShut()
{
    return true;

}

bool NFLogModule::AfterInit()
{

    return true;
}

bool NFLogModule::Execute()
{
    return true;

}

bool NFLogModule::Log(const NF_LOG_LEVEL nll, const char* format, ...)
{
    mnLogCountTotal++;

    char szBuffer[1024 * 10] = {0};

    va_list args;
    va_start(args, format);
    vsnprintf(szBuffer, sizeof(szBuffer) - 1, format, args);
    va_end(args);

    mstrLocalStream.clear();

    mstrLocalStream.append(std::to_string(mnLogCountTotal));
    mstrLocalStream.append(" | ");
    mstrLocalStream.append(std::to_string(pPluginManager->GetAppID()));
    mstrLocalStream.append(" | ");
    mstrLocalStream.append(szBuffer);

    if (mLogHooker)
    {
        mLogHooker.get()->operator()(nll, mstrLocalStream);
    }

    switch (nll)
    {
        case NFILogModule::NLL_DEBUG_NORMAL:
			{
				std::cout << termcolor::green;
				LOG(DEBUG) << mstrLocalStream;
			}
			break;
        case NFILogModule::NLL_INFO_NORMAL:
			{
				std::cout << termcolor::green;
				LOG(INFO) << mstrLocalStream;
			}	
			break;
        case NFILogModule::NLL_WARING_NORMAL:
			{
				std::cout << termcolor::yellow;
				LOG(WARNING) << mstrLocalStream;
			}
			break;
        case NFILogModule::NLL_ERROR_NORMAL:
			{
				std::cout << termcolor::red;
				LOG(ERROR) << mstrLocalStream;
				//LogStack();
			}
			break;
        case NFILogModule::NLL_FATAL_NORMAL:
			{
				std::cout << termcolor::red;
				LOG(FATAL) << mstrLocalStream;
			}
			break;
        default:
			{
				std::cout << termcolor::green;
				LOG(INFO) << mstrLocalStream;
			}
			break;
    }

	std::cout<<termcolor::reset;

    return true;
}

bool NFLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& recordName, const std::string& strDesc, const char* func, int line)
{
	std::ostringstream os;
	auto record = m_pKernelModule->FindRecord(ident, recordName);
	if (record)
	{

		if (line > 0)
		{
			Log(nll, "[RECORD] Indent[%s] Record[%s] %s %s %d", ident.ToString().c_str(), recordName.c_str(), record->ToString().c_str(), func, line);
		}
		else
		{
			Log(nll, "[RECORD] Indent[%s] Record[%s] %s", ident.ToString().c_str(), recordName.c_str(), record->ToString().c_str());
		}
	}


    return true;
}

bool NFLogModule::LogObject(const NF_LOG_LEVEL nll, const NFGUID ident, const std::string& strDesc, const char* func, int line)
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

void NFLogModule::LogStack()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
	NFException::CrashHandler(0);
#endif
}
bool NFLogModule::LogDebugFunctionDump(const NFGUID ident, const int nMsg, const std::string& strArg,  const char* func /*= ""*/, const int line /*= 0*/)
{
    //#ifdef NF_DEBUG_MODE
    LogDebug(ident, strArg + "MsgID:" + std::to_string(nMsg), func, line);
    //#endif
    return true;
}

bool NFLogModule::ChangeLogLevel(const std::string& strLevel)
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
    LogInfo("[Log] Change log level as " + strLevel, __FUNCTION__, __LINE__);
    return true;
}

bool NFLogModule::LogDebug(const std::string& strLog, const char* func , int line)
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

bool NFLogModule::LogInfo(const std::string& strLog, const  char* func, int line)
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

bool NFLogModule::LogWarning(const std::string& strLog, const char* func , int line)
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

bool NFLogModule::LogError(const std::string& strLog, const char* func , int line)
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

bool NFLogModule::LogFatal(const std::string& strLog, const char* func , int line)
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


bool NFLogModule::LogDebug(const std::ostringstream& stream, const char* func , int line)
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

bool NFLogModule::LogInfo(const std::ostringstream& stream, const  char* func, int line)
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

bool NFLogModule::LogWarning(const std::ostringstream& stream, const char* func , int line)
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

bool NFLogModule::LogError(const std::ostringstream& stream, const char* func , int line)
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

bool NFLogModule::LogFatal(const std::ostringstream& stream, const char* func , int line)
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


bool NFLogModule::LogDebug(const NFGUID ident, const std::string& strLog, const char* func , int line)
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

bool NFLogModule::LogInfo(const NFGUID ident, const std::string& strLog, const  char* func, int line)
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

bool NFLogModule::LogWarning(const NFGUID ident, const std::string& strLog, const char* func , int line)
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

bool NFLogModule::LogError(const NFGUID ident, const std::string& strLog, const char* func , int line)
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

bool NFLogModule::LogFatal(const NFGUID ident, const std::string& strLog, const char* func , int line)
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


bool NFLogModule::LogDebug(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
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

bool NFLogModule::LogInfo(const NFGUID ident, const std::ostringstream& stream, const  char* func, int line)
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

bool NFLogModule::LogWarning(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
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

bool NFLogModule::LogError(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
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

bool NFLogModule::LogFatal(const NFGUID ident, const std::ostringstream& stream, const char* func , int line)
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

void NFLogModule::SetHooker(LOG_HOOKER_FUNCTOR_PTR hooker)
{
    mLogHooker = hooker;
}