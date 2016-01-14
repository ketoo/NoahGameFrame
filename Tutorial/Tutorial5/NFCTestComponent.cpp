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

int NFCTestComponent::OnASyncEvent( const NFGUID& self, const int event, std::string& arg)
{
    std::cout << "Begin OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg << " ThreadID: " << std::this_thread::get_id() << std::endl;

    arg += "  event test ok";

    return 0;
}

NF_SHARE_PTR<NFIComponent> NFCTestComponent::CreateNewInstance()
{
	return NF_SHARE_PTR<NFIComponent>(NF_NEW NFCTestComponent(pPluginManager));
}
