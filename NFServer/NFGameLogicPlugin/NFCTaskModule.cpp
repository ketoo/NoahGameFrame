#include "NFCTaskModule.h"

bool NFCTaskModule::Init()
{
    return true;
}

bool NFCTaskModule::Shut()
{
    return true;
}

bool NFCTaskModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCTaskModule::AfterInit()
{
    return true;
}

int NFCTaskModule::AddTask( const NFGUID& self, const std::string& strTask )
{
    // ��������Ҫ�ض�����������ʱֹͣ����ϵͳ�Ŀ���
    return 0;
}

int NFCTaskModule::RemoveTask( const NFGUID& self, const std::string& strTask )
{
    return 0;
}