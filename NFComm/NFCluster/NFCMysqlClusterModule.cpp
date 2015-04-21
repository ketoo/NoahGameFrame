// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <algorithm>
#include "NFCMysqlDriver.h"
#include "NFCMysqlClusterModule.h"

NFCMysqlClusterModule::NFCMysqlClusterModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCMysqlClusterModule::~NFCMysqlClusterModule()
{

}

bool NFCMysqlClusterModule::Init()
{
//     m_pShareMemoryModule = dynamic_cast<NFIShareMemoryModule*>(pPluginManager->FindModule("NFCShareMemoryModule"));
// 
//     otl_connect::otl_initialize();
// 
//     bool bLoad = mConfig.Load(mstrDataBaseConfigFile);
// 
//     mstrUID = mConfig["UID"].str();
//     mstrPWD = mConfig["PWD"].str();
//     mstrDSN = mConfig["DSN"].str();
//     mstrMasterName = mConfig["MST"].str();
// 
//     mstrAccountTableName = mConfig["AccountTBL"].str();
//     mstrPlayerTableName = mConfig["RoleTBL"].str();
//     mstrGlobalTableName = mConfig["GlobalTBL"].str();
// 
// 
//     m_pDataBaseDriver->OTLConnect(mstrUID, mstrPWD, mstrDSN, motlConnect);

    return true;
}

bool NFCMysqlClusterModule::Shut()
{
    //m_pDataBaseDriver->OTLDisconnect(motlConnect);

    return true;
}

bool NFCMysqlClusterModule::AfterInit()
{

    return true;
}

bool NFCMysqlClusterModule::Execute(const float fLasFrametime, const float fStartedTime)
{


    return true;
}

bool NFCMysqlClusterModule::Updata( const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec )
{
	return false;
}

bool NFCMysqlClusterModule::Query( const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec )
{
	return false;
}

bool NFCMysqlClusterModule::Delete( const std::string& strKey )
{
	return false;
}

bool NFCMysqlClusterModule::Exists( const std::string& strKey, bool& bExit )
{
return false;
}
