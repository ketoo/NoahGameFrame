// -------------------------------------------------------------------------
//    @FileName			:    NFCUIManager.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUIManager
//
// -------------------------------------------------------------------------

#include "stdafx.h"

#include "NFCUIManager.h"

#include "NFCRoot.h"
bool NFCUIManager::Init()
{
	m_pCurrentDialog = NULL;
    m_pRootNode = CCNode::create();
    m_pRootNode->retain();

	NFCRoot::Instance()->addChild(m_pRootNode, NFCRoot::eIZO_UI);
    
    m_pPanelNode = CCNode::create();
    m_pRootNode->addChild(m_pPanelNode);

    m_pDialogNode = CCNode::create();
    m_pRootNode->addChild(m_pDialogNode);

    m_pBoxNode = CCNode::create();
    m_pRootNode->addChild(m_pBoxNode);

	m_pEffectNode = CCNode::create(); 
	m_pRootNode->addChild(m_pEffectNode);

	return true;
}
bool NFCUIManager::Shut()
{
	if (NULL == m_pRootNode)
		return true;

	m_pRootNode->removeFromParentAndCleanup(true);
	CC_SAFE_RELEASE_NULL(m_pRootNode);
	
	m_pRootNode = NULL;
	m_pDialogNode = NULL;
	m_pPanelNode = NULL;
	m_pEffectNode = NULL;
	m_pBoxNode = NULL;

	return true;
}

bool NFCUIManager::ReadyExecute()
{
	return true;
}

bool NFCUIManager::Execute()
{
	return true;
}

bool NFCUIManager::AfterInit()
{
	return true;
}

Node* NFCUIManager::RootNode()
{
    return m_pRootNode;
}

NFCUIDialog* NFCUIManager::ShowDialog(NFCUIDialog *pDialog, bool bPushToHistory/* = true*/)
{
	NFCUIDialog *pOldDialog = m_pCurrentDialog;

    m_pCurrentDialog = pDialog;
    
    if(pOldDialog)
    {
        if (bPushToHistory)
            m_pDialogQueue.pushBack(pOldDialog);

        m_pDialogNode->removeChild(pOldDialog);
    }

    if(m_pCurrentDialog)
    {
        m_pDialogNode->addChild(m_pCurrentDialog);
    }

    return m_pCurrentDialog;
}

void NFCUIManager::CloseDialog(NFCUIDialog *pDialog)
{
	if(m_pCurrentDialog == pDialog)
	{
		ChangeBackDialog();
		return ;
	}

	int nIndex = m_pDialogQueue.getIndex(pDialog);
	if(nIndex != CC_INVALID_INDEX)
	{
		m_pDialogQueue.erase(nIndex);
	}
	m_pDialogNode->removeChild(pDialog);
}

void NFCUIManager::ChangeBackDialog()
{
    int nDialogs = m_pDialogQueue.size();

    if (m_pCurrentDialog)
    {
        if (nDialogs == 0)
        {
			m_pCurrentDialog->runAction(Sequence::create(DelayTime::create(0.3f), RemoveSelf::create(), NULL));
        }
        else
        {
            m_pDialogNode->removeChild(m_pCurrentDialog);
        }
        m_pCurrentDialog = NULL;
    }

    if(nDialogs > 0)
    {
        m_pCurrentDialog = (NFCUIDialog*)m_pDialogQueue.at(nDialogs-1);
        if(m_pCurrentDialog)
        {
            m_pDialogNode->addChild(m_pCurrentDialog);
        }
        m_pDialogQueue.popBack();
    }
}

bool NFCUIManager::HasBackDialog()
{
    return m_pDialogQueue.size() > 0;
}

void NFCUIManager::CloseDialog()
{
    if(m_pCurrentDialog)
    {
        m_pCurrentDialog->removeFromParentAndCleanup(true);
        m_pCurrentDialog = NULL;
    }
    m_pDialogQueue.clear();
}


void NFCUIManager::ShowPanel(NFCUIDialog *pPanel)
{
	int nIndex = m_pPanelQueue.getIndex(pPanel);
	if (nIndex == CC_INVALID_INDEX)
	{
		m_pPanelQueue.pushBack(pPanel);
		m_pPanelNode->addChild(pPanel);
	}
}

void NFCUIManager::ClosePanel(NFCUIDialog *pPanel)
{
	int nIndex = m_pPanelQueue.getIndex(pPanel);
	if (nIndex != CC_INVALID_INDEX)
	{
		m_pPanelQueue.erase(nIndex);
		m_pPanelNode->removeChild(pPanel);
	}
}

void NFCUIManager::CloseAllPanel()
{
	m_pPanelQueue.clear();
	m_pPanelNode->removeAllChildren();
}

void NFCUIManager::onBackKeyClicked()
{
	CCLOG("%s", __FUNCTION__);
}

void NFCUIManager::onMenuKeyClicked()
{

}