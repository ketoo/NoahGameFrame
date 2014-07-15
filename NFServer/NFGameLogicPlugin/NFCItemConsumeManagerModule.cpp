// -------------------------------------------------------------------------
//    @FileName      :   NFCItemConsumeManagerModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCItemConsumeManagerModule
//    @Desc             :   �������ѻ��ƹ�����,�������͵ĵ����������;���ע���������
// -------------------------------------------------------------------------

#include "NFCItemConsumeManagerModule.h"

//

bool NFCItemConsumeManagerModule::Init()
{
    return true;
}

bool NFCItemConsumeManagerModule::Shut()
{
    return true;
}

bool NFCItemConsumeManagerModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCItemConsumeManagerModule::AfterInit()
{
    return true;
}

bool NFCItemConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NFIItemConsumeProcessModule* pModule )
{
    return AddElement( nModuleType, pModule );
}

NFIItemConsumeProcessModule* NFCItemConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
