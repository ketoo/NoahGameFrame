// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneManager.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCSceneManager
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCSceneManager.h"
#include "NFCRoot.h"

bool NFCSceneManager::Init()
{
    m_pRootNode = CCNode::create();
    m_pRootNode->retain();
	m_pCurrentScene = NULL;
	NFCRoot::Instance()->addChild(m_pRootNode, NFCRoot::eIZO_Scene);
    
	return true;
}
bool NFCSceneManager::Shut()
{
	if (NULL == m_pRootNode)
		return true;

	m_pRootNode->removeFromParentAndCleanup(true);
	CC_SAFE_RELEASE_NULL(m_pRootNode);

	return true;
}

bool NFCSceneManager::ReadyExecute()
{
	return true;
}

bool NFCSceneManager::Execute()
{
	return true;
}

bool NFCSceneManager::AfterInit()
{
	return true;
}

Node* NFCSceneManager::RootNode()
{
    return m_pRootNode;
}

void NFCSceneManager::ShowScene(Node *pScene)
{
	if(!pScene)
		return ;

	if(m_pCurrentScene == pScene)
		return ;

	if(m_pCurrentScene)
	{
		CloseScene(m_pCurrentScene);
	}
	m_pCurrentScene = pScene;	
	m_pRootNode->addChild(pScene);
	m_pCurrentScene->retain();
}

void NFCSceneManager::CloseScene(Node *pScene)
{
	if(!pScene)
		return ;

	if(m_pCurrentScene == pScene)
	{
		m_pCurrentScene->autorelease();
		m_pCurrentScene->release();
		m_pCurrentScene = NULL;
	}

	m_pRootNode->removeChild(pScene);
}