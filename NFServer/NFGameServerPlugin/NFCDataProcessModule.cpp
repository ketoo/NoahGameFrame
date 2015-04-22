// -------------------------------------------------------------------------
//    @FileName      :    NFCDataProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             ：    2013-10-03
//    @Module           :    NFCDataProcessModule
//
// -------------------------------------------------------------------------


#include "NFCDataProcessModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#include "NFComm/NFMessageDefine/NFMsgBase.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCDataProcessModule::Init()
{
	m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
	m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
	m_pClusterSQLModule = dynamic_cast<NFIClusterModule*>( pPluginManager->FindModule( "NFCMysqlClusterModule" ) );
	m_pGameLogicModule = dynamic_cast<NFIGameLogicModule*>( pPluginManager->FindModule( "NFCGameLogicModule" ) );
	m_pUUIDModule = dynamic_cast<NFIUUIDModule*>( pPluginManager->FindModule( "NFCUUIDModule" ) );


	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pClusterSQLModule);
	assert(NULL != m_pGameLogicModule);
	assert(NULL != m_pUUIDModule);


	m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCDataProcessModule::OnObjectClassEvent );

	return true;
}


bool NFCDataProcessModule::Shut()
{
	return true;
}

bool NFCDataProcessModule::Execute( const float fLasFrametime, const float fStartedTime )
{

	return true;
}

bool NFCDataProcessModule::AfterInit()
{


	return true;
}

int NFCDataProcessModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
	//非完整NF架构目前不使用NOSQL
	return 0;

	if ( strClassName == "Player" )
	{
		if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
		{
			SaveDataToNoSql( self );
		}
		else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
		{
			LoadDataFormNoSql( self );
		}
	}

	return 0;
}

const bool NFCDataProcessModule::LoadDataFormNoSql( const NFIDENTID& self )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject )
	{
		NF_SHARE_PTR<NFIPropertyManager> pProManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

		std::vector<std::string> vFieldVec;
		std::vector<std::string> vValueVec;
		vFieldVec.push_back("Property");
		vFieldVec.push_back("Record");

		if(!m_pClusterSQLModule->Query(self.ToString(), vFieldVec, vValueVec)
			|| vFieldVec.size() != vValueVec.size()
			|| vFieldVec.size()  < 2)
		{
			return false;
		}

		NFMsg::PlayerPropertyBase xPropertyList;
		if(!xPropertyList.ParseFromString(vValueVec[0]))
		{
			return false;
		}

		NFMsg::PlayerRecordList xRecordList;
		if(!xRecordList.ParseFromString(vValueVec[1]))
		{
			return false;
		}


		for (int i = 0; i < xPropertyList.property_int_list_size(); i++)
		{
			const NFMsg::PropertyInt& xPropertyInt = xPropertyList.property_int_list(i);
			const std::string& strName = xPropertyInt.property_name();
			int nData = xPropertyInt.data();

			NFIDataList::TData varData;
			varData.nType = TDATA_INT;
			varData.variantData = (NFINT64)nData;
			pProManager->SetProperty(strName, varData);
		}

		for (int i = 0; i < xPropertyList.property_float_list_size(); i++)
		{
			const NFMsg::PropertyFloat& xPropertyFloat = xPropertyList.property_float_list(i);
			const std::string& strName = xPropertyFloat.property_name();
			float fData = xPropertyFloat.data();

			NFIDataList::TData varData;
			varData.nType = TDATA_FLOAT;
			varData.variantData = fData;
			pProManager->SetProperty(strName, varData);
		}

		for (int i = 0; i < xPropertyList.property_string_list_size(); i++)
		{
			const NFMsg::PropertyString& xPropertyString = xPropertyList.property_string_list(i);
			const std::string& strName = xPropertyString.property_name();
			const std::string& strData = xPropertyString.data();

			NFIDataList::TData varData;
			varData.nType = TDATA_STRING;
			varData.variantData = strData;
			pProManager->SetProperty(strName, varData);
		}

		for (int i = 0; i < xPropertyList.property_object_list_size(); i++)
		{
			const NFMsg::PropertyObject& xPropertyObject = xPropertyList.property_object_list(i);
			const std::string& strName = xPropertyObject.property_name();
			NFIDENTID nData = NFINetModule::PBToNF(xPropertyObject.data());

			NFIDataList::TData varData;
			varData.nType = TDATA_OBJECT;
			varData.variantData = nData;
			pProManager->SetProperty(strName, varData);
		}

		////////////////////////////
		for (int i = 0; i < xRecordList.record_list_size(); ++i)
		{
			const NFMsg::PlayerRecordBase& xRecordData = xRecordList.record_list(i);
			const std::string& strRecordName = xRecordData.record_name();
			NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->GetElement(strRecordName);

			for (int j = 0; j < xRecordData.record_int_list_size(); j++)
			{
				const NFMsg::RecordInt& xRecordInt = xRecordData.record_int_list(j);
				const int nRow = xRecordInt.row();
				const int nCol = xRecordInt.col();
				const NFINT64 nData = xRecordInt.data();

				xRecord->SetUsed(nRow, true);
				xRecord->SetInt(nRow, nCol, nData);
			}

			for (int j = 0; j < xRecordData.record_float_list_size(); j++)
			{
				const NFMsg::RecordFloat& xRecordFloat = xRecordData.record_float_list(j);
				const int nRow = xRecordFloat.row();
				const int nCol = xRecordFloat.col();
				const float fData = xRecordFloat.data();

				xRecord->SetUsed(nRow, true);
				xRecord->SetFloat(nRow, nCol, fData);
			}

			for (int j = 0; j < xRecordData.record_string_list_size(); j++)
			{
				const NFMsg::RecordString& xRecordString = xRecordData.record_string_list(j);
				const int nRow = xRecordString.row();
				const int nCol = xRecordString.col();
				const std::string& strData = xRecordString.data();

				xRecord->SetUsed(nRow, true);
				xRecord->SetString(nRow, nCol, strData.c_str());
			}

			for (int j = 0; j < xRecordData.record_object_list_size(); j++)
			{
				const NFMsg::RecordObject& xRecordObject = xRecordData.record_object_list(j);
				const int nRow = xRecordObject.row();
				const int nCol = xRecordObject.col();
				const NFIDENTID xObjectID = NFINetModule::PBToNF(xRecordObject.data());

				xRecord->SetUsed(nRow, true);
				xRecord->SetObject(nRow, nCol, xObjectID);
			}
		}		
	}

	return true;
}

