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
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"


NFCCommonRedisModule::NFCCommonRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

std::string NFCCommonRedisModule::GetPropertyCacheKey(const NFGUID& self)
{
    return self.ToString() + "_ObjectProperty";
}

std::string NFCCommonRedisModule::GetRecordCacheKey(const NFGUID& self)
{
    return self.ToString() + "_ObjectRecord";
}

std::string NFCCommonRedisModule::GetAccountCacheKey(const std::string & strAccount)
{
	return strAccount + "_AccountInfo";
}

std::string NFCCommonRedisModule::GetTileCacheKey(const int & nSceneID)
{
	return lexical_cast<std::string>(nSceneID) + "_TileInfo";
}

bool NFCCommonRedisModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

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
            xProperty->SetRef(pStaticConfigPropertyInfo->GetRef());

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

NF_SHARE_PTR<NFIPropertyManager> NFCCommonRedisModule::GetCachePropertyInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList)
{
	//TODO optimize
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = NewPropertyManager(strClassName);
    if (!pPropertyManager)
    {
        return nullptr;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return nullptr;
    }

	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->First();
	while (xProperty)
	{
		if (xProperty->GetCache() || xProperty->GetSave())
		{
			vKeyCacheList.push_back(xProperty->GetKey());
		}

		xProperty = pPropertyManager->Next();
	}

	//cache
	std::string strCacheKey = GetPropertyCacheKey(self);
    if (!pDriver->HMGET(strCacheKey, vKeyCacheList, vValueCacheList))
    {
        return nullptr;
    }

	if (vKeyCacheList.size() == vValueCacheList.size())
	{
		ConvertPBToPropertyManager(vKeyCacheList, vValueCacheList, pPropertyManager);
	}

    return pPropertyManager;
}

NF_SHARE_PTR<NFIRecordManager> NFCCommonRedisModule::GetCacheRecordInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = NewRecordManager(strClassName);
    if (!pRecordManager.get())
    {
        return nullptr;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return nullptr;
    }


	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First();
	while (xRecord)
	{
		if (xRecord->GetCache() || xRecord->GetSave())
		{
			vKeyCacheList.push_back(xRecord->GetName());
		}

		xRecord = pRecordManager->Next();
	}

	//cache
	std::string strCacheKey = GetRecordCacheKey(self);
	if (!pDriver->HMGET(strCacheKey, vKeyCacheList, vValueCacheList))
	{
		return nullptr;
	}

	if (vKeyCacheList.size() == vValueCacheList.size())
	{
		ConvertPBToRecordManager(vKeyCacheList, vValueCacheList, pRecordManager);
	}

    return pRecordManager;
}

bool NFCCommonRedisModule::SaveCachePropertyInfo(const NFGUID& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const int nExpireSecond)
{
    if (self.IsNull())
    {
        return false;
    }

    if (!pPropertyManager)
    {
        return false;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return false;
    }

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
    if (!ConvertPropertyManagerToPB(pPropertyManager, vKeyList, vValueList))
    {
        return false;
    }

	if (vKeyList.size() != vValueList.size())
	{
		return false;
	}

	std::string strKey= GetPropertyCacheKey(self);

    if (!pDriver->HMSET(strKey, vKeyList, vValueList))
    {
        return false;
    }

	if (nExpireSecond > 0)
	{
		pDriver->EXPIRE(strKey, nExpireSecond);
	}

    return true;
}

bool NFCCommonRedisModule::SaveCacheRecordInfo(const NFGUID& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const int nExpireSecond)
{
    if (self.IsNull())
    {
        return false;
    }

    if (!pRecordManager)
    {
        return false;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return false;
    }

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
    if (!ConvertRecordManagerToPB(pRecordManager, vKeyList, vValueList))
    {
        return false;
    }

	if (vKeyList.size() != vValueList.size())
	{
		return false;
	}

	std::string strKey = GetRecordCacheKey(self);
	if (!pDriver->HMSET(strKey, vKeyList, vValueList))
	{
		return false;
	}

	if (nExpireSecond > 0)
	{
		pDriver->EXPIRE(strKey, nExpireSecond);
	}

    return true;
}

