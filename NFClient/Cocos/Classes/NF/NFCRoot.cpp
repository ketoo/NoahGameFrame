// -------------------------------------------------------------------------
//    @FileName			:    NFCRoot.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-27
//    @Module           :    NFCRoot
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCRoot.h"
#include "NFCClientPlugin.h"
#include "NFComm/NFPluginLoader/NFCPluginManager.h"

#include "UI/NFCUIPlugin.h"
#include "Logic/NFCLogicPlugin.h"
#include "Scene/NFCScenePlugin.h"

#include "UI/NFCUILogin.h"

USING_NS_CC;

#ifdef WIN32
extern "C" int gettimeofday(struct timeval *val, struct timezone *tz)
{
	return cocos2d::gettimeofday(val, tz);
}
#endif

Scene* NFCRoot::scene()
{
    Scene *scene = Scene::create();    
    NFCRoot *layer = NFCRoot::create();
    scene->addChild(layer);
    return scene;
}

NFCRoot::NFCRoot()
{		
}

NFCRoot::~NFCRoot()
{
	NFCPluginManager::GetSingletonPtr()->BeforeShut();
	NFCPluginManager::GetSingletonPtr()->Shut();
	NFCPluginManager::GetSingletonPtr()->ReleaseInstance();
}

bool NFCRoot::init()
{
    if ( !CCNode::init() )
    {
        return false;
    }

	NFIPluginManager *pPluginManager = NFCPluginManager::GetSingletonPtr();

	pPluginManager->SetGetFileContentFunctor([](const std::string &strFileName, std::string &strContent) -> bool
	{
		if (FileUtils::getInstance()->getContents(strFileName, &strContent) != FileUtils::Status::OK || strContent.empty())
		{
			return false;
		}
		return true;
	});

	pPluginManager->SetAppName("GameClient");

	CREATE_PLUGIN(pPluginManager, NFCClientPlugin);

	CREATE_PLUGIN(pPluginManager, NFCLogicPlugin);
	CREATE_PLUGIN(pPluginManager, NFCUIPlugin);
	CREATE_PLUGIN(pPluginManager, NFCScenePlugin);
	
	NFCPluginManager::GetSingletonPtr()->Init();
	NFCPluginManager::GetSingletonPtr()->AfterInit();
	NFCPluginManager::GetSingletonPtr()->CheckConfig();
	NFCPluginManager::GetSingletonPtr()->ReadyExecute();

	CCFileUtils::getInstance()->addSearchPath("res");

	NFCUILogin::showUI();

	scheduleUpdate();

    return true;
}

void NFCRoot::update(float dt)
{
	NFCPluginManager::GetSingletonPtr()->Execute();
}