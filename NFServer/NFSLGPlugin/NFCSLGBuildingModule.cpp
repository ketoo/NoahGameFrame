#include "NFCSLGBuildingModule.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFMessageDefine/NFSLGDefine.pb.h"
#include "NFComm/NFCore/NFTimer.h"

NFCSLGBuildingModule::NFCSLGBuildingModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

bool NFCSLGBuildingModule::Init()
{
    return true;
}

bool NFCSLGBuildingModule::AfterInit()
{
	m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
	m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );
	m_pUUIDModule = dynamic_cast<NFIUUIDModule*>( pPluginManager->FindModule( "NFCUUIDModule" ) );
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );

	assert( NULL != m_pKernelModule );
	assert( NULL != m_pLogModule );
	assert( NULL != m_pUUIDModule );
    assert( NULL != m_pEventProcessModule );

    m_pEventProcessModule->AddClassCallBack("BB_Player", this, &NFCSLGBuildingModule::OnClassObjectEvent);

    return true;
}

bool NFCSLGBuildingModule::CheckConfig()
{
    return true;
}

bool NFCSLGBuildingModule::BeforeShut()
{
    return true;
}

bool NFCSLGBuildingModule::Shut()
{
    return true;
}

bool NFCSLGBuildingModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

int NFCSLGBuildingModule::AddBuilding(const NFIDENTID& self, const std::string& strBuilding, const float fX, const float fY, const float fZ)
{
    if (strBuilding.empty())
    {
        return 1;
    }

    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (!pObject.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no object!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement("BuildingList");
    if (!pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList xDataList;
    xDataList << strBuilding;
    xDataList << m_pUUIDModule->CreateGUID();
    xDataList << NFMsg::EBS_IDLE;; // state
    xDataList << int(fX); // x
    xDataList << int(fY); // y
    xDataList << int(fZ); // z

    xDataList << NFTimeEx::GetNowTime(); // z
    xDataList << int(0); // z

    if (0 != pRecord->AddRow(-1, xDataList))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strBuilding, "cannot be added, [BuildingList] Record is full!", __FUNCTION__, __LINE__);
        return 1;
    }

    return 0;
}

int NFCSLGBuildingModule::Upgrade(const NFIDENTID& self, const NFIDENTID& xBuilID)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "BuildingList");
    if (NULL == pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList var;
    pRecord->FindObject("BuildingGUID", xBuilID, var);
    if (var.GetCount() <= 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "there are no the building", xBuilID.ToString(), __FUNCTION__, __LINE__);
        return 1;
    }

    const int nRow = var.Int(0);
    const int nNeedTime = 20;//升级所需要的时间 //TO ADD
    const std::string strBuildID = pRecord->GetString(nRow, "BuildingID");

    NFCDataList varHeart;
    varHeart << xBuilID;
    m_pKernelModule->AddHeartBeat(self, "OnUpgradeHeartBeat", this, &NFCSLGBuildingModule::OnUpgradeHeartBeat, varHeart, nNeedTime, 1);

    //修改建筑的状态
    pRecord->SetInt(nRow, "State", NFMsg::EBS_UPGRADE);
    pRecord->SetInt(nRow, "StateStartTime", NFTimeEx::GetNowTime());
    pRecord->SetInt(nRow, "StateEndTime", NFTimeEx::GetNowTime() + nNeedTime);

    return 0;
}


int NFCSLGBuildingModule::OnUpgradeHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var)
{
    if (var.GetCount() != 1 ||  var.TypeEx(TDATA_OBJECT, TDATA_UNKNOWN))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Param is wrong", "", __FUNCTION__, __LINE__);
        return 1;
    }

    const NFIDENTID& xBuildID = var.Object(0);

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "BuildingList");
    if (NULL == pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList varBuild;
    pRecord->FindObject("BuildingGUID", xBuildID, varBuild);
    if (varBuild.GetCount() <= 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "there are no the building", xBuildID.ToString(), __FUNCTION__, __LINE__);
        return 1;
    }

    const int nRow = var.Int(0);
    const std::string strBuildID = pRecord->GetString(nRow, "BuildingID");

    //升级
    //TO ADD

    //修改建筑的状态
    pRecord->SetInt(nRow, "State", NFMsg::EBS_IDLE);
    pRecord->SetInt(nRow, "StateStartTime", NFTimeEx::GetNowTime());
    pRecord->SetInt(nRow, "StateEndTime", 0);
    
    return 0;
}