bool NFCCommonRedisModule::ConvertPBToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& strValue = vValueList[i];
			if (strValue.empty())
			{
				continue;
			}

			NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strKey);
			if (!pProperty->GetCache() && !pProperty->GetSave())
			{
				continue;
			}

			if(!pProperty->FromString(strValue))
			{
				//TODO
				//error
			}
		}
	}

    return true;
}

bool NFCCommonRedisModule::ConvertPBToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager> pRecordManager)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& strValue = vValueList[i];
			if (strValue.empty())
			{
				continue;
			}

			NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->GetElement(strKey);
			if (!pRecord->GetCache() && !pRecord->GetSave())
			{
				continue;
			}

			NFMsg::ObjectRecordBase xRecordData;
			if (xRecordData.ParseFromString(strValue))
			{
				ConvertPBToRecord(pRecord, &xRecordData);
			}
			else
			{
				//TODO
				//error
			}
		}
	}

    return true;
}

bool NFCCommonRedisModule::ConvertPropertyManagerToPB(NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList)
{
	for (NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->First(); pProperty != NULL; pProperty = pPropertyManager->Next())
	{
		const int nType = pProperty->GetType();
		if (!pProperty->GetCache() && !pProperty->GetSave())
		{
			continue;
		}

		const std::string& strPropertyName = pProperty->GetKey();
		const std::string& strPropertyValue = pProperty->ToString();

		vKeyList.push_back(strPropertyName);
		vValueList.push_back(strPropertyValue);
	}

	return true;
}

bool NFCCommonRedisModule::ConvertRecordManagerToPB(NF_SHARE_PTR<NFIRecordManager> pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList)
{
	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if (!pRecord->GetCache() && !pRecord->GetSave())
		{
			continue;
		}

		NFMsg::ObjectRecordBase* pRecordData = new NFMsg::ObjectRecordBase();
		if (!pRecordData)
		{
			continue;
		}

		ConvertRecordToPB(pRecord, pRecordData);

		////
		std::string strValue;
		if (!pRecordData->SerializeToString(&strValue))
		{
			continue;
		}

		vKeyList.push_back(pRecordData->record_name());
		vValueList.push_back(strValue);
	}

	return true;
}

bool NFCCommonRedisModule::ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase * pRecordData)
{
	pRecordData->set_record_name(pRecord->GetName());

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

		pRowData->set_row(iRow);

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
				const double xPropertyValue = pRecord->GetFloat(iRow, iCol);

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
				const std::string& xPropertyValue = pRecord->GetString(iRow, iCol);

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
			case TDATA_VECTOR2:
			{
				NFMsg::RecordVector2* pPropertyData = pRowData->add_record_vector2_list();
				const NFVector2 xPropertyValue = pRecord->GetVector2(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector2* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
				}
			}
			break;
			case TDATA_VECTOR3:
			{
				NFMsg::RecordVector3* pPropertyData = pRowData->add_record_vector3_list();
				const NFVector3 xPropertyValue = pRecord->GetVector3(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector3* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
					pVec->set_z(xPropertyValue.Z());
				}
			}
			break;
			default:
				break;
			}
		}
	}

	return true;
}

bool NFCCommonRedisModule::ConvertPBToRecord(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase * pRecordData)
{
	for (int iStuct = 0; iStuct < pRecordData->row_struct_size(); iStuct++)
	{
		const NFMsg::RecordAddRowStruct& xAddRowStruct = pRecordData->row_struct(iStuct);

		const int nCommonRow = xAddRowStruct.row();
		pRecord->SetUsed(nCommonRow, true);
		pRecord->PreAllocMemoryForRow(nCommonRow);

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
			const std::string& xPropertyValue = xPropertyData.data();

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

		for (int i = 0; i < xAddRowStruct.record_vector2_list_size(); i++)
		{
			const NFMsg::RecordVector2& xPropertyData = xAddRowStruct.record_vector2_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector2 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector2(nRow, nCol, v);
		}

		for (int i = 0; i < xAddRowStruct.record_vector3_list_size(); i++)
		{
			const NFMsg::RecordVector3& xPropertyData = xAddRowStruct.record_vector3_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector3 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector3(nRow, nCol, v);
		}
	}

	return false;
}
