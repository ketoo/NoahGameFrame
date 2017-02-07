#include "HelloWorld1.h"

bool NFCHelloWorld1::Init()
{
	std::cout << typeid(NFCHelloWorld1).name() << std::endl;

    
    std::cout << "Hello, world1, Init" << std::endl;

    return true;
}

bool NFCHelloWorld1::AfterInit()
{
    
    std::cout << "Hello, world1, AfterInit" << std::endl;

    return true;
}

bool NFCHelloWorld1::Execute()
{
    
    //std::cout << "Hello, world1, Execute" << std::endl;

    return true;
}

bool NFCHelloWorld1::BeforeShut()
{
    std::cout << "Hello, world1, BeforeShut1111" << std::endl;

    system("PAUSE");

    
    std::cout << "Hello, world1, BeforeShut" << std::endl;

    return true;
}

bool NFCHelloWorld1::Shut()
{
    
    std::cout << "Hello, world1, Shut" << std::endl;

    return true;
}
