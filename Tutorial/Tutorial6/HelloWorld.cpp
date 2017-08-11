#include "HelloWorld.h"

bool NFCHelloWorld::Init()
{ 

    std::cout << "Hello, world, Init" << std::endl;

    return true;
}

bool NFCHelloWorld::AfterInit()
{
	m_pNavigationModule = pPluginManager->FindModule<NFINavigationModule>();

	int sceneId = 1;
	NFVector3 start(0, 0, 0);
	NFVector3 end(23, 0, 5);
	vector<NFVector3> pathResult;

	int ret = m_pNavigationModule->FindPath(sceneId, start, end, pathResult);
	if (ret > 0)
	{
		std::cout << "FindPath Success" << std::endl;
	}
	else
	{
		std::cout << "FindPath Fail" << std::endl;
	}
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