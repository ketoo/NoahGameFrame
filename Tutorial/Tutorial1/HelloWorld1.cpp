#include "HelloWorld1.h"

extern NFCMemory xMemory;

bool HelloWorld1::Init()
{
    //初始化
    std::cout << "Hello, world1, Init" << std::endl;

    return true;
}

bool HelloWorld1::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world1, AfterInit" << std::endl;

    return true;
}

bool HelloWorld1::Execute( const float fLasFrametime, const float fStartedTime )
{
    //每帧执行
    //std::cout << "Hello, world1, Execute" << std::endl;

    return true;
}

bool HelloWorld1::BeforeShut()
{
    xMemory.Dump();
    std::cout << "Hello, world1, BeforeShut1111" << std::endl;

    system("PAUSE");

    //反初始化之前
    std::cout << "Hello, world1, BeforeShut" << std::endl;

    return true;
}

bool HelloWorld1::Shut()
{
    //反初始化
    std::cout << "Hello, world1, Shut" << std::endl;

    return true;
}
