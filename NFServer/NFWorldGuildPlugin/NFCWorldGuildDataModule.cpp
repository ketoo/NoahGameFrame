// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildDataModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildDataModule
//
// -------------------------------------------------------------------------

#include "NFWorldGuildPlugin.h"
#include "NFCWorldGuildDataModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFRecordDefine.pb.h"

bool NFCWorldGuildDataModule::Init()
{
    return true;
}

bool NFCWorldGuildDataModule::Shut()
{
    return true;
}

bool NFCWorldGuildDataModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCWorldGuildDataModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pUUIDModule = dynamic_cast<NFIUUIDModule*>(pPluginManager->FindModule("NFCUUIDModule"));
    m_pClusterSQLModule = dynamic_cast<NFIClusterModule*>(pPluginManager->FindModule("NFCMysqlClusterModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pClusterSQLModule);

    m_pEventProcessModule->AddClassCallBack("Guild", this, &NFCWorldGuildDataModule::OnGuildClassEvent);

    m_pKernelModule->CreateContainer(mContainerID, "");

    return true;
}

bool NFCWorldGuildDataModule::ExitGuild( const NFIDENTID& self, const std::string& strName, bool& bExit)
{
    if (!m_pClusterSQLModule->Exists(mstrGuildNameTalble, strName, bExit))
    {
        return false;
    }

    return true;
}

void NFCWorldGuildDataModule::CheckLoadGuild( const NFIDENTID& self, const NFIDENTID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pObejct = m_pKernelModule->GetObject(xGuild);
    if (!pObejct.get())
    {
        if (LoadDataFormNoSql(xGuild))
        {
            m_pKernelModule->CreateObject(xGuild, 1, 0, "Guild", "", NFCDataList());
        }
    }
}

int NFCWorldGuildDataModule::OnGuildClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == "Guild" )
    {
        if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
        {
            SaveDataToNoSql(self);
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
        {
            LoadDataFormNoSql(self);
        }
    }

    return 0;
}


bool NFCWorldGuildDataModule::CanLoadMysqlData( const NFIDENTID& self )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    if(!m_pClusterSQLModule->Query(mstrGuildTalble, self.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    if (vFieldVec.size() != vValueVec.size())
    {
        return false;
    }

    return true;
}

const bool NFCWorldGuildDataModule::LoadDataFormNoSql( const NFIDENTID& self )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        NF_SHARE_PTR<NFIPropertyManager> pProManager = pObject->GetPropertyManager();
        NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

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


        if(!m_pClusterSQLModule->Query(mstrGuildTalble, self.ToString(), vFieldVec, vValueVec))
        {
            return false;
        }

        if (vFieldVec.size() != vValueVec.size())
        {
            return false;
        }

        //assign property value 
        xProperty = pProManager->First(strName);
        while (xProperty)
        {
            if (xProperty->GetSave())
            {
                std::map<std::string, int>::iterator it = xDataIndex.find(strName);
                if (it != xDataIndex.end())
                {
                    const int nIndex = it->second;
                    if (nIndex < vValueVec.size())
                    {
                        const std::string& strData = vValueVec[nIndex];
                        xProperty->FromString(strData);
                    }
                }
            }

            strName.clear();
            xProperty = pProManager->Next(strName);
        }

        //assign record value
        xRecord= pRecordManager->First(strName);
        while (xRecord)
        {
            if (xRecord->GetSave())
            {
                std::map<std::string, int>::iterator it = xDataIndex.find(strName);
                if (it != xDataIndex.end())
                {
                    const int nIndex = it->second;
                    if (nIndex < vValueVec.size())
                    {
                        const std::string& strData = vValueVec[nIndex];

                        NFMsg::PlayerRecordBase xRecordData;

                        if (xRecordData.ParseFromString(strData) && xRecordData.record_name() == strName)
                        {
                            ConvertPBToRecord(xRecordData, xRecord);
                        }
                    }
                }
            }

            strName.clear();
            xRecord = pRecordManager->Next(strName);
        }
    }

    return true;
}

const bool NFCWorldGuildDataModule::ConvertPBToRecord(const NFMsg::PlayerRecordBase& xRecordData, NF_SHARE_PTR<NFIRecord> xRecord)
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
        const NFIDENTID xObjectID = NFINetModule::PBToNF(xRecordObject.data());

        xRecord->SetUsed(nRow, true);
        xRecord->SetObject(nRow, nCol, xObjectID);
    }

    return true;
}

const bool NFCWorldGuildDataModule::SaveDataToNoSql(const NFIDENTID& self)
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
                if(!xRecordData.SerializeToString(&strRecordValue))
                {
                    vFieldVec.push_back(strName);
                    vValueVec.push_back(strRecordValue);
                }
            }

            strName.clear();
            xRecord = pRecordManager->Next(strName);
        }

        if(!m_pClusterSQLModule->Updata(mstrGuildTalble, self.ToString(), vFieldVec, vValueVec))
        {
            return false;
        }

        return true;
    }

    return true;
}