int NFCSLGBuildingModule::OnBoostHeartBeat( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var )
{
    if (var.GetCount() != 1 ||  var.TypeEx(TDATA_OBJECT, TDATA_UNKNOWN))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Param is wrong", "", __FUNCTION__, __LINE__);
        return 1;
    }

    const NFIDENTID& xBuidID = var.Object(0);

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "BuildingList");
    if (NULL == pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList varBuild;
    pRecord->FindObject("BuildingGUID", xBuidID, varBuild);
    if (varBuild.GetCount() <= 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "there are no the building", xBuidID.ToString(), __FUNCTION__, __LINE__);
        return 1;
    }

    const int nRow = var.Int(0);

    //修改建筑的状态
    pRecord->SetInt(nRow, "State", NFMsg::EBS_IDLE);
    pRecord->SetInt(nRow, "StateStartTime", NFTimeEx::GetNowTime());
    pRecord->SetInt(nRow, "StateEndTime", 0);

    return 0;
}

int NFCSLGBuildingModule::OnProduceHeartBeat( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var )
{
    if (var.GetCount() != 2 || var.TypeEx(TDATA_OBJECT, TDATA_STRING, TDATA_UNKNOWN))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "var Param is wrong", "", __FUNCTION__, __LINE__);
        return 1;
    }
    //AddItem
    //To ADD

    //Cost Left Count
    const NFIDENTID& xBuildID = var.Object(0);
    const std::string& strItem = var.String(1);    
    CostProduceData(self, xBuildID, strItem, 1);

    if (nCount == 0)
    {
        int nLeftCount = 0;
        GetProduceDataCount(self, xBuildID, strItem, nLeftCount);

        if (nLeftCount > 0)
        {
            const std::string strHeartname = GetProduceHeartName(self, xBuildID, strItem);
            const int nTime = 50;//To ADD
  
            NFCDataList varHeart;
            varHeart << xBuildID;
            varHeart << strItem;
            m_pKernelModule->AddHeartBeat(self, strHeartname, this, &NFCSLGBuildingModule::OnProduceHeartBeat, varHeart, nTime, nLeftCount);
        }
    }


    return 0;
}
int NFCSLGBuildingModule::Boost(const NFIDENTID& self, const NFIDENTID& xBuilID)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "BuildingList");
    if (NULL == pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList var;
    pRecord->FindObject("BuildingGUID", xBuilID, var);
    if (var.GetCount() <= 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "there are no the building", xBuilID.ToString(), __FUNCTION__, __LINE__);
        return 1;
    }

    const int nRow = var.Int(0);
    const int nBoostTime = 20;//加速时间 // TO ADD
    const std::string strBuildID = pRecord->GetString(nRow, "BuildingID");

    NFCDataList varHeart;
    varHeart << xBuilID;
    m_pKernelModule->AddHeartBeat(self, "OnBoostHeartBeat", this, &NFCSLGBuildingModule::OnBoostHeartBeat, varHeart, nBoostTime, 1);

    //修改建筑的状态
    pRecord->SetInt(nRow, "State", NFMsg::EBS_BOOST);
    pRecord->SetInt(nRow, "StateStartTime", NFTimeEx::GetNowTime());
    pRecord->SetInt(nRow, "StateEndTime", NFTimeEx::GetNowTime() + nBoostTime);

    return 0;
}

