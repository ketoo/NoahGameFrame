// -------------------------------------------------------------------------
//    @FileName      :    NFCObjectSaveModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCObjectSaveModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCObjectSaveModule.h"
#include "NFDataBaseNet_ClientPlugin.h"

bool NFCObjectSaveModule::Init()
{    
    return true;
}

bool NFCObjectSaveModule::Shut()
{
    return true;
}

bool NFCObjectSaveModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCObjectSaveModule::AfterInit()
{
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pClusterSQLModule = dynamic_cast<NFIClusterModule*>( pPluginManager->FindModule( "NFCMysqlClusterModule" ) );
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pClusterSQLModule);
    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pLogicClassModule);

    // Log

    return true;
}

bool NFCObjectSaveModule::BeforeShut()
{
    return true;
}

const bool NFCObjectSaveModule::LoadDataFormNoSql( const NFIDENTID& self , const std::string& strClassName)
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

const bool NFCObjectSaveModule::SaveDataToNoSql( const NFIDENTID& self )
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

const bool NFCObjectSaveModule::AttachData( const NFIDENTID& self )
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
                xProperty->FromString(strData);
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

bool NFCObjectSaveModule::RegisterAutoSave( const std::string& strClassName )
{
    return m_pEventProcessModule->AddClassCallBack( strClassName, this, &NFCObjectSaveModule::OnObjectClassEvent );
}


int NFCObjectSaveModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {
        SaveDataToNoSql(self);
    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
    {
        AttachData(self);
    }

    return 0;
}

const bool NFCObjectSaveModule::ConvertPBToRecord(const NFMsg::PlayerRecordBase& xRecordData, NF_SHARE_PTR<NFIRecord> xRecord)
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