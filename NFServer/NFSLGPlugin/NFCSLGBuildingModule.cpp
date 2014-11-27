#include "NFCSLGBuildingModule.h"
#include "NFComm\NFCore\NFIObject.h"

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
	
	assert( NULL != m_pKernelModule );
	assert( NULL != m_pLogModule );
	assert( NULL != m_pUUIDModule );

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

    NFIObject* pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no object!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFIRecord* pRecord = pObject->GetRecordManager()->GetElement("BuildingList");
    if (NULL == pRecord)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no [BuildingList] Record!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFCDataList xDataList;
    xDataList << strBuilding;
    xDataList << m_pUUIDModule->CreateGUID();
    xDataList << 0; // state
    xDataList << int(0); // x
    xDataList << int(0); // y
    xDataList << int(0); // z

    if (0 != pRecord->AddRow(-1, xDataList))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strBuilding, "cannot be added, [BuildingList] Record is full!", __FUNCTION__, __LINE__);
        return 1;
    }

    return 0;
}

int NFCSLGBuildingModule::OnUpgrade(const NFIDENTID& self, const NFIDataList& var)
{
    return 0;
}

int NFCSLGBuildingModule::OnBoost(const NFIDENTID& self, const NFIDataList& var)
{
    return 0;
}

int NFCSLGBuildingModule::OnProduce(const NFIDENTID& self, const NFIDataList& var)
{
    return 0;
}

int NFCSLGBuildingModule::OnMove(const NFIDENTID& self, const NFIDataList& var)
{
    // guid, config_id, x, y, z
    if (var.GetCount() != 4 || !var.TypeEx(TDATA_OBJECT, TDATA_FLOAT, TDATA_FLOAT, TDATA_FLOAT, TDATA_UNKNOWN))
    {
        return 1;
    }

    NFIDENTID nGUID = var.Object(0);
    float fX = var.Float(1);
    float fY = var.Float(2);
    float fZ = var.Float(3);

    NFIObject* pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "this is no object!", "", __FUNCTION__, __LINE__);
        return 1;
    }

    NFIRecord* pRecord = pObject->GetRecordManager()->GetElement("BuildingList");
    if (NULL == pRecord)
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