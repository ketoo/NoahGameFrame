// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCPropertyModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCPropertyModule.h"

bool NFCPropertyModule::Init()
{
	   m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>();
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();

    return true;
}

bool NFCPropertyModule::Shut()
{
    return true;
}

bool NFCPropertyModule::Execute()
{
    return true;
}

bool NFCPropertyModule::AfterInit()
{
    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCPropertyModule::OnObjectClassEvent);

    return true;
}

int NFCPropertyModule::GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType)
{
    if (NFPropertyGroup::NPG_ALL != eGroupType)
    {
        return m_pKernelModule->GetRecordInt(self, NFrame::Player::R_CommPropertyValue(), eGroupType, strPropertyName);
    }

    return m_pKernelModule->GetPropertyInt(self, strPropertyName);
}

int NFCPropertyModule::SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue)
{
    if (NFPropertyGroup::NPG_ALL != eGroupType)
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                return pRecord->SetInt(eGroupType, strPropertyName, nValue);
            }
        }

        //return m_pKernelModule->SetRecordInt( self, mstrCommPropertyName, eGroupType, *pTableCol, nValue );
    }

    
    m_pKernelModule->SetPropertyInt(self, strPropertyName, nValue);

    return 0;
}


int NFCPropertyModule::AddPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue)
{
    if (NFPropertyGroup::NPG_ALL != eGroupType)
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->GetInt(eGroupType, strPropertyName);

                return pRecord->SetInt(eGroupType, strPropertyName, nPropertyValue + nValue);
            }
        }
    }

    return 0;
}

int NFCPropertyModule::SubPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue)
{
    if (NFPropertyGroup::NPG_ALL != eGroupType)
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->GetInt(eGroupType, strPropertyName);

                return pRecord->SetInt(eGroupType, strPropertyName, nPropertyValue - nValue);
            }
        }
    }

    return 0;
}

int NFCPropertyModule::OnObjectLevelEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
    RefreshBaseProperty(self);

    FullHPMP(self);
    FullSP(self);

    return 0;
}

int NFCPropertyModule::OnRecordPropertyEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    
    const std::string& strRecordName = xEventData.strRecordName;
    const int nOpType = xEventData.nOpType;
    const int nRow = xEventData.nRow;
    const int nCol = xEventData.nCol;

    int nAllValue = 0;
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
    for (int i = 0; i < (int)(NFPropertyGroup::NPG_ALL); i++)
    {
        if (i < pRecord->GetRows())
        {
            int nValue = pRecord->GetInt(i, nCol);
            nAllValue += nValue;
        }
    }

    m_pKernelModule->SetPropertyInt(self, pRecord->GetColTag(nCol), nAllValue);

    return 0;
}

int NFCPropertyModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
        if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
            if (pRecord)
            {
                for (int i = 0; i < NFPropertyGroup::NPG_ALL; i++)
                {
                    pRecord->AddRow(-1);
                }
            }

            m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFCPropertyModule::OnObjectLevelEvent);

            
            m_pKernelModule->AddRecordCallBack(self, NFrame::Player::R_CommPropertyValue(), this, &NFCPropertyModule::OnRecordPropertyEvent);


        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OnlineCount());
            if (nOnlineCount <= 0 && m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID()) > 0)
            {
                
                m_pKernelModule->SetPropertyInt(self, NFrame::Player::Level(), 1);
            }
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt(self, NFrame::Player::OnlineCount());
            m_pKernelModule->SetPropertyInt(self, NFrame::Player::OnlineCount(), (nOnlineCount + 1));

        }
    }

    return 0;
}

int NFCPropertyModule::RefreshBaseProperty(const NFGUID& self)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
    if (!pRecord)
    {
        return 1;
    }

    
    int eJobType = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Job());
    int nLevel = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Level());

    for (int i = 0; i < pRecord->GetCols(); ++i)
    {
        const std::string& strColTag = pRecord->GetColTag(i);
        int nValue = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, strColTag);
        SetPropertyValue(self, strColTag, NFPropertyGroup::NPG_JOBLEVEL, nValue);
    }

    return 1;
}

bool NFCPropertyModule::FullHPMP(const NFGUID& self)
{
    NFINT64 nMaxHP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());
    if (nMaxHP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nMaxHP);
    }

    NFINT64 nMaxMP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXMP());
    if (nMaxMP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nMaxMP);
    }

    return true;
}

bool NFCPropertyModule::AddHP(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());

    if (nCurValue > 0)
    {
        nCurValue += nValue;
        if (nCurValue > nMaxValue)
        {
            nCurValue = nMaxValue;
        }

        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nCurValue);
    }

    return true;
}

bool NFCPropertyModule::EnoughHP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::ConsumeHP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nCurValue);

        return true;
    }

    return false;
}

bool NFCPropertyModule::AddMP(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP());
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXMP());

    nCurValue += nValue;
    if (nCurValue > nMaxValue)
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nCurValue);

    return true;
}

bool NFCPropertyModule::ConsumeMP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nCurValue);

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughMP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::FullSP(const NFGUID& self)
{
    NFINT64 nMAXCSP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXSP());
    if (nMAXCSP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nMAXCSP);

        return true;
    }

    return false;
}

bool NFCPropertyModule::AddSP(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP());
    NFINT64 nMaxValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXSP());

    nCurValue += nValue;
    if (nCurValue > nMaxValue)
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nCurValue);

    return true;
}

bool NFCPropertyModule::ConsumeSP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCSP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP());
    if ((nCSP > 0) && (nCSP - nValue >= 0))
    {
        nCSP -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nCSP);

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughSP(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::AddGold(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt(self, NFrame::Player::Gold(), nCurValue);

    return false;
}

bool NFCPropertyModule::ConsumeGold(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Gold(), nCurValue);

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughGold(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFCPropertyModule::AddDiamond(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt(self, NFrame::Player::Money(), nCurValue);

    return false;
}

bool NFCPropertyModule::ConsumeDiamond(const NFGUID& self, const NFINT64& nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Money(), nCurValue);

        return true;
    }

    return false;
}

bool NFCPropertyModule::EnoughDiamond(const NFGUID& self, const NFINT64& nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Money());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}