// -------------------------------------------------------------------------
//    @FileName      :    NFCDataProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             ：    2013-10-03
//    @Module           :    NFCDataProcessModule
//
// -------------------------------------------------------------------------


#include "NFCDataProcessModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

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
		if(!m_pClusterSQLModule->Query(self.ToString(), vFieldVec, vValueVec)
			|| vFieldVec.size() != vValueVec.size()
			|| vFieldVec.size()  < 2)
		{
			return false;
		}

		for (int i = 0; i < vFieldVec.size(); ++i)
		{
			const std::string& strField = vFieldVec[i];
			const std::string& strValue = vFieldVec[i];
			if (strField == "Property")
			{

			}
			else if (strField == "Record")
			{
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
		


    }

    return false;
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

	vFieldVec.push_back("Name");
	vValueVec.push_back(strName);

	vFieldVec.push_back("Job");
	vValueVec.push_back(boost::lexical_cast<std::string>(nJob));

	vFieldVec.push_back("Sex");
	vValueVec.push_back(boost::lexical_cast<std::string>(nSex));

	vFieldVec.push_back("Level");
	vValueVec.push_back(boost::lexical_cast<std::string>(1));

	if(!m_pClusterSQLModule->Updata(xID.ToString(), vFieldVec, vValueVec))
	{
		return NFIDENTID();
	}

	vFieldVec.clear();
	vValueVec.clear();
	vFieldVec.push_back("RoleID");
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
	vFieldVec.push_back("RoleID");

	if(!m_pClusterSQLModule->Query(strAccount, vFieldVec, vValueVec)
		|| vFieldVec.size() != vValueVec.size())
	{
		return false;
	}

	vFieldVec.clear();
	vValueVec.clear();

	const std::string& stRolerID = vValueVec[0];

	if(!m_pClusterSQLModule->Query(stRolerID, vFieldVec, vValueVec)
		|| vFieldVec.size() != vValueVec.size())
	{
		return false;
	}

	xFieldVec = vFieldVec;
	xValueVeec = vValueVec;

	return true;
}