int NFCSLGBuildingModule::Produce(const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, const int nCount)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "BuildingList");
    if (NULL == pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList var;
    pRecord->FindObject("BuildingGUID", xBuilID, var);
    if (var.GetCount() <= 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "there are no the building", xBuilID.ToString(), __FUNCTION__, __LINE__);
        return 1;
    }
    
    //增加
    AddProduceData(self, xBuilID, strItemID, nCount);
    
    const std::string strHeartname = GetProduceHeartName(self, xBuilID, strItemID);
    const int nTime = 50;//To ADD
    if (!m_pKernelModule->FindHeartBeat(self, strHeartname))
    {
        NFCDataList varHeart;
        varHeart << xBuilID;
        varHeart << strItemID;
        m_pKernelModule->AddHeartBeat(self, strHeartname, this, &NFCSLGBuildingModule::OnProduceHeartBeat, varHeart, nTime, nCount);
    }

    return 0;
}

int NFCSLGBuildingModule::Move(const NFIDENTID& self, const NFIDENTID nGUID, const float fX, const float fY, const float fZ)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "BuildingList");
    if (NULL == pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList xMatchList;
    int nMatchCount = pRecord->FindObject(BUILDING_GUID, nGUID, xMatchList);
    if (nMatchCount != 1)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "[BuildingList] Record !", "", __FUNCTION__, __LINE__);
        return 1;
    }

    int nMatchRow = xMatchList.Int(0);
    pRecord->SetInt(nMatchRow, BUILDINNG_X, fX);
    pRecord->SetInt(nMatchRow, BUILDINNG_Y, fY);
    pRecord->SetInt(nMatchRow, BUILDINNG_Z, fZ);

    return 0;
}


int NFCSLGBuildingModule::CheckBuildingStatusEnd( const NFIDENTID& self )
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "BuildingList");
    if (NULL == pRecord.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    const NFINT64 nNowTime = NFTimeEx::GetNowTime();
    for (int i= 0; i < pRecord->GetRows(); i++)
    {
        if (!pRecord->IsUsed(i))
        {
            continue;
        }

        const NFINT64 nEndTime = pRecord->GetInt(i, "StateEndTime");
        const NFIDENTID& xBuildID = pRecord->GetObject(i, "BuildingGUID");
        const int& nStatus = pRecord->GetInt(i, "State");

        float fTime = nEndTime - nNowTime;
        if (fTime <= 0.0f)
        {
            fTime = 0.1f;
        }

        if (nStatus == NFMsg::EBS_IDLE)
        {
            continue;
        }
        else if(nStatus == NFMsg::EBS_UPGRADE)
        {
            NFCDataList varHeart;
            varHeart << xBuildID;

            m_pKernelModule->AddHeartBeat(self, "OnUpgradeHeartBeat", this, &NFCSLGBuildingModule::OnUpgradeHeartBeat, varHeart, fTime, 1);
        }
        else if(nStatus == NFMsg::EBS_BOOST)
        {
            NFCDataList varHeart;
            varHeart << xBuildID;

            m_pKernelModule->AddHeartBeat(self, "OnUpgradeHeartBeat", this, &NFCSLGBuildingModule::OnUpgradeHeartBeat, varHeart, fTime, 1);
        }
    }

    return 0;
}

int NFCSLGBuildingModule::OnClassObjectEvent( const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if (COE_CREATE_FINISH == eClassEvent)
    {
        CheckBuildingStatusEnd(self);
        CheckProduceData(self);
    }

    return 0;
}

int NFCSLGBuildingModule::AddProduceData(const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, const int nCount)
{
    NF_SHARE_PTR<NFIRecord> pProduce = m_pKernelModule->FindRecord(self, "BuildingProduce");
    if (NULL == pProduce.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingProduce] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList varProduce;
    pProduce->FindObject("BuildingGUID", xBuilID, varProduce);

    int  nFindRow = -1;
    for (int i = 0; i< varProduce.GetCount(); i++)
    {
        const std::string& strHasItemID = pProduce->GetString(i, "ItemID");
        if (strHasItemID  == strItemID)
        {
            nFindRow = i;
            break;
        } 
    }

    if (nFindRow < 0)
    {
        //already exit
        const int nOldCount = pProduce->GetInt(nFindRow, "LeftCount");
        pProduce->SetInt(nFindRow, "LeftCount", nOldCount + nCount);

    }
    else
    {
        const int nTime = 20;//生产时间 //TO ADD
        NFCDataList varItem;

        varItem << xBuilID;
        varItem << strItemID;
        varItem << nCount;
        varItem << nTime;
        varItem << NFTimeEx::GetNowTime();

        pProduce->AddRow(-1, varItem);
    }

    return 0;
}

