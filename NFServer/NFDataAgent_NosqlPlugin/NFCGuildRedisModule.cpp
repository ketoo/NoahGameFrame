// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCGuildRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCGuildRedisModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"

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
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pNoSqlModule);
    assert(NULL != m_pCommonRedisModule);
    assert(NULL != m_pKernelModule);


    m_pKernelModule->AddClassCallBack(NFrame::Guild::ThisName(), this, &NFCGuildRedisModule::OnObjectClassEvent);
    return true;
}

int NFCGuildRedisModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    switch (eClassEvent)
    {
    case CLASS_OBJECT_EVENT::COE_CREATE_FINISH:
    {
        NF_SHARE_PTR<NFIObject> pSelf = m_pKernelModule->GetObject(self);
        if (pSelf)
        {
            SetGuildCachePropertyInfo(self, pSelf->GetPropertyManager());
            SetGuildCacheRecordManager(self, pSelf->GetRecordManager());
        }
    }
    break;
    default:
        break;
    }

    return 0;
}

NF_SHARE_PTR<NFIPropertyManager> NFCGuildRedisModule::GetGuildCachePropertyInfo(const NFGUID& xGuid)
{
    return m_pCommonRedisModule->GetCachePropertyInfo(xGuid, NFrame::Guild::ThisName());
}

bool NFCGuildRedisModule::GetGuildCachePropertyInfo(const std::vector<std::string>& xGuidList, std::vector<NF_SHARE_PTR<NFIPropertyManager>>& xPMList)
{
    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    const std::string& strKey = m_pCommonRedisModule->GetPropertyCacheKey(NFrame::Guild::ThisName());
    std::vector<std::string> strValueList;
    if (!pDriver->HMGet(strKey, xGuidList, strValueList))
    {
        return nullptr;
    }

    for (std::vector<std::string>::iterator it; it != strValueList.end(); ++it)
    {
        std::string& strValue = *it;

        NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pCommonRedisModule->NewPropertyManager(NFrame::Guild::ThisName());
        if (!pPropertyManager.get())
        {
            continue;
        }

        NFMsg::ObjectPropertyList xMsg;
        if (!xMsg.ParseFromString(strValue))
        {
            continue;
        }

        if (!m_pCommonRedisModule->ConvertPBToPropertyManager(xMsg, pPropertyManager))
        {
            continue;
        }

        xPMList.push_back(pPropertyManager);
    }

    return true;
}

NF_SHARE_PTR<NFIRecordManager> NFCGuildRedisModule::GetGuildCacheRecordManager(const NFGUID& xGuid)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->NewRecordManager(NFrame::Guild::ThisName());
    if (!pRecordManager.get())
    {
        return nullptr;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return nullptr;
    }

    const std::string& strKey = m_pCommonRedisModule->GetRecordCacheKey(NFrame::Guild::ThisName());
    std::string strValue;
    if (!pDriver->HGet(strKey, xGuid.ToString(), strValue))
    {
        return nullptr;
    }

    NFMsg::ObjectRecordList xMsg;
    if (!xMsg.ParseFromString(strValue))
    {
        return nullptr;
    }

    if (!m_pCommonRedisModule->ConvertPBToRecordManager(xMsg, pRecordManager))
    {
        return nullptr;
    }

    return pRecordManager;
}

bool NFCGuildRedisModule::GetGuildCacheRecordManager(const std::vector<std::string>& xGuidList, std::vector<NF_SHARE_PTR<NFIRecordManager>>& xRMList)
{
	NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
	if (!pDriver)
	{
		return false;
	}

	const std::string& strKey = m_pCommonRedisModule->GetRecordCacheKey(NFrame::Guild::ThisName());
	std::vector<std::string> strValueList;
	if (!pDriver->HMGet(strKey, xGuidList, strValueList))
	{
		return nullptr;
	}

	for (std::vector<std::string>::iterator it; it != strValueList.end(); ++it)
	{
		std::string& strValue = *it;

		NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->NewRecordManager(NFrame::Guild::ThisName());
		if (!pRecordManager.get())
		{
			continue;
		}

		NFMsg::ObjectRecordList xMsg;
		if (!xMsg.ParseFromString(strValue))
		{
			continue;
		}

		if (!m_pCommonRedisModule->ConvertPBToRecordManager(xMsg, pRecordManager))
		{
			continue;
		}

		xRMList.push_back(pRecordManager);
	}

	return true;
}

bool NFCGuildRedisModule::SetGuildCachePropertyInfo(const NFGUID& xGuid, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager)
{
    if (xGuid.IsNull())
    {
        return false;
    }

    if (!pPropertyManager.get())
    {
        return false;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    NFMsg::ObjectPropertyList xMsg;
    if (!m_pCommonRedisModule->ConvertPropertyManagerToPB(pPropertyManager, xMsg, true))
    {
        return false;
    }

    *xMsg.mutable_player_id() = NFINetModule::NFToPB(xGuid);

    std::string strValue;
    if (!xMsg.SerializeToString(&strValue))
    {
        return false;
    }

    const std::string& strKey = m_pCommonRedisModule->GetPropertyCacheKey(NFrame::Guild::ThisName());
    if (!pDriver->HSet(strKey, xGuid.ToString(), strValue))
    {
        return false;
    }

	return true;
}

bool NFCGuildRedisModule::SetGuildCacheRecordManager(const NFGUID& xGuid, NF_SHARE_PTR<NFIRecordManager>& pRecordManager)
{
    if (xGuid.IsNull())
    {
        return false;
    }

    if (!pRecordManager.get())
    {
        return false;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    NFMsg::ObjectRecordList xMsg;
    if (!m_pCommonRedisModule->ConvertRecordManagerToPB(pRecordManager, xMsg, true))
    {
        return false;
    }

    *xMsg.mutable_player_id() = NFINetModule::NFToPB(xGuid);

    std::string strValue;
    if (!xMsg.SerializeToString(&strValue))
    {
        return false;
    }

    const std::string& strKey = m_pCommonRedisModule->GetRecordCacheKey(NFrame::Guild::ThisName());
    if (!pDriver->HSet(strKey, xGuid.ToString(), strValue))
    {
        return false;
    }

    return true;
}

