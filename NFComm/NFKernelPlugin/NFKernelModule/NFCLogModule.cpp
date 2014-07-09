// -------------------------------------------------------------------------
//    @FileName      :    NFCLogModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"
#include <time.h>
#include "NFStackWalker.h"
#include "NFCLogModule.h"
#include <boost/filesystem.hpp>
#include "NFComm/NFPluginModule/NFIActorManager.h"
#include "NFComm/NFPluginModule/NFIActor.h"

NFCLogModule::NFCLogModule(NFIPluginManager* p)
{
    pPluginManager = p;
    
    //m_pFd = NULL;
}

bool NFCLogModule::Init()
{
#ifdef NF_DYNAMIC_PLUGIN

    NFIActor* pActor = (NFIActor*)(pPluginManager);
    if(pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
    {
#endif

        char szName[MAX_PATH] = {0};

        GetConsoleTitle(szName, MAX_PATH);
        google::InitGoogleLogging(szName);

        std::string strPath = std::string("./log/") + szName;
        if (!boost::filesystem::exists(strPath))
        {
            boost::filesystem::create_directories(strPath);
        }

#ifdef NF_DEBUG_MODE
        google::SetStderrLogging(google::GLOG_INFO); //���ü������ google::INFO ����־ͬʱ�������Ļ
#else
        google::SetStderrLogging(google::GLOG_FATAL);//���ü������ google::FATAL ����־ͬʱ�������Ļ
#endif

        FLAGS_colorlogtostderr = true;    //�����������Ļ����־��ʾ��Ӧ��ɫ
        FLAGS_servitysinglelog = true;

        google::SetLogDestination(google::GLOG_FATAL, std::string(strPath + "/log_fatal_").c_str());
        google::SetLogDestination(google::GLOG_ERROR, std::string(strPath + "/log_error_").c_str());      //���� google::error �������־�洢·�����ļ���ǰ׺
        google::SetLogDestination(google::GLOG_WARNING, std::string(strPath + "/log_warning_").c_str());  //���� google::WARNING �������־�洢·�����ļ���ǰ׺
        google::SetLogDestination(google::GLOG_INFO, std::string(strPath + "/log_Info_").c_str());        //���� google::INFO �������־�洢·�����ļ���ǰ׺

        FLAGS_logbufsecs = 0;                       //������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
        FLAGS_max_log_size = 100;                   //�����־��СΪ 100MB
        FLAGS_stop_logging_if_full_disk = true;     //�����̱�д��ʱ��ֹͣ��־���
       
#ifdef NF_DYNAMIC_PLUGIN
    }
#endif

    //google::SetLogFilenameExtension("91_");   //�����ļ�����չ����ƽ̨����������Ҫ���ֵ���Ϣ
    //google::InstallFailureSignalHandler();    //��׽ core dumped
    //google::InstallFailureWriter(&Log);       //Ĭ�ϲ�׽ SIGSEGV �ź���Ϣ���������� stderr������ͨ������ķ����Զ��������ʽ��
    
    return true;
}

bool NFCLogModule::Shut()
{
#ifdef NF_DYNAMIC_PLUGIN
    NFIActor* pActor = (NFIActor*)(pPluginManager);
    if(pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
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

void NFCLogModule::OnReload(const char* strModuleName, NFILogicModule* pModule)
{
    return;

}

bool NFCLogModule::Log(const NF_LOG_LEVEL nll, const char* format, ...)
{
    char szBuffer[1024 * 10] = {0};

    va_list args;
    va_start(args, format);
    _vsnprintf(szBuffer, sizeof(szBuffer) - 1, format, args);
    va_end(args);

#ifdef NF_DYNAMIC_PLUGIN
    NFIActor* pActor = (NFIActor*)(pPluginManager);
    if(!pActor->GetActorID() == NFIActorManager::EACTOR_MAIN)
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

    switch (nll)
    {
        case NFILogModule::NLL_INFO_NORMAL:
            LOG(INFO) << szBuffer;
            break;
        case NFILogModule::NLL_WARING_NORMAL:
            LOG(WARNING) << szBuffer;
            break;
        case NFILogModule::NLL_ERROR_NORMAL:
            LOG(ERROR) << szBuffer;
            break;
        default:
            LOG(INFO) << szBuffer;
            break;
    }

    return true;
}

bool NFCLogModule::LogElement(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strElement, const std::string& strDesc,  char* func, int line)
{
    if(line > 0)
    {
        Log(nll, "[ELEMENT] Ident=%lld Element=%s %s %s %d\n", ident.nData64, strElement.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[ELEMENT] Ident=%lld Element=%s %s\n", ident.nData64, strElement.c_str(), strDesc.c_str());
    }
    LogStack();

    return true;
}

bool NFCLogModule::LogProperty(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strProperty, const std::string& strDesc,  char* func, int line)
{
    if(line > 0)
    {
        Log(nll, "[PROPERTY] Ident=%lld Element=%s %s %s %d\n", ident.nData64, strProperty.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[PROPERTY] Ident=%lld Element=%s %s\n", ident.nData64, strProperty.c_str(), strDesc.c_str());
    }

    LogStack();

    return true;

}

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol,  char* func, int line)
{
    if(line > 0)
    {
        Log(nll, "[RECORD] Ident=%lld Record=%s Row=%d Col=%d %s %s %d\n", ident.nData64, strRecord.c_str(), nRow, nCol, strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[RECORD] Ident=%lld Record=%s Row=%d Col=%d %s \n", ident.nData64, strRecord.c_str(), nRow, nCol, strDesc.c_str());
    }

    LogStack();

    return true;

}

bool NFCLogModule::LogRecord(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strRecord, const std::string& strDesc,  char* func, int line)
{
    if(line > 0)
    {
        Log(nll, "[RECORD] Ident=%lld Record=%s %s %s %d\n", ident.nData64, strRecord.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[RECORD] Ident=%lld Record=%s %s\n", ident.nData64, strRecord.c_str(), strDesc.c_str());
    }

    LogStack();

    return true;
}

bool NFCLogModule::LogObject(const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strDesc,  char* func, int line)
{
    if(line > 0)
    {
        Log(nll, "[OBJECT] Ident=%lld %s %s %d\n", ident.nData64,  strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[OBJECT] Ident=%lld %s\n", ident.nData64, strDesc.c_str());
    }

    LogStack();

    return true;

}

void NFCLogModule::LogStack()
{

}

bool NFCLogModule::LogNormal( const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const std::string& strDesc,  char* func, int line )
{
    if(line > 0)
    {
        Log(nll, "[NORMAL] Ident=%lld  %s %s %s %d\n", ident.nData64, strInfo.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "[NORMAL] Ident=%lld  %s %s \n", ident.nData64, strInfo.c_str(), strDesc.c_str());
    }
    LogStack();
    return true;
}

bool NFCLogModule::LogNormal( const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, const int nDesc,  char* func, int line )
{
    if(line > 0)
    {
        Log(nll, "[NORMAL] Ident=%lld  %s %d %s %d\n", ident.nData64, strInfo.c_str(), nDesc, func, line);
    }
    else
    {
        Log(nll, "[NORMAL] Ident=%lld  %s %d\n", ident.nData64, strInfo.c_str(), nDesc);
    }

    LogStack();
    return true;
}

bool NFCLogModule::LogNormal( const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::ostringstream& stream, char* func, int line)
{
	switch (nll)
	{
	case NFILogModule::NLL_INFO_NORMAL:
        {
            if (0 == line)
            {
                LOG(INFO) << "[NORMAL] Ident=" << ident.nData64 << " " << stream.str();
            }
            else
            {
                LOG(INFO) << "[NORMAL] Ident=" << ident.nData64 << " " << stream.str() << " " << func << " " << line;
            }
        }
        break;
	case NFILogModule::NLL_WARING_NORMAL:
        {
            if (0 == line)
            {
                LOG(WARNING) << "[NORMAL] Ident=" << ident.nData64 << " " << stream.str();
            }
            else
            {
                LOG(WARNING) << "[NORMAL] Ident=" << ident.nData64 << " " << stream.str() << " " << func << " " << line;
            }
        }
		break;
	case NFILogModule::NLL_ERROR_NORMAL:
        {
            if (0 == line)
            {
                LOG(ERROR) << "[NORMAL] Ident=" << ident.nData64 << " " << stream.str();
            }
            else
            {
                LOG(ERROR) << "[NORMAL] Ident=" << ident.nData64 << " " << stream.str() << " " << func << " " << line;
            }
        }
		break;
	default:
		break;
	}
	return true;
}

bool NFCLogModule::LogNormal( const NF_LOG_LEVEL nll, const NFIDENTID ident, const std::string& strInfo, char* func /*= ""*/, int line /*= 0*/ )
{
	if(line > 0)
	{
		Log(nll, "[NORMAL] Ident=%lld  %s %s %d\n", ident.nData64, strInfo.c_str(), func, line);
	}
	else
	{
		Log(nll, "[NORMAL] Ident=%lld  %s\n", ident.nData64, strInfo.c_str());
	}

	return true;
}
