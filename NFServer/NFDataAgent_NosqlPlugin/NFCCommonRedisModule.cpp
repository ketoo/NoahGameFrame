// -------------------------------------------------------------------------
//    @FileName			:    NFCCommonRedisModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2013-10-03
//    @Module           :    NFCCommonRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCCommonRedisModule.h"
#include "NFComm/NFCore/NFCPropertyManager.h"
#include "NFComm/NFCore/NFCRecordManager.h"
#include "NFComm/NFPluginModule/NFINetModule.h"


NFCCommonRedisModule::NFCCommonRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

std::string NFCCommonRedisModule::GetPropertyCacheKey(const std::string& strClassName)
{
    return strClassName + "_ObjectProperty";
}

std::string NFCCommonRedisModule::GetRecordCacheKey(const std::string& strClassName)
{
    return strClassName + "_ObjectRecord";
}

bool NFCCommonRedisModule::AfterInit()
{
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>();
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
    m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();

    return true;
}

NF_SHARE_PTR<NFIPropertyManager> NFCCommonRedisModule::NewPropertyManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIPropertyManager> pStaticClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);
    if (pStaticClassPropertyManager)
    {
        NFGUID ident;
        NF_SHARE_PTR<NFIPropertyManager> pPropertyManager(NF_NEW NFCPropertyManager(ident));

        NF_SHARE_PTR<NFIProperty> pStaticConfigPropertyInfo = pStaticClassPropertyManager->First();
        while (pStaticConfigPropertyInfo.get())
        {
            NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->AddProperty(ident, pStaticConfigPropertyInfo->GetKey(), pStaticConfigPropertyInfo->GetType());

            xProperty->SetPublic(pStaticConfigPropertyInfo->GetPublic());
            xProperty->SetPrivate(pStaticConfigPropertyInfo->GetPrivate());
            xProperty->SetSave(pStaticConfigPropertyInfo->GetSave());
            xProperty->SetCache(pStaticConfigPropertyInfo->GetCache());
            xProperty->SetRelationValue(pStaticConfigPropertyInfo->GetRelationValue());

            pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
        }

        return pPropertyManager;
    }

    return NF_SHARE_PTR<NFIPropertyManager>(NULL);
}

NF_SHARE_PTR<NFIRecordManager> NFCCommonRedisModule::NewRecordManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIRecordManager> pStaticClassRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);
    if (pStaticClassRecordManager)
    {
        NFGUID ident;
        NF_SHARE_PTR<NFIRecordManager> pRecordManager(NF_NEW NFCRecordManager(ident));

        NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
        while (pConfigRecordInfo)
        {
            NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->AddRecord(ident,
                pConfigRecordInfo->GetName(),
                pConfigRecordInfo->GetInitData(),
                pConfigRecordInfo->GetTag(),
                pConfigRecordInfo->GetRows());

            xRecord->SetPublic(pConfigRecordInfo->GetPublic());
            xRecord->SetPrivate(pConfigRecordInfo->GetPrivate());
            xRecord->SetSave(pConfigRecordInfo->GetSave());
            xRecord->SetCache(pConfigRecordInfo->GetCache());

            pConfigRecordInfo = pStaticClassRecordManager->Next();
        }

        return pRecordManager;
    }

    return NF_SHARE_PTR<NFIRecordManager>(NULL);
}

NF_SHARE_PTR<NFIPropertyManager> NFCCommonRedisModule::GetCachePropertyInfo(const NFGUID& self, const std::string& strClassName)
{
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pCommonRedisModule->NewPropertyManager(strClassName);
    if (!pPropertyManager.get())
    {
        return nullptr;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return nullptr;
    }

    const std::string& strKey = m_pCommonRedisModule->GetPropertyCacheKey(strClassName);
    std::string strValue;
    if (!pDriver->HGet(strKey, self.ToString(), strValue))
    {
        return nullptr;
    }

    NFMsg::ObjectPropertyList xMsg;
    if (!xMsg.ParseFromString(strValue))
    {
        return nullptr;
    }

    if (!m_pCommonRedisModule->ConvertPBToPropertyManager(xMsg, pPropertyManager))
    {
        return nullptr;
    }

    return pPropertyManager;
}

