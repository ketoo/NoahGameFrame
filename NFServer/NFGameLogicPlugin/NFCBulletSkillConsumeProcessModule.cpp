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
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>( "NFCSkillConsumeManagerModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );

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

bool NFCBulletSkillConsumeProcessModule::Execute()
{
    return true;
}


int NFCBulletSkillConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFIDataList& other )
{
    //�Ƿ񱻳�Ĭ,CD,�����
    return 1;
}

int NFCBulletSkillConsumeProcessModule::ConsumeSelf( const NFGUID& self, const std::string& skillID )
{
    //�õ�����-1

    //ʧ��

    return 0;
}

int NFCBulletSkillConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFIDataList& other, NFIDataList& damageListValue , NFIDataList& damageResultList )
{
    return 0;
}

int NFCBulletSkillConsumeProcessModule::ConsumeProcessEx( const NFGUID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList )
{
    return 0;
}
