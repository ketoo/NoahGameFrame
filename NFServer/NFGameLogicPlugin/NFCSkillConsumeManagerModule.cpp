// -------------------------------------------------------------------------
//    @FileName      :   NFCSkillConsumeManagerModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCSkillConsumeManagerModule
//    @Desc             :   �������ѻ��ƹ�����,�������͵ļ����������;���ע���������
// -------------------------------------------------------------------------

#include "NFCSkillConsumeManagerModule.h"

//

bool NFCSkillConsumeManagerModule::Init()
{
    return true;
}

bool NFCSkillConsumeManagerModule::Shut()
{
    return true;
}

bool NFCSkillConsumeManagerModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCSkillConsumeManagerModule::AfterInit()
{
    return true;
}

bool NFCSkillConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NFISkillConsumeProcessModule* pModule )
{
    return AddElement( nModuleType, pModule );
}

NFISkillConsumeProcessModule* NFCSkillConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