NF_SHARE_PTR<NFIRecordManager> NFCCommonRedisModule::GetCacheRecordInfo(const NFGUID& self, const std::string& strClassName)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->NewRecordManager(strClassName);
    if (!pRecordManager.get())
    {
        return nullptr;
    }

    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return nullptr;
    }

    const std::string strKey = m_pCommonRedisModule->GetRecordCacheKey(strClassName);
    std::string strValue;
    if (!pDriver->HGet(strKey, self.ToString(), strValue))
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

bool NFCCommonRedisModule::SetCachePropertyInfo(const NFGUID& self, const std::string& strClassName, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager)
{
    if (self.IsNull())
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

    *xMsg.mutable_player_id() = NFINetModule::NFToPB(self);

    std::string strValue;
    if (!xMsg.SerializeToString(&strValue))
    {
        return false;
    }

    const std::string strKey = m_pCommonRedisModule->GetPropertyCacheKey(strClassName);
    if (!pDriver->HSet(strKey, self.ToString(), strValue))
    {
        return false;
    }

    return true;
}

bool NFCCommonRedisModule::SetCacheRecordInfo(const NFGUID& self, const std::string& strClassName, NF_SHARE_PTR<NFIRecordManager> pRecordManager)
{
    if (self.IsNull())
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

    *xMsg.mutable_player_id() = NFINetModule::NFToPB(self);

    std::string strValue;
    if (!xMsg.SerializeToString(&strValue))
    {
        return false;
    }

    const std::string strKey = m_pCommonRedisModule->GetRecordCacheKey(strClassName);
    if (!pDriver->HSet(strKey, self.ToString(), strValue))
    {
        return false;
    }

    return true;
}

bool NFCCommonRedisModule::RemoveCachePropertyInfo(const NFGUID& self, const std::string& strClassName)
{
    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    const std::string strKey = m_pCommonRedisModule->GetPropertyCacheKey(strClassName);
    std::string strValue;
    if (!pDriver->HDel(strKey, self.ToString()))
    {
        return false;
    }

    return true;
}

bool NFCCommonRedisModule::RemoveCacheRecordInfo(const NFGUID& self, const std::string& strClassName)
{
    NFINoSqlDriver* pDriver = m_pNoSqlModule->GetDriver();
    if (!pDriver)
    {
        return false;
    }

    std::string strKey = m_pCommonRedisModule->GetRecordCacheKey(strClassName);
    std::string strValue;
    if (!pDriver->HDel(strKey, self.ToString()))
    {
        return false;
    }

    return true;
}

bool NFCCommonRedisModule::ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& xMsg, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager)
{
    NFGUID xIdent = NFINetModule::PBToNF(xMsg.player_id());
    pPropertyManager->Self();

    for (int i = 0; i < xMsg.property_int_list_size(); ++i)
    {
        const NFMsg::PropertyInt& xPropertyData = xMsg.property_int_list(i);
        const std::string& strPropertyName = xPropertyData.property_name();
        const int xPropertyValue = xPropertyData.data();

        NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strPropertyName);
        if (!pProperty)
        {
            pProperty = pPropertyManager->AddProperty(xIdent, strPropertyName, TDATA_INT);
        }

        pProperty->SetInt(xPropertyValue);
    }

    for (int i = 0; i < xMsg.property_float_list_size(); ++i)
    {
        const NFMsg::PropertyFloat& xPropertyData = xMsg.property_float_list(i);
        const std::string& strPropertyName = xPropertyData.property_name();
        const float xPropertyValue = xPropertyData.data();

        NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strPropertyName);
        if (!pProperty)
        {
            pProperty = pPropertyManager->AddProperty(xIdent, strPropertyName, TDATA_FLOAT);
        }

        pProperty->SetFloat(xPropertyValue);
    }

    for (int i = 0; i < xMsg.property_string_list_size(); ++i)
    {
        const NFMsg::PropertyString& xPropertyData = xMsg.property_string_list(i);
        const std::string& strPropertyName = xPropertyData.property_name();
        const std::string xPropertyValue = xPropertyData.data();

        NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strPropertyName);
        if (!pProperty)
        {
            pProperty = pPropertyManager->AddProperty(xIdent, strPropertyName, TDATA_STRING);
        }

        pProperty->SetString(xPropertyValue);
    }

    for (int i = 0; i < xMsg.property_object_list_size(); ++i)
    {
        const NFMsg::PropertyObject& xPropertyData = xMsg.property_object_list(i);
        const std::string& strPropertyName = xPropertyData.property_name();
        const NFGUID xPropertyValue = NFINetModule::PBToNF(xPropertyData.data());

        NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strPropertyName);
        if (!pProperty)
        {
            pProperty = pPropertyManager->AddProperty(xIdent, strPropertyName, TDATA_OBJECT);
        }

        pProperty->SetObject(xPropertyValue);
    }
    
    return true;
}

