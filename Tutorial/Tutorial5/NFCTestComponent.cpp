#include "NFCTestComponent.h"
#include "NFComm/NFCore/NFTimer.h"
#include <thread>

bool NFCTestComponent::Init()
{
    std::cout << "Hello, world5, NFCTestComponent Init" << " Init Data:" << mstrInitData <<" ThreadID: " << std::this_thread::get_id() << std::endl;
    return true;
}

bool NFCTestComponent::AfterInit()
{
    std::cout << "Hello, world5, NFCTestComponent AfterInit, ThreadID: " << std::this_thread::get_id() << std::endl;
    return true;
}

int NFCTestComponent::OnASyncEvent(const NFIDENTID& self, const int event, std::string& arg)
{
    std::cout << "Begin OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " Init Data:"<< mstrInitData << " ThreadID: " << std::this_thread::get_id() << std::endl;

    arg += "  event test ok";

    return 0;
}

void NFCTestComponent::SetInitData(const std::string& strInitData)
{
    mstrInitData = strInitData;
}