const NFIDENTID NFCWorldGuildDataModule::CreateGuild(const NFIDENTID& xPlayeID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine /*= 1*/, const int nPower/* = NFMsg::GUILD_POWER_TYPE_PRESIDENT*/)
{
    bool bExit = false;
    if (!m_pClusterSQLModule->Exists(mstrGuildNameTalble, strName, bExit))
    {
        return NFIDENTID();
    }

    if (bExit)
    {
        return NFIDENTID();
    }

    const NFIDENTID xGuidID = m_pUUIDModule->CreateGUID();

    //Guild
    std::vector<std::string> vGuildFieldVec;
    std::vector<std::string> vGuildValueVec;

    vGuildFieldVec.push_back("Name");
    vGuildValueVec.push_back(strName);

    vGuildFieldVec.push_back("GuildLevel");
    vGuildValueVec.push_back("1");

    vGuildFieldVec.push_back("PresidentID");
    vGuildValueVec.push_back(xPlayeID.ToString());

    vGuildFieldVec.push_back("PresidentName");
    vGuildValueVec.push_back(strRoleName);

    std::string strGuildMemberList;
    NFMsg::PlayerRecordBase xRecordData;
    xRecordData.set_record_name("GuildMemberList");

    NFMsg::RecordObject* pRecordGUID = xRecordData.add_record_object_list();
    pRecordGUID->set_row(0);
    pRecordGUID->set_col(NFMsg::GuildMemberList_GUID);
    *pRecordGUID->mutable_data() = NFINetModule::NFToPB(xPlayeID);

    NFMsg::RecordString* pRecordName = xRecordData.add_record_string_list();
    pRecordName->set_row(0);
    pRecordName->set_col(NFMsg::GuildEvent_Name);
    pRecordName->set_data(strRoleName);

    NFMsg::RecordInt* pRecordLevel = xRecordData.add_record_int_list();
    pRecordLevel->set_row(0);
    pRecordLevel->set_col(NFMsg::GuildEvent_Level);
    pRecordLevel->set_data(nLevel);

    NFMsg::RecordInt* pRecordJob = xRecordData.add_record_int_list();
    pRecordJob->set_row(0);
    pRecordJob->set_col(NFMsg::GuildEvent_Job);
    pRecordJob->set_data(nJob);

    NFMsg::RecordInt* pRecordDonation = xRecordData.add_record_int_list();
    pRecordDonation->set_row(0);
    pRecordDonation->set_col(NFMsg::GuildEvent_Donation);
    pRecordDonation->set_data(nDonation);

    NFMsg::RecordInt* pRecordVip = xRecordData.add_record_int_list();
    pRecordVip->set_row(0);
    pRecordVip->set_col(NFMsg::GuildEvent_VIP);
    pRecordVip->set_data(nVIP);

    NFMsg::RecordInt* pRecordOffLine = xRecordData.add_record_int_list();
    pRecordOffLine->set_row(0);
    pRecordOffLine->set_col(NFMsg::GuildEvent_Offline);
    pRecordOffLine->set_data(1);

    NFMsg::RecordInt* pRecordPower = xRecordData.add_record_int_list();
    pRecordPower->set_row(0);
    pRecordPower->set_col(NFMsg::GuildEvent_Power);
    pRecordPower->set_data(nPower);

    strGuildMemberList;
    if(!xRecordData.SerializeToString(&strGuildMemberList))
    {
        return NFIDENTID();
    }

    vGuildFieldVec.push_back("GuildMemberList");
    vGuildValueVec.push_back(strGuildMemberList);

    if (!m_pClusterSQLModule->Updata(mstrGuildTalble, xGuidID.ToString(), vGuildFieldVec, vGuildValueVec))
    {
        return NFIDENTID();
    }

    //Name
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("GuildID");
    vValueVec.push_back(xGuidID.ToString());

    m_pClusterSQLModule->Updata(mstrGuildNameTalble, strName, vFieldVec, vValueVec);

    return xGuidID;
}

