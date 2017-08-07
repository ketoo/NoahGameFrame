// -------------------------------------------------------------------------
//    @FileName			:   NFCBriefSkillConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBriefSkillConsumeProcessModule
// -------------------------------------------------------------------------

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFCBriefSkillConsumeProcessModule.h"

bool NFCBriefSkillConsumeProcessModule::Init()
{
 
    return true;
}

bool NFCBriefSkillConsumeProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSkillModule = pPluginManager->FindModule<NFISkillModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();

    return true;
}

bool NFCBriefSkillConsumeProcessModule::Shut()
{
    return true;
}

bool NFCBriefSkillConsumeProcessModule::Execute()
{
    return true;
}


int NFCBriefSkillConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFDataList& other )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject == NULL )
	{
	    return 1;
	}

    return 0;
}

int NFCBriefSkillConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strSkillName, const NFDataList& other, NFDataList& damageListValue, NFDataList& damageResultList )
{
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pElementModule->GetPropertyManager( strSkillName );
    if ( pPropertyManager == NULL )
    {
        return 1;
    }

    {
        for ( int j = 0; j < other.GetCount(); j++ )
        {
            NFGUID identOther = other.Object( j );
            if ( identOther.IsNull() )
            {
                continue;
            }

            NF_SHARE_PTR<NFIObject> pOtherObject = m_pKernelModule->GetObject( identOther );
            if ( pOtherObject == NULL )
            {
                continue;
            }

			pOtherObject->SetPropertyObject(NFrame::NPC::LastAttacker(), self);
        }
    }

    return 0;
}
