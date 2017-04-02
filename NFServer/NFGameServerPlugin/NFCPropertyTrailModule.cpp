// -------------------------------------------------------------------------
//    @FileName			:    NFCPropertyTrailModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyTrailModule
//
// -------------------------------------------------------------------------

#include "NFCPropertyTrailModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFCore/NFDataList.hpp"

bool NFCPropertyTrailModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}


bool NFCPropertyTrailModule::Shut()
{
    return true;
}

bool NFCPropertyTrailModule::Execute()
{
    return true;
}

bool NFCPropertyTrailModule::AfterInit()
{

    return true;
}

void NFCPropertyTrailModule::StartTrail(const NFGUID self)
{
    LogObjectData(self);


}

void NFCPropertyTrailModule::EndTrail(const NFGUID self)
{

}

int NFCPropertyTrailModule::LogObjectData(const NFGUID& self)
{
    NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
    if (nullptr == xObject)
    {
        return -1;
    }

    NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = xObject->GetPropertyManager();
    if (nullptr != xPropertyManager)
    {
        NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->First();
        while (nullptr != xProperty)
        {
            std::ostringstream stream;

            stream << " Start trail ";
            stream << xProperty->ToString();

            m_pLogModule->LogProperty(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, xProperty->GetKey(), stream.str(),  __FUNCTION__, __LINE__);

            xProperty = xPropertyManager->Next();
        }
    }

    NF_SHARE_PTR<NFIRecordManager> xRecordManager = xObject->GetRecordManager();
    if (nullptr != xRecordManager)
    {
        NF_SHARE_PTR<NFIRecord> xRecord = xRecordManager->First();
        while (nullptr != xRecord)
        {
            for (int i = 0; i < xRecord->GetRows(); ++i)
            {
                NFDataList xDataList;
                bool bRet = xRecord->QueryRow(i, xDataList);
                if (bRet)
                {
                    std::ostringstream stream;
                    stream << " Start trail Row[" << i << "]";

                    for (int j = 0; j < xDataList.GetCount(); ++j)
                    {
                        stream << " [" << j << "] " << xDataList.StringValEx(j);
                    }

                    m_pLogModule->LogRecord(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, xRecord->GetName(), stream.str(),  __FUNCTION__, __LINE__);
                }
            }

            xRecord = xRecordManager->Next();
        }
    }

    return 0;
}

int NFCPropertyTrailModule::OnObjectPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
    std::ostringstream stream;

    stream << " Trailing ";
    stream << " [Old] ";
    stream << oldVar.GetString();
    stream << " [New] ";
    stream << newVar.GetString();

    m_pLogModule->LogProperty(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, strPropertyName, stream.str(),  __FUNCTION__, __LINE__);

    return 0;
}

int NFCPropertyTrailModule::OnObjectRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    std::ostringstream stream;
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
    if (nullptr == xRecord)
    {
        return 0;
    }

    switch (xEventData.nOpType)
    {
        case RECORD_EVENT_DATA::Add:
        {
            NFDataList xDataList;
            bool bRet = xRecord->QueryRow(xEventData.nRow, xDataList);
            if (bRet)
            {
                stream << " Trail Add Row[" << xEventData.nRow << "]";

                for (int j = 0; j < xDataList.GetCount(); ++j)
                {
                    stream << " [" << j << "] " << xDataList.StringValEx(j);
                }

                m_pLogModule->LogRecord(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, xRecord->GetName(), stream.str(),  __FUNCTION__, __LINE__);
            }
        }
        break;
        case RECORD_EVENT_DATA::Del:
        {
            stream << " Trail Del Row[" << xEventData.nRow << "]";
            m_pLogModule->LogRecord(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, xRecord->GetName(), stream.str(),  __FUNCTION__, __LINE__);
        }
        break;
        case RECORD_EVENT_DATA::Swap:
        {
            stream << " Trail Swap Row[" << xEventData.nRow << "] Row[" << xEventData.nCol << "]";
            m_pLogModule->LogRecord(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, xRecord->GetName(), stream.str(),  __FUNCTION__, __LINE__);
        }
        break;
        case RECORD_EVENT_DATA::Create:
            break;
        case RECORD_EVENT_DATA::Update:
        {
            stream << " Trail UpData Row[" << xEventData.nRow << "] Col[" << xEventData.nCol << "]";
            stream << " [Old] " << oldVar.StringValEx();
            stream << " [New] " << newVar.StringValEx();
            m_pLogModule->LogRecord(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, self, xRecord->GetName(), stream.str(),  __FUNCTION__, __LINE__);
        }
        break;
        case RECORD_EVENT_DATA::Cleared:
            break;
        case RECORD_EVENT_DATA::Sort:
            break;
        default:
            break;
    }

    return 0;
}

int NFCPropertyTrailModule::TrailObjectData(const NFGUID& self)
{
    NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
    if (nullptr == xObject)
    {
        return -1;
    }

    NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = xObject->GetPropertyManager();
    if (nullptr != xPropertyManager)
    {
        NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->First();
        while (nullptr != xProperty)
        {
            m_pKernelModule->AddPropertyCallBack(self, xProperty->GetKey(), this, &NFCPropertyTrailModule::OnObjectPropertyEvent);

            xProperty = xPropertyManager->Next();
        }
    }

    NF_SHARE_PTR<NFIRecordManager> xRecordManager = xObject->GetRecordManager();
    if (nullptr != xRecordManager)
    {
        NF_SHARE_PTR<NFIRecord> xRecord = xRecordManager->First();
        while (nullptr != xRecord)
        {
            m_pKernelModule->AddRecordCallBack(self, xRecord->GetName(), this, &NFCPropertyTrailModule::OnObjectRecordEvent);


            xRecord = xRecordManager->Next();
        }
    }

    return 0;
}
