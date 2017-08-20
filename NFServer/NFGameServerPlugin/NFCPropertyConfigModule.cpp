// -------------------------------------------------------------------------
//    @FileName			:    NFCPropertyConfigModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyConfigModule
//
// -------------------------------------------------------------------------

#include "NFCPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCPropertyConfigModule::Init()
{
    return true;
}

bool NFCPropertyConfigModule::Shut()
{
    return true;
}

bool NFCPropertyConfigModule::Execute()
{
    return true;
}

bool NFCPropertyConfigModule::AfterInit()
{
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    Load();

    return true;
}

NFINT64 NFCPropertyConfigModule::CalculateBaseValue(const int nJob, const int nLevel, const std::string& strProperty)
{
	NF_SHARE_PTR <NFMapEx<int, std::string> > xPropertyMap = mhtCoefficienData.GetElement(nJob);
	if (xPropertyMap)
	{
		NF_SHARE_PTR<std::string> xRefPropertyIDName = xPropertyMap->GetElement(nLevel);
		if (xRefPropertyIDName)
		{
			return m_pElementModule->GetPropertyInt(*xRefPropertyIDName, strProperty);
		}
    }

    return 0;
}

void NFCPropertyConfigModule::Load()
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::InitProperty::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager(strId);
            if (pPropertyManager)
            {
                int nJob = m_pElementModule->GetPropertyInt32(strId, NFrame::InitProperty::Job());
                int nLevel = m_pElementModule->GetPropertyInt32(strId, NFrame::InitProperty::Level());
                std::string strEffectData = m_pElementModule->GetPropertyString(strId, NFrame::InitProperty::EffectData());

				NF_SHARE_PTR <NFMapEx<int, std::string> > xPropertyMap = mhtCoefficienData.GetElement(nJob);
				if (!xPropertyMap)
				{
					xPropertyMap = NF_SHARE_PTR<NFMapEx<int, std::string>>(NF_NEW NFMapEx<int, std::string>());
					mhtCoefficienData.AddElement(nJob, xPropertyMap);
				}

				NF_SHARE_PTR<std::string> xRefPropertyIDName = xPropertyMap->GetElement(nLevel);
				if (!xRefPropertyIDName)
				{
					xRefPropertyIDName = NF_SHARE_PTR<std::string>(NF_NEW std::string(strEffectData));
				}

				xPropertyMap->AddElement(nLevel, xRefPropertyIDName);
            }
        }
    }
}

bool NFCPropertyConfigModule::LegalLevel(const int nJob, const int nLevel)
{
	NF_SHARE_PTR <NFMapEx<int, std::string> > xPropertyMap = mhtCoefficienData.GetElement(nJob);
	if (xPropertyMap)
	{
		NF_SHARE_PTR<std::string> xRefPropertyIDName = xPropertyMap->GetElement(nLevel);
		if (xRefPropertyIDName)
		{
			return true;
		}
	}


    return false;
}
