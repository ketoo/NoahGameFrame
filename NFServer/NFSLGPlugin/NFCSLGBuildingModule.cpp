#include "NFCSLGBuildingModule.h"

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

int NFCSLGBuildingModule::AddBuilding(const NFIDENTID& self, const std::string& strBuilding)
{
    if (strBuilding.empty())
    {
        return 1;
    }

    NFIObject* pObject = m_pKernelModule->GetObject(self);
    if (NULL == pObject)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "LoadDataFormNoSql", "this is no object!", __FUNCTION__, __LINE__);
        return 1;
    }


    return 0;
}