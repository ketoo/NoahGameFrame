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

int NFCTaskModule::AddTask( const NFIDENTID& self, const std::string& strTask )
{
    // 因任务需要特定需求，所以暂时停止任务系统的开发
    return 0;
}

int NFCTaskModule::RemoveTask( const NFIDENTID& self, const std::string& strTask )
{
    return 0;
}