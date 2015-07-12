#include "NFCCostModule.h"


bool NFCCostModule::Init()
{
    return true;
}

bool NFCCostModule::Shut()
{
    return true;
}

bool NFCCostModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCCostModule::AfterInit()
{
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pElementModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>(pPluginManager->FindModule("NFCPropertyModule"));

    assert(nullptr != m_pKernelModule);
    assert(nullptr != m_pElementModule);
    assert(nullptr != m_pLogModule);
    assert(nullptr != m_pPropertyModule);

    return true;
}

bool NFCCostModule::Cost(const NFIDENTID& self, const std::string& strCostID)
{
    NF_SHARE_PTR<NFIObject> pOjbect = m_pKernelModule->GetObject(self);
    if (nullptr == pOjbect)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Cannot find object", "", __FUNCTION__, __LINE__);
        return false;
    }

    if (!m_pElementModule->ExistElement(strCostID))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Cannot find CostID = ", strCostID, __FUNCTION__, __LINE__);
        return false;
    }

    NFINT64 nCostMoney = m_pElementModule->GetPropertyInt(strCostID, "CostMoney");
    NFINT64 nCostDiamond = m_pElementModule->GetPropertyInt(strCostID, "CostDiamond");
    // ...
    if (!m_pPropertyModule->EnoughMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->EnoughDiamond(self, nCostDiamond))
    {
        return false;
    }
    // ...

    m_pPropertyModule->ConsumeMoney(self, nCostMoney);
    m_pPropertyModule->ConsumeDiamond(self, nCostDiamond);
    // ...

    return true;
}

bool NFCCostModule::Cost(const NFIDENTID& self, const std::list<std::string>& xCostList)
{
    NF_SHARE_PTR<NFIObject> pOjbect = m_pKernelModule->GetObject(self);
    if (nullptr == pOjbect)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Cannot find object", "", __FUNCTION__, __LINE__);
        return false;
    }

    NFINT64 nCostMoney = 0;
    NFINT64 nCostDiamond = 0;

    for (auto iter = xCostList.begin(); iter != xCostList.end(); ++iter)
    {
        const std::string& strCostID = *iter;
        if (!m_pElementModule->ExistElement(strCostID))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Cannot find CostID = ", strCostID, __FUNCTION__, __LINE__);
            return false;
        }

        nCostMoney += m_pElementModule->GetPropertyInt(strCostID, "CostMoney");
        nCostDiamond += m_pElementModule->GetPropertyInt(strCostID, "CostDiamond");

        // ...
    }

    if (!m_pPropertyModule->EnoughMoney(self, nCostMoney))
    {
        return false;
    }

    if (!m_pPropertyModule->EnoughDiamond(self, nCostDiamond))
    {
        return false;
    }
    // ...

    m_pPropertyModule->ConsumeMoney(self, nCostMoney);
    m_pPropertyModule->ConsumeDiamond(self, nCostDiamond);

    return true;
}