// -------------------------------------------------------------------------
//    @FileName			:    NFCPlayerMysqlModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPlayerMysqlModule
//
// -------------------------------------------------------------------------


#include "NFCPlayerMysqlModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCPlayerMysqlModule::Init()
{
	return true;
}


bool NFCPlayerMysqlModule::Shut()
{
	return true;
}

bool NFCPlayerMysqlModule::Execute()
{

	return true;
}

bool NFCPlayerMysqlModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pClusterSQLModule = pPluginManager->FindModule<NFIMysqlModule>( "NFCMysqlModule" );
	m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>( "NFCUUIDModule" );
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>( "NFCLogModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pAsyClusterSQLModule = pPluginManager->FindModule<NFIAsyMysqlModule>( "NFCAsyMysqlModule" );
	
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pClusterSQLModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pAsyClusterSQLModule);
	
    NF_SHARE_PTR<NFILogicClass> pLogicClass = m_pLogicClassModule->GetElement("SqlServer");
    if (nullptr == pLogicClass)
    {
        return false;
    }

    NFList<std::string>& xNameList = pLogicClass->GetConfigNameList();
    std::string strConfigName; 
    if (!xNameList.Get(0, strConfigName))
    {
        return false;
    }

    const int nID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
    const std::string& mstrSQLIP = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlIP");
    const int mnSQLPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "SqlPort");
    const std::string& mstrSQLName = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlName");
    const std::string& mstrSQLUser = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlUser");
    const std::string& mstrSQLPWD = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlPwd");

    bool bConnect = m_pClusterSQLModule->AddMysqlServer(nID, "", mstrSQLIP, mnSQLPort, mstrSQLName, mstrSQLUser, mstrSQLPWD);

    if (!m_pAsyClusterSQLModule->StartActorPool(10))
    {
        return false;
    }

    m_pAsyClusterSQLModule->AddMysqlServer(nID, "", mstrSQLIP, mnSQLPort, mstrSQLName, mstrSQLUser, mstrSQLPWD);

    RegisterAutoSave(NFrame::Player::ThisName());

	return true;
}

int NFCPlayerMysqlModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
	if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
	{
		OnOffline(self);
		SaveDataToSql(self);
        //SaveDataToSqlAsy(self);
	}
	else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
	{
		OnOnline(self);
		AttachData(self);

	}

	return 0;
}

const NFGUID NFCPlayerMysqlModule::CreateRole( const std::string& strAccount, const std::string& strName, const int nRace, const int nJob, const int nSex )
{
	bool bExit = false;
	if (!m_pClusterSQLModule->Exists(mstrAccountTable, strAccount, bExit)
		|| !bExit)
	{
		return NULL_OBJECT;
	}    

	//不存在此角色名,看帐号下面是否有角色
	std::vector<std::string> vFieldVec;
    vFieldVec.push_back("RoleID");

	std::vector<std::string> vValueVec;
	if(!m_pClusterSQLModule->Query(mstrAccountTable, strAccount, vFieldVec, vValueVec)
		|| vFieldVec.size() != vValueVec.size())
	{
		return NULL_OBJECT;
	}

	if (vValueVec[0].length() > 0)
	{
		//已经有角色了
		return NULL_OBJECT;
	}

	NFGUID xID = m_pUUIDModule->CreateGUID();

	vFieldVec.clear();
	vValueVec.clear();

    vFieldVec.push_back("Name");
    vValueVec.push_back(strName);

    vFieldVec.push_back("Job");
    vValueVec.push_back(lexical_cast<std::string>(nJob));

	vFieldVec.push_back("Race");
	vValueVec.push_back(lexical_cast<std::string>(nRace));

    vFieldVec.push_back("Sex");
    vValueVec.push_back(lexical_cast<std::string>(nSex));

    vFieldVec.push_back("Level");
    vValueVec.push_back("0");

	if(!m_pClusterSQLModule->Updata(mstrRoleTable, xID.ToString(), vFieldVec, vValueVec))
	{
		return NULL_OBJECT;
	}

	vFieldVec.clear();
	vValueVec.clear();
	vFieldVec.push_back("RoleID");
	vValueVec.push_back(xID.ToString());
	if(!m_pClusterSQLModule->Updata(mstrAccountTable, strAccount, vFieldVec, vValueVec))
	{
		return NULL_OBJECT;
	}

	return xID;
}