int NFCSLGBuildingModule::CostProduceData( const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, const int nCount )
{
    NF_SHARE_PTR<NFIRecord> pProduce = m_pKernelModule->FindRecord(self, "BuildingProduce");
    if (NULL == pProduce.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingProduce] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList varProduce;
    pProduce->FindObject("BuildingGUID", xBuilID, varProduce);

    for (int i = 0; i< varProduce.GetCount(); i++)
    {
        const std::string& strHasItemID = pProduce->GetString(i, "ItemID");
        if (strHasItemID  == strItemID)
        {
            const int nOldCount = pProduce->GetInt(i, "LeftCount");
            pProduce->SetInt(i, "LeftCount", nOldCount - nCount);
        }
    }

    return 0;
}

bool NFCSLGBuildingModule::GetProduceDataCount( const NFIDENTID& self, const NFIDENTID& xBuilID, const std::string& strItemID, int& nCount )
{
    NF_SHARE_PTR<NFIRecord> pProduce = m_pKernelModule->FindRecord(self, "BuildingProduce");
    if (NULL == pProduce.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingProduce] Record!", "", __FUNCTION__, __LINE__);
        return false;
    }

    NFCDataList varProduce;
    pProduce->FindObject("BuildingGUID", xBuilID, varProduce);

    if(varProduce.GetCount() <= 0)
    {
        nCount = 0;
        return true;
    }

    for (int i = 0; i< varProduce.GetCount(); i++)
    {
        const std::string& strHasItemID = pProduce->GetString(i, "ItemID");
        if (strHasItemID  == strItemID)
        {
            const int nOldCount = pProduce->GetInt(i, "LeftCount");
            nCount = pProduce->GetInt(i, "LeftCount");

            return true;
        }
    }

    return false;
}

int NFCSLGBuildingModule::CheckProduceData( const NFIDENTID& self )
{
    NF_SHARE_PTR<NFIRecord> pProduce = m_pKernelModule->FindRecord(self, "BuildingProduce");
    if (NULL == pProduce.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingProduce] Record!", "", __FUNCTION__, __LINE__);
        return false;
    }

    for (int i = 0; i < pProduce->GetRows(); i++)
    {
        if (!pProduce->IsUsed(i))
        {
            continue;
        }

        const int nNowTime = NFTimeEx::GetNowTime();
        const NFIDENTID xBuildID = pProduce->GetObject(i, "BuildingGUID");
        const std::string strItemID = pProduce->GetString(i, "ItemID");
        const int nLeftCount = pProduce->GetInt(i, "LeftCount");
        const NFINT64 nLastOnceBeginTime = pProduce->GetInt(i, "OnceStartTime");
        const NFINT64 nOnceTime = pProduce->GetInt(i, "OnceTime");

        const int nPassTime = nNowTime - nLastOnceBeginTime;
        if (nPassTime <= 0)
        {
            continue;
        }

        const int nCount = nPassTime/nOnceTime;
        if (nCount >= nLeftCount)
        {
            //add Item 
            //TO ADD

            pProduce->Remove(i);
        }
        else
        {            
            //add Item 
            //TO ADD
         
            pProduce->SetInt(i, "", nLeftCount - nCount);

            //增加定时任务
            NFCDataList varHeart;
            varHeart << xBuildID;
            varHeart << strItemID;

            const std::string strHeartname = GetProduceHeartName(self, xBuildID, strItemID);
            const int nTime = (nCount + 1) * nOnceTime - nPassTime;
            m_pKernelModule->AddHeartBeat(self, strHeartname, this, &NFCSLGBuildingModule::OnProduceHeartBeat, varHeart, nTime, 1);            
        }
    }

    return 0;
}

std::string NFCSLGBuildingModule::GetProduceHeartName(const NFIDENTID& self, const NFIDENTID& xBuild, const std::string& strItemID)
{
    return xBuild.ToString() + "_" + strItemID + "_" + "OnProduceHeartBeat";
}
