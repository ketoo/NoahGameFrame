#include "HelloWorld1.h"

extern NFCMemory xMemory;

bool HelloWorld1::Init()
{
    //��ʼ��
    std::cout << "Hello, world1, Init" << std::endl;

    return true;
}

bool HelloWorld1::AfterInit()
{
    //��ʼ�����
    std::cout << "Hello, world1, AfterInit" << std::endl;

    return true;
}

bool HelloWorld1::Execute( const float fLasFrametime, const float fStartedTime )
{
    //ÿִ֡��
    //std::cout << "Hello, world1, Execute" << std::endl;

    return true;
}

bool HelloWorld1::BeforeShut()
{
    xMemory.Dump();
    std::cout << "Hello, world1, BeforeShut1111" << std::endl;

    system("PAUSE");

    //����ʼ��֮ǰ
    std::cout << "Hello, world1, BeforeShut" << std::endl;

    return true;
}

bool HelloWorld1::Shut()
{
    //����ʼ��
    std::cout << "Hello, world1, Shut" << std::endl;

    return true;
}