const bool NFCPlayerMysqlModule::DeleteRole( const std::string& strAccount, const NFGUID xID )
{
	bool bExit = false;
	if (!m_pClusterSQLModule->Exists(mstrAccountTable, strAccount, bExit)
		|| !bExit)
	{
		return false;
	}

	bExit = false;
	if (!m_pClusterSQLModule->Exists(mstrRoleTable, xID.ToString(), bExit)
		|| !bExit)
	{
		return false;
	}

	if (!m_pClusterSQLModule->Delete(mstrRoleTable, xID.ToString()))
	{
		return false;
	}

	return true;
}

const NFGUID NFCPlayerMysqlModule::GetChar( const std::string& strAccount, const std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVec )
{
	bool bExit = false;
	if (!m_pClusterSQLModule->Exists(mstrAccountTable, strAccount, bExit)
		|| !bExit)
	{
		return NULL_OBJECT;
	}
    std::vector<std::string> vFieldAccountVector;
    std::vector<std::string> vValueAccountVector;

	vFieldAccountVector.push_back("RoleID");

	if(!m_pClusterSQLModule->Query(mstrAccountTable, strAccount, vFieldAccountVector, vValueAccountVector)
		|| vFieldAccountVector.size() != vValueAccountVector.size())
	{
		return NULL_OBJECT;
	}

	const std::string& stRolerID = vValueAccountVector[0];

    //////////////////////////////////////////////////////////////////////////    
	if(!m_pClusterSQLModule->Query(mstrRoleTable, stRolerID, xFieldVec, xValueVec)
		|| xFieldVec.size() != xValueVec.size())
	{
		return NULL_OBJECT;
	}

    NFGUID ident;
    if (!ident.FromString(stRolerID))
    {
        return NULL_OBJECT;
    }

	return ident;
}

void NFCPlayerMysqlModule::OnOnline( const NFGUID& self )
{
    const int nGateID = m_pKernelModule->GetPropertyInt(self, "GateID");
    const int nGameID = m_pKernelModule->GetPropertyInt(self, "GameID");

    std::vector<std::string> xFieldVec;
    std::vector<std::string> vValueVec;

    xFieldVec.push_back("GateID");
    xFieldVec.push_back("GameID");

    vValueVec.push_back(lexical_cast<std::string> (nGateID));
    vValueVec.push_back(lexical_cast<std::string> (nGameID));

    m_pClusterSQLModule->Updata(mstrRoleTable, self.ToString(), xFieldVec, vValueVec);
}

void NFCPlayerMysqlModule::OnOffline( const NFGUID& self )
{
    const int nGateID = 0;
    const int nGameID = 0;

    std::vector<std::string> xFieldVec;
    std::vector<std::string> vValueVec;

    xFieldVec.push_back("GateID");
    xFieldVec.push_back("GameID");

    vValueVec.push_back(lexical_cast<std::string> (nGateID));
    vValueVec.push_back(lexical_cast<std::string> (nGameID));

    m_pClusterSQLModule->Updata(mstrRoleTable, self.ToString(), xFieldVec, vValueVec);
}


