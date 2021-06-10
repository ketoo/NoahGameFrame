/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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

#include <stdarg.h>
#include "NFLogModule.h"
#include "NFLogPlugin.h"
#include "termcolor.hpp"

#if NF_PLATFORM != NF_PLATFORM_WIN
#include "NFComm/NFCore/NFException.hpp"
#endif


std::string NFLogModule::GenerateFileName(const std::string &fileName)
{
	std::string finalFileName;

	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(4) << 1900 + ltm->tm_year << "-" << std::setw(2) << 1 + ltm->tm_mon << "-" << std::setw(2) << ltm->tm_mday;

	finalFileName.append(fileName);
	finalFileName.append(ss.str());

	return finalFileName;
}

NFLogModule::NFLogModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

bool NFLogModule::Awake()
{
	mnLogCountTotal = 0;
	auto fileName = GenerateFileName(pPluginManager->GetAppName());
	auto max_size = 1048576 * 100;//100M per file
	auto max_files = 10000;//100 files
	//try
	{
		std::string strLogConfigName = pPluginManager->GetLogConfigName();
		//auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
		logger = spdlog::rotating_logger_mt(pPluginManager->GetAppName(), "logs/" + GenerateFileName(fileName) + ".log", max_size, max_files);

		// Create a daily logger - a new file is created every day on 2:30am
		//auto logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 0, 0);

	}
	//catch (const spdlog::spdlog_ex &ex)
	{
		//std::cout << "Log init failed: " << ex.what() << std::endl;
	}

	spdlog::set_level(spdlog::level::debug);
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%l][thread %t]%v");
	//spdlog::set_pattern("[%H:%M:%S %z][%n][%L][thread %t] %v");
	spdlog::flush_every(std::chrono::seconds(2));
	spdlog::flush_on(spdlog::level::warn);
/*
	if (strLogConfigName.empty())
	{
		strLogConfigName = pPluginManager->GetAppName();
	}

	string strAppLogName = GetConfigPath(strLogConfigName);

	el::Configurations conf(strAppLogName);

	el::Configuration* globalConfiguration = conf.get(el::Level::Global, el::ConfigurationType::Filename);
	el::Configuration* debugConfiguration = conf.get(el::Level::Debug, el::ConfigurationType::Filename);
	el::Configuration* infoConfiguration = conf.get(el::Level::Info, el::ConfigurationType::Filename);
	el::Configuration* warningConfiguration = conf.get(el::Level::Warning, el::ConfigurationType::Filename);
	el::Configuration* errorConfiguration = conf.get(el::Level::Error, el::ConfigurationType::Filename);
	el::Configuration* fatalConfiguration = conf.get(el::Level::Fatal, el::ConfigurationType::Filename);

	globalConfiguration->setValue(GenerateFileName(globalConfiguration->value()));
	debugConfiguration->setValue(GenerateFileName(debugConfiguration->value()));
	infoConfiguration->setValue(GenerateFileName(infoConfiguration->value()));
	warningConfiguration->setValue(GenerateFileName(warningConfiguration->value()));
	errorConfiguration->setValue(GenerateFileName(errorConfiguration->value()));
	fatalConfiguration->setValue(GenerateFileName(fatalConfiguration->value()));

	std::cout << "LogConfig global: " << globalConfiguration->value() << std::endl;
	std::cout << "LogConfig debug: " << debugConfiguration->value() << std::endl;
	std::cout << "LogConfig info: " << infoConfiguration->value() << std::endl;
	std::cout << "LogConfig warning: " << warningConfiguration->value() << std::endl;
	std::cout << "LogConfig error: " << errorConfiguration->value() << std::endl;
	std::cout << "LogConfig fatal: " << fatalConfiguration->value() << std::endl;

	el::Loggers::reconfigureAllLoggers(conf);
	el::Helpers::installPreRollOutCallback(rolloutHandler);
*/
	return true;
}

bool NFLogModule::Init()
{
    return true;
}

bool NFLogModule::Shut()
{
   // el::Helpers::uninstallPreRollOutCallback();

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

	std::string localStream;
	localStream.append("[");
	localStream.append(std::to_string(mnLogCountTotal));
	localStream.append("][");
	localStream.append(std::to_string(pPluginManager->GetAppID()));
	localStream.append("]");

	char szBuffer[1024 * 10] = {0};
	try
	{
		va_list args;
		va_start(args, format);
		vsnprintf(szBuffer, sizeof(szBuffer) - 1, format, args);
		va_end(args);
	}
	catch(...)
	{
		//nll = NF_LOG_LEVEL::NLL_FATAL_NORMAL;
		localStream.append("szBuffer catch......");
	}

    localStream.append(szBuffer);

    if (mLogHooker)
    {
        mLogHooker.get()->operator()(nll, localStream);
    }

    switch (nll)
    {
        case NFILogModule::NLL_DEBUG_NORMAL:
			{
				std::cout << termcolor::green;
				logger->debug(localStream);
			}
			break;
        case NFILogModule::NLL_INFO_NORMAL:
			{
				std::cout << termcolor::green;
				logger->info(localStream);
			}
			break;
        case NFILogModule::NLL_WARING_NORMAL:
			{
				std::cout << termcolor::yellow;
				logger->warn(localStream);
			}
			break;
        case NFILogModule::NLL_ERROR_NORMAL:
			{
				std::cout << termcolor::red;
				logger->error(localStream);
				//LogStack();
			}
			break;
        case NFILogModule::NLL_FATAL_NORMAL:
			{
				std::cout << termcolor::red;
				logger->critical(localStream);
			}
			break;
        default:
			{
				std::cout << termcolor::green;
				logger->debug(localStream);
			}
			break;
    }

	std::cout<<termcolor::reset;

    return true;
}

void NFLogModule::LogStack()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
	NFException::CrashHandler(0);
#endif
}

bool NFLogModule::ChangeLogLevel(const NF_LOG_LEVEL lgoLevel)
{
	//spdlog::set_level(spdlog::level::debug);
	/*
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
    */
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
