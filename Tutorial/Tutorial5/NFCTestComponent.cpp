#include "NFCTestComponent.h"
#include "NFComm/NFCore/NFTimer.h"
#include <thread>

bool NFCTestComponent::Init()
{
    std::cout << "Hello, world5, NFCTestComponent Init" <<" ThreadID: " << std::this_thread::get_id() << std::endl;
    return true;
}

bool NFCTestComponent::AfterInit()
{
    std::cout << "Hello, world5, NFCTestComponent AfterInit, ThreadID: " << std::this_thread::get_id() << std::endl;
    return true;
}

int NFCTestComponent::OnASyncEvent( const NFIDENTID& self, const int event, std::string& arg)
{
    std::cout << "Begin OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << std::this_thread::get_id() << std::endl;

    arg += "  event test ok";

    return 0;
}