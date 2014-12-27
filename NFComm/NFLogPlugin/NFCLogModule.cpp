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
#include "glog/logging.h"
#include <time.h>
//#include "NFStackWalker.h"
#include "NFCLogModule.h"
#include <boost/filesystem.hpp>
#include <stdarg.h>

NFCLogModule::NFCLogModule(NFIPluginManager* p)
{
    pPluginManager = p;

    //m_pFd = NULL;
}

bool NFCLogModule::Init()
{

#ifdef NF_USE_ACTOR
    NFIActor* pActor = (NFIActor*)(pPluginManager);
    if (pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
#endif
    {
        char szName[MAX_PATH] = {0};
        google::InitGoogleLogging(szName);

        std::string strPath = std::string("./log/");
        if (!boost::filesystem::exists(strPath))
        {
            boost::filesystem::create_directories(strPath);
        }

#ifdef NF_DEBUG_MODE
        google::SetStderrLogging(google::GLOG_INFO); //设置级别高于 google::INFO 的日志同时输出到屏幕
#else
        google::SetStderrLogging(google::GLOG_ERROR);//设置级别高于 google::ERROR 的日志同时输出到屏幕
#endif

        FLAGS_colorlogtostderr = true;    //设置输出到屏幕的日志显示相应颜色
        FLAGS_servitysinglelog = true;

        google::SetLogDestination(google::GLOG_FATAL, std::string(strPath + "/log_fatal_").c_str());
        google::SetLogDestination(google::GLOG_ERROR, std::string(strPath + "/log_error_").c_str());      //设置 google::error 级别的日志存储路径和文件名前缀
        google::SetLogDestination(google::GLOG_WARNING, std::string(strPath + "/log_warning_").c_str());  //设置 google::WARNING 级别的日志存储路径和文件名前缀
        google::SetLogDestination(google::GLOG_INFO, std::string(strPath + "/log_Info_").c_str());        //设置 google::INFO 级别的日志存储路径和文件名前缀

        FLAGS_logbufsecs = 0;                       //缓冲日志输出，默认为30秒，此处改为立即输出
        FLAGS_max_log_size = 100;                   //最大日志大小为 100MB
        FLAGS_stop_logging_if_full_disk = true;     //当磁盘被写满时，停止日志输出

    }

    //google::SetLogFilenameExtension("91_");   //设置文件名扩展，如平台？或其它需要区分的信息
    //google::InstallFailureSignalHandler();    //捕捉 core dumped
    //google::InstallFailureWriter(&Log);       //默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出方式：

    return true;
}

bool NFCLogModule::Shut()
{

#ifdef NF_USE_ACTOR
    NFIActor* pActor = (NFIActor*)(pPluginManager);
    if (pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
    {
        google::ShutdownGoogleLogging();
    }
#else
    google::ShutdownGoogleLogging();
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
#ifdef NF_DYNAMIC_PLUGIN
	NFIActor* pActor = (NFIActor*)(pPluginManager);
	if (!pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
	{
		NFIActorMessage message;
		message.eType = NFIActorMessage::EACTOR_LOG_MSG;
		message.data = szBuffer;

		const Theron::Address address = pActor->GetActorManager()->GetAddress(NFIActorManager::EACTOR_MAIN);
		if (address != Theron::Address::Null())
		{
			pActor->GetFramework().Send(message, pActor->GetAddress(), address);
		}

		return true;
	}
#endif
#endif


    switch (nll)
    {
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
        default:
            LOG(INFO) << szBuffer;
            break;
    }

    return true;
}

bool NFCLogModule::LogElement(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strElement, const std::string& strDesc, const char* func, int line)
{
	std::ostringstream stream;
	stream << " Element= " <<  strElement << " Desc=  " << strDesc;
	LogNormal(nll, ident, stream, func, line);

    return true;
}

bool NFCLogModule::LogProperty(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strProperty, const std::string& strDesc, const char* func, int line)
{
	std::ostringstream stream;
	stream << " Property= " <<  strProperty << " Desc=  " << strDesc;
	LogNormal(nll, ident, stream, func, line);

    return true;

}

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol, const char* func, int line)
{
	std::ostringstream stream;
	stream << " Record= " <<  strRecord << " Desc=  " << strDesc << " Row= " << nRow << " Col=" << nCol;
	LogNormal(nll, ident, stream, func, line);

    return true;

}

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc, const char* func, int line)
{
	std::ostringstream stream;
	stream << " Record= " <<  strRecord << " Desc=  " << strDesc;
	LogNormal(nll, ident, stream, func, line);

    return true;
}

bool NFCLogModule::LogObject(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strDesc, const char* func, int line)
{
	std::ostringstream stream;
	stream << " Desc=  " << strDesc;
	LogNormal(nll, ident, stream, func, line);

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
	std::ostringstream stream;
	stream << " Info= " <<  strInfo << " Desc=  " << strDesc;
	LogNormal(nll, ident, stream, func, line);

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const int nDesc, const char* func, int line)
{
	std::ostringstream stream;
	stream << " Info= " <<  strInfo << " Desc=  " << nDesc;
	LogNormal(nll, ident, stream, func, line);

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::ostringstream& stream, const char* func, int line)
{
    switch (nll)
    {
        case NFILogModule::NLL_INFO_NORMAL:
        {
            if (0 == line)
            {
                LOG(INFO) << "[NORMAL] Ident=" << ident.ToString() << " " << stream.str();
            }
            else
            {
                LOG(INFO) << "[NORMAL] Ident=" << ident.ToString() << " " << stream.str() << " " << func << " " << line;
            }
        }
        break;
        case NFILogModule::NLL_WARING_NORMAL:
        {
            if (0 == line)
            {
                LOG(WARNING) << "[NORMAL] Ident=" << ident.ToString() << " " << stream.str();
            }
            else
            {
                LOG(WARNING) << "[NORMAL] Ident=" << ident.ToString() << " " << stream.str() << " " << func << " " << line;
            }
        }
        break;
        case NFILogModule::NLL_ERROR_NORMAL:
        {
            if (0 == line)
            {
                LOG(ERROR) << "[NORMAL] Ident=" << ident.ToString() << " " << stream.str();
            }
            else
            {
                LOG(ERROR) << "[NORMAL] Ident=" << ident.ToString() << " " << stream.str() << " " << func << " " << line;
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
	std::ostringstream stream;
	stream << " Info= " <<  strInfo;
	LogNormal(nll, ident, stream, func, line);

    return true;
}
