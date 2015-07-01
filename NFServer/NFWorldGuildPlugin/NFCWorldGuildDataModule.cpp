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
            AttachData(self);
        }
    }

    return 0;
}


const bool NFCWorldGuildDataModule::AttachData( const NFIDENTID& self )
{
    NF_SHARE_PTR<RoleData> pRoleData = mxRoleDataMap.GetElement(self);
    if (nullptr == pRoleData)
    {
        return false;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if (nullptr != pObject)
    {
        NF_SHARE_PTR<NFIPropertyManager> pProManager = pObject->GetPropertyManager();
        NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

        NFMsg::PlayerPropertyBase xPropertyList;
        if (!pRoleData->strProperty.empty() && pRoleData->strProperty != "NULL")
        {
            if(!xPropertyList.ParseFromString(pRoleData->strProperty))
            {
                return false;
            }
        }

        NFMsg::PlayerRecordList xRecordList;
        if (!pRoleData->strRecord.empty() && pRoleData->strRecord != "NULL")
        {
            if(!xRecordList.ParseFromString(pRoleData->strRecord))
            {
                return false;
            }
        }

        mxRoleDataMap.RemoveElement(self);

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

const bool NFCWorldGuildDataModule::LoadDataFormNoSql( const NFIDENTID& self )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
    if ( !pObject )
    {
        std::vector<std::string> vFieldVec;
        std::vector<std::string> vValueVec;
        vFieldVec.push_back("Property");
        vFieldVec.push_back("Record");

        if(!m_pClusterSQLModule->Query(mstrGuildTalble, self.ToString(), vFieldVec, vValueVec)
            || vFieldVec.size() != vValueVec.size()
            || vFieldVec.size()  < 2)
        {
            return false;
        }

        NF_SHARE_PTR<RoleData> pRoleData = NF_SHARE_PTR<RoleData>(NF_NEW RoleData());
        pRoleData->strProperty = vValueVec[0];
        pRoleData->strRecord = vValueVec[1];
        mxRoleDataMap.AddElement(self, pRoleData);
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

        NFMsg::PlayerPropertyBase xPropertyList;
        NFMsg::PlayerRecordList xRecordList;

        std::vector<std::string> vFieldPropertyVec;
        std::vector<std::string> vValuePropertyVec;

        std::string strName;
        NF_SHARE_PTR<NFIProperty> xProperty = pProManager->First(strName);
        while (xProperty)
        {
            if (xProperty->GetSave())
            {
                switch (xProperty->GetType())
                {
                case TDATA_INT:
                    {
                        NFMsg::PropertyInt* xPropertyInt = xPropertyList.add_property_int_list();
                        xPropertyInt->set_property_name(strName);
                        xPropertyInt->set_data(xProperty->GetInt());

                        vFieldPropertyVec.push_back(strName);
                        vValuePropertyVec.push_back(boost::lexical_cast<std::string>(xProperty->GetInt()));
                    }
                    break;
                case TDATA_FLOAT:
                    {
                        NFMsg::PropertyFloat* xPropertyFloat = xPropertyList.add_property_float_list();
                        xPropertyFloat->set_property_name(strName);
                        xPropertyFloat->set_data(xProperty->GetFloat());

                        vFieldPropertyVec.push_back(strName);
                        vValuePropertyVec.push_back(boost::lexical_cast<std::string>(xProperty->GetFloat()));
                    }
                    break;
                case TDATA_STRING:
                    {
                        NFMsg::PropertyString* xPropertyString = xPropertyList.add_property_string_list();
                        xPropertyString->set_property_name(strName);
                        xPropertyString->set_data(xProperty->GetString());

                        vFieldPropertyVec.push_back(strName);
                        vValuePropertyVec.push_back(xProperty->GetString());
                    }
                    break;
                case TDATA_OBJECT:
                    {
                        NFMsg::PropertyObject* xPropertyObject = xPropertyList.add_property_object_list();
                        xPropertyObject->set_property_name(strName);
                        *xPropertyObject->mutable_data() = NFINetModule::NFToPB(xProperty->GetObject());

                        vFieldPropertyVec.push_back(strName);
                        vValuePropertyVec.push_back(xProperty->GetObject().ToString());
                    }
                    break;
                default:
                    break;
                }
            }

            strName.clear();
            xProperty = pProManager->Next(strName);
        }


        NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First(strName);
        while (xRecord)
        {
            if (xRecord->GetSave())
            {
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

        if(!m_pClusterSQLModule->Updata(mstrGuildTalble, self.ToString(), vFieldVec, vValueVec))
        {
            return false;
        }

        if(!m_pClusterSQLModule->Updata(mstrGuildTalble, self.ToString(), vFieldPropertyVec, vValuePropertyVec))
        {
            return false;
        }

        return false;
    }

    return true;
}

const NFIDENTID NFCWorldGuildDataModule::CreateGuild(const NFIDENTID& xPlayeID, const std::string& strName)
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

    NF_SHARE_PTR<NFIObject> pGuildObejct = m_pKernelModule->CreateObject(xGuidID, 1, 0, "Guild", "", NFCDataList());
    if (!pGuildObejct.get())
    {
        return NFIDENTID();
    }

    pGuildObejct->SetPropertyString("Name", strName);
    pGuildObejct->SetPropertyInt("GuildLevel", 1);

    std::vector<std::string> vFieldVec;
    std::vector<std::string> vValueVec;

    vFieldVec.push_back("GuildID");
    vValueVec.push_back(xGuidID.ToString());

    m_pClusterSQLModule->Updata(mstrGuildNameTalble, strName, vFieldVec, vValueVec);
    SaveDataToNoSql(xGuidID);
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
        const std::string& strGuildName = vValueName[1];
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