const bool NFCPlayerMysqlModule::LoadDataFormSql( const NFGUID& self , const std::string& strClassName)
{
	NF_SHARE_PTR<NFIPropertyManager> pProManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);
	NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);

	if (!pProManager || !pRecordManager)
	{
		return false;
	}

	std::vector<std::string> vFieldVec;
	std::vector<std::string> vValueVec;

	int nIndex = 0;
	std::string strName;
	std::map<std::string, int> xDataIndex;

	//witch Property Need Load
	NF_SHARE_PTR<NFIProperty> xProperty = pProManager->First(strName);
	while (xProperty)
	{
		if (xProperty->GetSave())
		{
			vFieldVec.push_back(strName);
			xDataIndex.insert(std::make_pair(strName, nIndex));
			nIndex ++;
		}

		strName.clear();
		xProperty = pProManager->Next(strName);
	}

	//witch Record Need Load
	NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First(strName);
	while (xRecord)
	{
		if (xRecord->GetSave())
		{
			vFieldVec.push_back(strName);
			xDataIndex.insert(std::make_pair(strName, nIndex));
			nIndex ++;
		}

		strName.clear();
		xRecord = pRecordManager->Next(strName);
	}


	if(!m_pClusterSQLModule->Query(strClassName, self.ToString(), vFieldVec, vValueVec))
	{
		return false;
	}

	if (vFieldVec.size() != vValueVec.size())
	{
		return false;
	}

	NF_SHARE_PTR<NFMapEx<std::string, std::string> > pSelf = mtObjectCache.GetElement(self);
	if (!pSelf)
	{
		pSelf = NF_SHARE_PTR< NFMapEx<std::string, std::string> > (NF_NEW NFMapEx<std::string, std::string>());
		if (!mtObjectCache.AddElement(self, pSelf))
		{
			return false;
		}
	}

	for (int i = 0; i< vValueVec.size(); i++)
	{
		const std::string& strName = vFieldVec[i];
		const std::string& strValue = vValueVec[i];

		pSelf->AddElement(strName, NF_SHARE_PTR<std::string>(NF_NEW std::string(strValue)));
	}

	return true;
}

const bool NFCPlayerMysqlModule::SaveDataToSql( const NFGUID& self )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject.get() )
	{
		NF_SHARE_PTR<NFIPropertyManager> pProManager = pObject->GetPropertyManager();
		NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

		std::vector<std::string> vFieldVec;
		std::vector<std::string> vValueVec;

		//witch property to save
		std::string strName;
		NF_SHARE_PTR<NFIProperty> xProperty = pProManager->First(strName);
		while (xProperty)
		{
			if (xProperty->GetSave())
			{
				vFieldVec.push_back(strName);
				vValueVec.push_back(xProperty->ToString());
			}

			strName.clear();
			xProperty = pProManager->Next(strName);
		}

		//witch Record to save
		NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First(strName);
		while (xRecord)
		{
			if (xRecord->GetSave())
			{
				NFMsg::PlayerRecordBase xRecordData;
				xRecordData.set_record_name(strName);

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
									NFMsg::RecordInt* pRecordInt = xRecordData.add_record_int_list();
									pRecordInt->set_row(i);
									pRecordInt->set_col(j);
									pRecordInt->set_data(xRecord->GetInt(i, j));
								}
								break;
							case TDATA_FLOAT:
								{
									NFMsg::RecordFloat* xRecordFloat = xRecordData.add_record_float_list();
									xRecordFloat->set_row(i);
									xRecordFloat->set_col(j);
									xRecordFloat->set_data(xRecord->GetFloat(i, j));
								}
								break;
							case TDATA_STRING:
								{
									NFMsg::RecordString* xRecordString = xRecordData.add_record_string_list();
									xRecordString->set_row(i);
									xRecordString->set_col(j);
									xRecordString->set_data(xRecord->GetString(i, j));
								}
								break;
							case TDATA_OBJECT:
								{
									NFMsg::RecordObject* xRecordObejct = xRecordData.add_record_object_list();
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

				std::string strRecordValue;
				if(xRecordData.SerializeToString(&strRecordValue))
				{
					vFieldVec.push_back(strName);
					vValueVec.push_back(strRecordValue);
				}
			}

			strName.clear();
			xRecord = pRecordManager->Next(strName);
		}

		const std::string& strClass = m_pKernelModule->GetPropertyString(self, "ClassName");
		if(!m_pClusterSQLModule->Updata(strClass, self.ToString(), vFieldVec, vValueVec))
		{
			return false;
		}

		return true;
	}

	return false;
}