const bool NFCDataProcessModule::SaveDataToNoSql(const NFIDENTID& self, bool bOffline/* = false*/)
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject.get() )
	{
		NF_SHARE_PTR<NFIPropertyManager> pProManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

		NFMsg::PlayerPropertyBase xPropertyList;
		NFMsg::PlayerRecordList xRecordList;

		std::string strName;
		NF_SHARE_PTR<NFIProperty> xProperty = pProManager->First(strName);
		while (xProperty)
		{
			if (!xProperty->GetSave())
			{
				continue;
			}

			switch (xProperty->GetType())
			{
			case TDATA_INT:
				{
					NFMsg::PropertyInt* xPropertyInt = xPropertyList.add_property_int_list();
					xPropertyInt->set_property_name(strName);
					xPropertyInt->set_data(xProperty->GetInt());
				}
				break;
			case TDATA_FLOAT:
				{
					NFMsg::PropertyFloat* xPropertyFloat = xPropertyList.add_property_float_list();
					xPropertyFloat->set_property_name(strName);
					xPropertyFloat->set_data(xProperty->GetFloat());
				}
				break;
			case TDATA_STRING:
				{
					NFMsg::PropertyString* xPropertyString = xPropertyList.add_property_string_list();
					xPropertyString->set_property_name(strName);
					xPropertyString->set_data(xProperty->GetString());
				}
				break;
			case TDATA_OBJECT:
				{
					NFMsg::PropertyObject* xPropertyObject = xPropertyList.add_property_object_list();
					xPropertyObject->set_property_name(strName);
					*xPropertyObject->mutable_data() = NFINetModule::NFToPB(xProperty->GetObject());
				}
				break;
			default:
				break;
			}


			strName.clear();
			xProperty = pProManager->Next(strName);
		}


		NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First(strName);
		while (xRecord)
		{
			if (!xRecord->GetSave())
			{
				continue;
			}

			NFMsg::PlayerRecordBase* xRecordData = xRecordList.add_record_list();
			xRecordData->set_record_name(strName);

			for (int i = 0; i < xRecord->GetRows(); ++i)
			{
				if(xRecord->IsUsed(i))
				{
					for (int j = 0; j < xRecord->GetCols(); ++j)
					{
						switch (xRecord->GetColType(j))
						{
						case TDATA_INT:
							{
								NFMsg::RecordInt* pRecordInt = xRecordData->add_record_int_list();
								pRecordInt->set_row(i);
								pRecordInt->set_col(j);
								pRecordInt->set_data(xRecord->GetInt(i, j));
							}
							break;
						case TDATA_FLOAT:
							{
								NFMsg::RecordFloat* xRecordFloat = xRecordData->add_record_float_list();
								xRecordFloat->set_row(i);
								xRecordFloat->set_col(j);
								xRecordFloat->set_data(xRecord->GetFloat(i, j));
							}
							break;
						case TDATA_STRING:
							{
								NFMsg::RecordString* xRecordString = xRecordData->add_record_string_list();
								xRecordString->set_row(i);
								xRecordString->set_col(j);
								xRecordString->set_data(xRecord->GetString(i, j));
							}
							break;
						case TDATA_OBJECT:
							{
								NFMsg::RecordObject* xRecordObejct = xRecordData->add_record_object_list();
								xRecordObejct->set_row(i);
								xRecordObejct->set_col(j);
								*xRecordObejct->mutable_data() = NFINetModule::NFToPB(xRecord->GetObject(i, j));
							}
							break;
						default:
							break;
						}
					}
				}

			}


			strName.clear();
			xRecord = pRecordManager->Next(strName);
		}

		std::string strPropertyList;
		if(!xPropertyList.SerializeToString(&strPropertyList))
		{
			return false;
		}

		std::string strRecordList;
		if(!xRecordList.SerializeToString(&strRecordList))
		{
			return false;
		}

		std::vector<std::string> vFieldVec;
		std::vector<std::string> vValueVec;
		vFieldVec.push_back("Property");
		vFieldVec.push_back("Record");
		vValueVec.push_back(strPropertyList);
		vValueVec.push_back(strRecordList);


		if(!m_pClusterSQLModule->Updata(self.ToString(), vFieldVec, vValueVec))
		{
			return false;
		}

		return false;
	}
}