bool NFCCommonRedisModule::ConvertPBToRecordManager(const NFMsg::ObjectRecordList& xMsg, NF_SHARE_PTR<NFIRecordManager>& pRecordManager)
{
    NFGUID xIdent = NFINetModule::PBToNF(xMsg.player_id());
    //pRecordManager->Self();

    for (int iRecord = 0; iRecord < xMsg.record_list_size(); ++iRecord)
    {
        const NFMsg::ObjectRecordBase& xRecordBase = xMsg.record_list(iRecord);
        const std::string& strRecordName = xRecordBase.record_name();

        NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->GetElement(strRecordName);
        if (!pRecord.get())
        {
            continue;
        }

        for (int iStuct = 0; iStuct < xRecordBase.row_struct_size(); iStuct++)
        {
            const NFMsg::RecordAddRowStruct& xAddRowStruct = xRecordBase.row_struct(iStuct);

            const int nCommonRow = xAddRowStruct.row();
            pRecord->SetUsed(nCommonRow, true);

            for (int i = 0; i < xAddRowStruct.record_int_list_size(); i++)
            {
                const NFMsg::RecordInt& xPropertyData = xAddRowStruct.record_int_list(i);
                const int nRow = xPropertyData.row();
                const int nCol = xPropertyData.col();
                const NFINT64 xPropertyValue = xPropertyData.data();

                pRecord->SetInt(nRow, nCol, xPropertyValue);
            }

            for (int i = 0; i < xAddRowStruct.record_float_list_size(); i++)
            {
                const NFMsg::RecordFloat& xPropertyData = xAddRowStruct.record_float_list(i);
                const int nRow = xPropertyData.row();
                const int nCol = xPropertyData.col();
                const float xPropertyValue = xPropertyData.data();

                pRecord->SetFloat(nRow, nCol, xPropertyValue);
            }

            for (int i = 0; i < xAddRowStruct.record_string_list_size(); i++)
            {
                const NFMsg::RecordString& xPropertyData = xAddRowStruct.record_string_list(i);
                const int nRow = xPropertyData.row();
                const int nCol = xPropertyData.col();
                const std::string xPropertyValue = xPropertyData.data();

                pRecord->SetString(nRow, nCol, xPropertyValue.c_str());
            }

            for (int i = 0; i < xAddRowStruct.record_object_list_size(); i++)
            {
                const NFMsg::RecordObject& xPropertyData = xAddRowStruct.record_object_list(i);
                const int nRow = xPropertyData.row();
                const int nCol = xPropertyData.col();
                const NFGUID xPropertyValue = NFINetModule::PBToNF(xPropertyData.data());

                pRecord->SetObject(nRow, nCol, xPropertyValue);
            }
        }
    }
    return true;
}

