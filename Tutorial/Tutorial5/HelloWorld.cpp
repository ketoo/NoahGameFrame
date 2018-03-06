#include "HelloWorld.h"
#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFDataList.hpp"

bool NFCHelloWorld::Init()
{ 

    std::cout << "Hello, world, Init" << std::endl;

    return true;
}

bool NFCHelloWorld::AfterInit()
{

    return true;
}

bool NFCHelloWorld::Execute()
{
 
    return true;
}

bool NFCHelloWorld::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFCHelloWorld::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}