const NFIDENTID NFCDataProcessModule::CreateRole( const std::string& strAccount, const std::string& strName, const int nJob, const int nSex )
{
	bool bExit = false;
	if (!m_pClusterSQLModule->Exists(strAccount, bExit)
		|| !bExit)
	{
		return NFIDENTID();
	}

	bExit = false;
	if (!m_pClusterSQLModule->Exists(strName, bExit)
		|| !bExit)
	{
		return NFIDENTID();
	}

	//不存在此角色名,看帐号下面是否有角色

	std::vector<std::string> vFieldVec;
	std::vector<std::string> vValueVec;
	if(!m_pClusterSQLModule->Query(strAccount, vFieldVec, vValueVec)
		|| vFieldVec.size() != vValueVec.size())
	{
		return NFIDENTID();
	}

	if (vValueVec[0].length() > 0)
	{
		//已经有角色了
		return NFIDENTID();
	}

	NFIDENTID xID = m_pUUIDModule->CreateGUID();

	vFieldVec.clear();
	vValueVec.clear();


	NFMsg::PlayerPropertyBase xPropertyList;
	NFMsg::PropertyString* pPropertyStr = xPropertyList.add_property_string_list();
	pPropertyStr->set_property_name("Name");
	pPropertyStr->set_data(strName);

	NFMsg::PropertyInt* pPropertyInt = xPropertyList.add_property_int_list();
	pPropertyInt->set_property_name("Job");
	pPropertyInt->set_data(nJob);

	pPropertyInt = xPropertyList.add_property_int_list();
	pPropertyInt->set_property_name("Sex");
	pPropertyInt->set_data(nSex);

	pPropertyInt = xPropertyList.add_property_int_list();
	pPropertyInt->set_property_name("Level");
	pPropertyInt->set_data(1);

	std::string strData;
	xPropertyList.SerializeToString(&strData);

	vFieldVec.push_back("Property");
	vValueVec.push_back(strData);

	if(!m_pClusterSQLModule->Updata(xID.ToString(), vFieldVec, vValueVec))
	{
		return NFIDENTID();
	}

	vFieldVec.clear();
	vValueVec.clear();
	vFieldVec.push_back("Property");
	vFieldVec.push_back(xID.ToString());
	if(!m_pClusterSQLModule->Updata(strAccount, vFieldVec, vValueVec))
	{
		return NFIDENTID();
	}

	return xID;
}

const bool NFCDataProcessModule::DeleteRole( const std::string& strAccount, const NFIDENTID xID )
{
	bool bExit = false;
	if (!m_pClusterSQLModule->Exists(strAccount, bExit)
		|| !bExit)
	{
		return false;
	}

	bExit = false;
	if (!m_pClusterSQLModule->Exists(xID.ToString(), bExit)
		|| !bExit)
	{
		return false;
	}

	if (!m_pClusterSQLModule->Delete(xID.ToString()))
	{
		return false;
	}

	return true;
}

const bool NFCDataProcessModule::GetChar( const std::string& strAccount, std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVeec )
{
	bool bExit = false;
	if (!m_pClusterSQLModule->Exists(strAccount, bExit)
		|| !bExit)
	{
		return false;
	}

	std::vector<std::string> vFieldVec;
	std::vector<std::string> vValueVec;
	vFieldVec.push_back("Property");

	if(!m_pClusterSQLModule->Query(strAccount, vFieldVec, vValueVec)
		|| vFieldVec.size() != vValueVec.size())
	{
		return false;
	}

	const std::string stRolerID = vValueVec[0];

	vFieldVec.clear();
	vValueVec.clear();

	if(!m_pClusterSQLModule->Query(stRolerID, vFieldVec, vValueVec)
		|| vFieldVec.size() != vValueVec.size())
	{
		return false;
	}

	xFieldVec = vFieldVec;
	xValueVeec = vValueVec;

	return true;
}