bool NFCCommonRedisModule::ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager, NFMsg::ObjectPropertyList& xMsg, const bool bCheckCache /*= true*/)
{
    *xMsg.mutable_player_id() = NFINetModule::NFToPB(pPropertyManager->Self());
    for (NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->First(); pProperty != NULL; pProperty = pPropertyManager->Next())
    {
        const int nType = pProperty->GetType();
        if (bCheckCache)
        {
            if (!pProperty->GetCache())
            {
                continue;
            }
        }

        switch (nType)
        {
        case TDATA_INT:
        {
            NFMsg::PropertyInt* pPropertyData = xMsg.add_property_int_list();
            const std::string& strPropertyName = pProperty->GetKey();
            const NFINT64 xPropertyValue = pProperty->GetInt();

            if (pPropertyData)
            {
                pPropertyData->set_property_name(strPropertyName);
                pPropertyData->set_data(xPropertyValue);
            }
        }
        break;
        case TDATA_FLOAT:
        {
            NFMsg::PropertyFloat* pPropertyData = xMsg.add_property_float_list();
            const std::string& strPropertyName = pProperty->GetKey();
            const float xPropertyValue = pProperty->GetFloat();

            if (pPropertyData)
            {
                pPropertyData->set_property_name(strPropertyName);
                pPropertyData->set_data(xPropertyValue);
            }
        }
        break;
        case TDATA_STRING:
        {
            NFMsg::PropertyString* pPropertyData = xMsg.add_property_string_list();
            const std::string& strPropertyName = pProperty->GetKey();
            const std::string xPropertyValue = pProperty->GetString();

            if (pPropertyData)
            {
                pPropertyData->set_property_name(strPropertyName);
                pPropertyData->set_data(xPropertyValue);
            }
        }
        break;
        case TDATA_OBJECT:
        {
            NFMsg::PropertyObject* pPropertyData = xMsg.add_property_object_list();
            const std::string& strPropertyName = pProperty->GetKey();
            const NFGUID& xPropertyValue = pProperty->GetObject();

            if (pPropertyData)
            {
                pPropertyData->set_property_name(strPropertyName);
                *pPropertyData->mutable_data() = NFINetModule::NFToPB(xPropertyValue);
            }
        }
        break;
        default:
            break;
        }
    }
    
    return true;
}

bool NFCCommonRedisModule::ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecordManager, NFMsg::ObjectRecordList& xMsg, const bool bCheckCache /*= true*/)
{
    *xMsg.mutable_player_id() = NFINetModule::NFToPB(pRecordManager->Self());
    for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
    {
        const std::string& strRecordName = pRecord->GetName();

        NFMsg::ObjectRecordBase* pRecordData = xMsg.add_record_list();
        if (!pRecordData)
        {
            continue;
        }

        if (bCheckCache)
        {
            if (!pRecord->GetCache())
            {
                continue;
            }
        }

        pRecordData->set_record_name(strRecordName);
        for (int iRow = 0; iRow < pRecord->GetRows(); iRow++)
        {
            if (!pRecord->IsUsed(iRow))
            {
                continue;
            }

            NFMsg::RecordAddRowStruct* pRowData = pRecordData->add_row_struct();
            if (!pRowData)
            {
                continue;
            }

            for (int iCol = 0; iCol < pRecord->GetCols(); iCol++)
            {
                const int nType = pRecord->GetColType(iCol);
                switch (nType)
                {
                case TDATA_INT:
                {
                    NFMsg::RecordInt* pPropertyData = pRowData->add_record_int_list();
                    const NFINT64 xPropertyValue = pRecord->GetInt(iRow, iCol);

                    if (pPropertyData)
                    {
                        pPropertyData->set_col(iCol);
                        pPropertyData->set_row(iRow);
                        pPropertyData->set_data(xPropertyValue);
                    }
                }
                break;
                case TDATA_FLOAT:
                {
                    NFMsg::RecordFloat* pPropertyData = pRowData->add_record_float_list();
                    const float xPropertyValue = pRecord->GetFloat(iRow, iCol);

                    if (pPropertyData)
                    {
                        pPropertyData->set_col(iCol);
                        pPropertyData->set_row(iRow);
                        pPropertyData->set_data(xPropertyValue);
                    }
                }
                break;
                case TDATA_STRING:
                {
                    NFMsg::RecordString* pPropertyData = pRowData->add_record_string_list();
                    const std::string xPropertyValue = pRecord->GetString(iRow, iCol);

                    if (pPropertyData)
                    {
                        pPropertyData->set_col(iCol);
                        pPropertyData->set_row(iRow);
                        pPropertyData->set_data(xPropertyValue);
                    }
                }
                break;
                case TDATA_OBJECT:
                {
                    NFMsg::RecordObject* pPropertyData = pRowData->add_record_object_list();
                    const NFGUID xPropertyValue = pRecord->GetObject(iRow, iCol);

                    if (pPropertyData)
                    {
                        pPropertyData->set_col(iCol);
                        pPropertyData->set_row(iRow);
                        *pPropertyData->mutable_data() = NFINetModule::NFToPB(xPropertyValue);
                    }
                }
                break;
                default:
                    break;
                }
            }
        }
    }

    return true;
}