const bool NFCWorldGuildDataModule::DeleteGuild(const NFIDENTID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pGuilD = m_pKernelModule->GetObject(xGuild);
    if (pGuilD.get())
    {
        m_pKernelModule->DestroyObject(xGuild);
    }

    bool bExit = false;
    if (!m_pClusterSQLModule->Exists(mstrGuildTalble, xGuild.ToString(), bExit)
        || !bExit)
    {
        return false;
    }    

    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;
    vFieldVec.push_back("Name");
    
    if (!m_pClusterSQLModule->Query(mstrGuildTalble, xGuild.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildName = vValueVec[0];
    bExit = false;

    if (!m_pClusterSQLModule->Exists(mstrGuildNameTalble, strGuildName, bExit)
        || !bExit)
    {
        return false;
    }

    if (!m_pClusterSQLModule->Delete(mstrGuildTalble, xGuild.ToString()))
    {
        return false;
    }

    return true;
}

NF_SHARE_PTR<NFIObject> NFCWorldGuildDataModule::GetGuild( const NFIDENTID& xGuild )
{
    CheckLoadGuild(NFIDENTID(), xGuild);

    return m_pKernelModule->GetObject(xGuild);
}

bool NFCWorldGuildDataModule::GetGuild( const NFIDENTID& self, NFIDENTID& xGuild )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;
    vFieldVec.push_back("GuildID");

    if (!m_pClusterSQLModule->Query(self.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildID = vValueVec[0];
    return xGuild.FromString(strGuildID);
}

bool NFCWorldGuildDataModule::SetGuild( const NFIDENTID& self, const NFIDENTID& xGuild )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;
    vFieldVec.push_back("GuildID");
    vValueVec.push_back(xGuild.ToString());

    if (!m_pClusterSQLModule->Updata(self.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    return true;
}

bool NFCWorldGuildDataModule::SearchGuild( const NFIDENTID& self, const std::string& strName, std::vector<SearchGuildObject>& xList )
{   
    std::vector<std::string> vValueName;
    if (!m_pClusterSQLModule->Keys(mstrGuildNameTalble, strName, vValueName))
    {
        return false;
    }

    for (int i = 0; i < vValueName.size(); ++i)
    {
        const std::string& strGuildName = vValueName[i];

        std::vector<std::string> vFieldVec;
        std::vector<std::string> vValueVec;

        vFieldVec.push_back("GuildID");

        if (!m_pClusterSQLModule->Query(mstrGuildNameTalble, strGuildName, vFieldVec, vValueVec))
        {
            continue;
        }

        NFIDENTID xGuild ;
        xGuild.FromString(vValueVec[0]);

        SearchGuildObject xGuildInfo;
        if (GetGuildInfo(self, xGuild, xGuildInfo))
        {
            xList.push_back(xGuildInfo);
        }
    }

    return true;
}

bool NFCWorldGuildDataModule::GetGuildInfo( const NFIDENTID& self, const NFIDENTID& xGuild, SearchGuildObject& xGuildInfo )
{
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("Name");
    vFieldVec.push_back("GuilIDIcon");
    vFieldVec.push_back("GuildMemeberCount");
    vFieldVec.push_back("GuildMemeberMaxCount");
    vFieldVec.push_back("GuildHonor");
    vFieldVec.push_back("Rank");

    if (!m_pClusterSQLModule->Query(mstrGuildTalble, xGuild.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    const std::string& strGuildName = vValueVec[0];

    int nGuildIconn = 0;
    int nGuildMemeberCount = 0;
    int nGuildMemeberMaxCount = 0;
    int nGuildHonor = 0;
    int nGuildRank = 0;

    NF_StrTo(vValueVec[1], nGuildIconn);
    NF_StrTo(vValueVec[2], nGuildMemeberCount);
    NF_StrTo(vValueVec[3], nGuildMemeberMaxCount);
    NF_StrTo(vValueVec[4], nGuildHonor);
    NF_StrTo(vValueVec[5], nGuildRank);

    xGuildInfo.mxGuildID = xGuild    ;
    xGuildInfo.mstrGuildName = strGuildName  ;
    xGuildInfo.mnGuildIcon = nGuildIconn  ;
    xGuildInfo.mnGuildMemberCount = nGuildMemeberCount  ;
    xGuildInfo.mnGuildMemberMaxCount = nGuildMemeberMaxCount  ;
    xGuildInfo.mnGuildHonor = nGuildHonor ;
    xGuildInfo.mnGuildRank = nGuildRank  ;  

    return true;
}

bool NFCWorldGuildDataModule::GetPlayerInfo( const NFIDENTID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP )
{
    nDonation = 0;//这什么字段，还没加？
    nVIP = 1;
    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("Name");
    vFieldVec.push_back("Level");
    vFieldVec.push_back("Job");

    if (!m_pClusterSQLModule->Query( self.ToString(), vFieldVec, vValueVec))
    {
        return false;
    }

    if (!NF_StrTo(vValueVec[0], strRoleName))
    {
        return false;
    }

    if (!NF_StrTo(vValueVec[1], nLevel))
    {
        return false;
    }

    if (!NF_StrTo(vValueVec[2], nJob))
    {
        return false;
    }

    return true;
}

bool NFCWorldGuildDataModule::GetGameID( const NFIDENTID& self, int& nGameID )
{
    std::vector<std::string> xVecFeild;
    std::vector<std::string> xVecValue;

    xVecFeild.push_back("GameID");
    if (!m_pClusterSQLModule->Query(self.ToString(), xVecFeild, xVecValue))
    {
        return false;
    }

    const std::string& strGameID = xVecValue[0];
    if (!NF_StrTo(strGameID, nGameID))
    {
        return false;
    }

    return true;
}