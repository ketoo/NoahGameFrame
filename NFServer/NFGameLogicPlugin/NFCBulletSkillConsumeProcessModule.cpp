// -------------------------------------------------------------------------
//    @FileName      :   NFCBulletSkillConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBulletSkillConsumeProcessModule
//    @Desc             :   �������ѻ�����
// -------------------------------------------------------------------------

#include "NFCBulletSkillConsumeProcessModule.h"

bool NFCBulletSkillConsumeProcessModule::Init()
{

    return true;
}

bool NFCBulletSkillConsumeProcessModule::AfterInit()
{
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pSkillConsumeManagerModule = dynamic_cast<NFISkillConsumeManagerModule*>( pPluginManager->FindModule( "NFCSkillConsumeManagerModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pSkillConsumeManagerModule );
    assert( NULL != m_pElementInfoModule );

    //�ӵ��༼��
    m_pSkillConsumeManagerModule->ResgisterConsumeModule( EGameSkillType::EGST_JOBSKILL_BULLET, this );

    return true;
}

bool NFCBulletSkillConsumeProcessModule::Shut()
{
    return true;
}

bool NFCBulletSkillConsumeProcessModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}


int NFCBulletSkillConsumeProcessModule::ConsumeLegal( const NFIDENTID& self, const std::string& skillID,  const NFIDataList& other )
{
    //�Ƿ񱻳�Ĭ,CD,�����
    return 1;
}

int NFCBulletSkillConsumeProcessModule::ConsumeSelf( const NFIDENTID& self, const std::string& skillID )
{
    //�õ�����-1

    //ʧ��

    return 0;
}

int NFCBulletSkillConsumeProcessModule::ConsumeProcess( const NFIDENTID& self, const std::string& strItemName, const NFIDataList& other, NFIDataList& damageListValue , NFIDataList& damageResultList )
{
    return 0;
}

int NFCBulletSkillConsumeProcessModule::ConsumeProcessEx( const NFIDENTID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList )
{
    return 0;
}
