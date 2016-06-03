// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCGuildRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCGuildRedisModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFCGuildRedisModule::NFCGuildRedisModule(NFIPluginManager * p)
{
    pPluginManager = p;
}

bool NFCGuildRedisModule::Init()
{
    return true;
}

bool NFCGuildRedisModule::Shut()
{
    return true;
}

bool NFCGuildRedisModule::Execute()
{
    return true;
}

bool NFCGuildRedisModule::AfterInit()
{
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>("NFCNoSqlModule");
    m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>("NFCCommonRedisModule");
    
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pNoSqlModule);
    assert(NULL != m_pCommonRedisModule);

    return true;
}

bool NFCGuildRedisModule::GetGuildCacheInfo(const NFGUID& xGuid, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager)
{
    pPropertyManager = m_pCommonRedisModule->NewPropertyManager(NFrame::Guild::ThisName());
    if (!pPropertyManager.get())
    {
        return false;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    const std::string& strKey = m_pCommonRedisModule->GetPropertyCacheKey(NFrame::Guild::ThisName());
    std::string strValue;
    if (!pDriver->HGet(strKey, xGuid.ToString(), strValue))
    {
        return false;
    }

    NFMsg::ObjectPropertyList xMsg;
    if (!xMsg.ParseFromString(strValue))
    {
        return false;
    }

    return m_pCommonRedisModule->ConvertPBToPropertyManager(xMsg, pPropertyManager);
}

bool NFCGuildRedisModule::GetGuildCacheRecordInfo(const NFGUID& xGuid, const std::string& strRecordName, NF_SHARE_PTR<NFIRecord>& pRecord)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->NewRecordManager(NFrame::Guild::ThisName());
    if (!pRecordManager.get())
    {
        return false;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    const std::string& strKey = m_pCommonRedisModule->GetRecordCacheKey(NFrame::Guild::ThisName());
    std::string strValue;
    if (!pDriver->HGet(strKey, xGuid.ToString(), strValue))
    {
        return false;
    }

    NFMsg::ObjectRecordList xMsg;
    if (!xMsg.ParseFromString(strValue))
    {
        return false;
    }

    if (!m_pCommonRedisModule->ConvertPBToRecordManager(xMsg, pRecordManager))
    {
        return false;
    }

    pRecord = pRecordManager->GetElement(strRecordName);
    if (!pRecord.get())
    {
        return false;
    }

    return true;
}

bool NFCGuildRedisModule::GetGuildCacheRecordManager(const NFGUID& xGuid, NF_SHARE_PTR<NFIRecordManager>& pRecordManager)
{
    pRecordManager = m_pCommonRedisModule->NewRecordManager(NFrame::Guild::ThisName());
    if (!pRecordManager.get())
    {
        return false;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    const std::string& strKey = m_pCommonRedisModule->GetRecordCacheKey(NFrame::Guild::ThisName());
    std::string strValue;
    if (!pDriver->HGet(strKey, xGuid.ToString(), strValue))
    {
        return false;
    }

    NFMsg::ObjectRecordList xMsg;
    if (!xMsg.ParseFromString(strValue))
    {
        return false;
    }

    if (!m_pCommonRedisModule->ConvertPBToRecordManager(xMsg, pRecordManager))
    {
        return false;
    }

    return true;
}