const bool NFCPlayerMysqlModule::AttachData( const NFGUID& self )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
	if (!pObject)
	{
		return false;
	}

	NF_SHARE_PTR<NFMapEx<std::string, std::string> > pSelfData = mtObjectCache.GetElement(self);
	if (!pSelfData)
	{
		return false;
	}

	NF_SHARE_PTR<NFIPropertyManager> pProManager = pObject->GetPropertyManager();
	NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

	//assign property value 
	NF_SHARE_PTR<NFIProperty> xProperty = pProManager->First(strName);
	while (xProperty)
	{
		if (xProperty->GetSave())
		{
			NF_SHARE_PTR<std::string> pValue = pSelfData->GetElement(strName);
			if (pValue)
			{
				const std::string& strData = *pValue;
				if (!xProperty->FromString(strData))
				{
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Load Property fail " + strName , ": " + strData, __FUNCTION__, __LINE__);
				}
				else
				{
					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Load Property success " + strName, ": " + strData, __FUNCTION__, __LINE__);
				}
			}
		}

		strName.clear();
		xProperty = pProManager->Next(strName);
	}

	//assign record value
	NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First(strName);
	while (xRecord)
	{
		if (xRecord->GetSave())
		{
			NF_SHARE_PTR<std::string> pValue = pSelfData->GetElement(strName);
			if (pValue)
			{
				const std::string& strData = *pValue;
				NFMsg::PlayerRecordBase xRecordData;
				if (xRecordData.ParseFromString(strData) && xRecordData.record_name() == strName)
				{
					ConvertPBToRecord(xRecordData, xRecord);
				}
			}
		}

		strName.clear();
		xRecord = pRecordManager->Next(strName);
	}

	mtObjectCache.RemoveElement(self);
	return true;
}

bool NFCPlayerMysqlModule::RegisterAutoSave( const std::string& strClassName )
{
	return m_pKernelModule->AddClassCallBack( strClassName, this, &NFCPlayerMysqlModule::OnObjectClassEvent );
}

const bool NFCPlayerMysqlModule::ConvertPBToRecord(const NFMsg::PlayerRecordBase& xRecordData, NF_SHARE_PTR<NFIRecord> xRecord)
{
	const std::string& strRecordName = xRecordData.record_name();
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
		const NFGUID xObjectID = NFINetModule::PBToNF(xRecordObject.data());

		xRecord->SetUsed(nRow, true);
		xRecord->SetObject(nRow, nCol, xObjectID);
	}

	return true;
}

const bool NFCPlayerMysqlModule::LoadDataFormSqlAsy( const NFGUID& self , const std::string& strClassName, const LOADDATA_RETURN_FUNCTOR& xFun, const std::string& strUseData)
{
    NF_SHARE_PTR<NFIPropertyManager> pProManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);

    if (!pProManager || !pRecordManager)
    {
        return false;
    }

    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    int nIndex = 0;
    std::string strName;
    std::map<std::string, int> xDataIndex;

    //witch Property Need Load
    NF_SHARE_PTR<NFIProperty> xProperty = pProManager->First(strName);
    while (xProperty)
    {
        if (xProperty->GetSave())
        {
            vFieldVec.push_back(strName);
            xDataIndex.insert(std::make_pair(strName, nIndex));
            nIndex ++;
        }

        strName.clear();
        xProperty = pProManager->Next(strName);
    }

    //witch Record Need Load
    NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First(strName);
    while (xRecord)
    {
        if (xRecord->GetSave())
        {
            vFieldVec.push_back(strName);
            xDataIndex.insert(std::make_pair(strName, nIndex));
            nIndex ++;
        }

        strName.clear();
        xRecord = pRecordManager->Next(strName);
    }

    mnLoadCount ++;
    std::string strData = lexical_cast<std::string>(mnLoadCount);
    
    NF_SHARE_PTR<LoadData> pData = NF_SHARE_PTR<LoadData> (NF_NEW LoadData());

    pData->mFunc = xFun;
    pData->strUseData = strData;

    if (mmLoadlisReq.AddElement(mnLoadCount, pData))
    {
        m_pAsyClusterSQLModule->Query(self, strClassName, self.ToString(), vFieldVec, this, &NFCPlayerMysqlModule::LoadDataFormSqlAsySucess, strData);
    }
   
    return true;
}
void NFCPlayerMysqlModule::LoadDataFormSqlAsySucess( const NFGUID& self, const int nResult, const std::vector<std::string>& vFieldVec, const std::vector<std::string>& vValueVec, const std::string& strUserData)
{
    NFINT64 nID = 0;
    if (!NF_StrTo(strUserData, nID))
    {
        return;
    }

    NF_SHARE_PTR<LoadData> pData = mmLoadlisReq.GetElement(nID);
    if (pData.get())
    {
        if (nResult < 0 )
        {
            mmLoadlisReq.RemoveElement(nID);
            return;
        }

        if (vFieldVec.size() != vValueVec.size())
        {
            mmLoadlisReq.RemoveElement(nID);
            return;
        }

        NF_SHARE_PTR<NFMapEx<std::string, std::string> > pSelf = mtObjectCache.GetElement(self);
        if (!pSelf)
        {
            pSelf = NF_SHARE_PTR< NFMapEx<std::string, std::string> > (NF_NEW NFMapEx<std::string, std::string>());
            if (!mtObjectCache.AddElement(self, pSelf))
            {
                mmLoadlisReq.RemoveElement(nID);
                return;
            }
        }

        for (int i = 0; i< vValueVec.size(); i++)
        {
            const std::string& strName = vFieldVec[i];
            const std::string& strValue = vValueVec[i];

            pSelf->AddElement(strName, NF_SHARE_PTR<std::string>(NF_NEW std::string(strValue)));
        }

        pData->mFunc(self, nResult, strUserData);
        mmLoadlisReq.RemoveElement(nID);
        return;
    }

}

const bool NFCPlayerMysqlModule::SaveDataToSqlAsy( const NFGUID& self )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject.get() )
    {
        NF_SHARE_PTR<NFIPropertyManager> pProManager = pObject->GetPropertyManager();
        NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

        std::vector<std::string> vFieldVec;
        std::vector<std::string> vValueVec;

        //witch property to save
        std::string strName;
        NF_SHARE_PTR<NFIProperty> xProperty = pProManager->First(strName);
        while (xProperty)
        {
            if (xProperty->GetSave())
            {
                vFieldVec.push_back(strName);
                vValueVec.push_back(xProperty->ToString());
            }

            strName.clear();
            xProperty = pProManager->Next(strName);
        }

        //witch Record to save
        NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First(strName);
        while (xRecord)
        {
            if (xRecord->GetSave())
            {
                NFMsg::PlayerRecordBase xRecordData;
                xRecordData.set_record_name(strName);

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
                                    NFMsg::RecordInt* pRecordInt = xRecordData.add_record_int_list();
                                    pRecordInt->set_row(i);
                                    pRecordInt->set_col(j);
                                    pRecordInt->set_data(xRecord->GetInt(i, j));
                                }
                                break;
                            case TDATA_FLOAT:
                                {
                                    NFMsg::RecordFloat* xRecordFloat = xRecordData.add_record_float_list();
                                    xRecordFloat->set_row(i);
                                    xRecordFloat->set_col(j);
                                    xRecordFloat->set_data(xRecord->GetFloat(i, j));
                                }
                                break;
                            case TDATA_STRING:
                                {
                                    NFMsg::RecordString* xRecordString = xRecordData.add_record_string_list();
                                    xRecordString->set_row(i);
                                    xRecordString->set_col(j);
                                    xRecordString->set_data(xRecord->GetString(i, j));
                                }
                                break;
                            case TDATA_OBJECT:
                                {
                                    NFMsg::RecordObject* xRecordObejct = xRecordData.add_record_object_list();
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

                std::string strRecordValue;
                if(xRecordData.SerializeToString(&strRecordValue))
                {
                    vFieldVec.push_back(strName);
                    vValueVec.push_back(strRecordValue);
                }
            }

            strName.clear();
            xRecord = pRecordManager->Next(strName);
        }

        const std::string& strClass = m_pKernelModule->GetPropertyString(self, "ClassName");
        m_pAsyClusterSQLModule->Updata(self, strClass, self.ToString(), vFieldVec, vValueVec, this, &NFCPlayerMysqlModule::SaveDataToSqlAsySucess, "");
    }

    return false;
}

void NFCPlayerMysqlModule::SaveDataToSqlAsySucess( const NFGUID& self, const int nRet, const std::string&strUseData )
{
   //
}
