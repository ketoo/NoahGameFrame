// -------------------------------------------------------------------------
//    @FileName			:    NFCUIDialog.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUIDialog
//
// -------------------------------------------------------------------------
#include "stdafx.h"

#include "NFCUIDialog.h"

NFCUIDialog::NFCUIDialog()
	: m_pContent(NULL)
{
}

NFCUIDialog::~NFCUIDialog()
{
}

Ref *NFCUIDialog::GetObjectByName(const char *pName)
{
	return ui::Helper::seekWidgetByName((ui::Widget*)m_pContent, pName);
}

bool NFCUIDialog::init()
{
	Layer::init();
	return initLayout();
}

bool NFCUIDialog::initLayout()
{	
	if(strcmp(getUIPath(), "") != 0)
	{
		m_strFileName = FileUtils::getInstance()->fullPathForFilename(getUIPath());
		m_pContent = CSLoader::createNode(m_strFileName.c_str());
	}
	else
	{
		Size winSize = Director::getInstance()->getWinSize();
		ui::Layout *pLayout = ui::Layout::create();
		pLayout->setContentSize(winSize);
		m_pContent = pLayout;
	}

	if(!m_pContent)
	{
		return false;
	}

	addChild(m_pContent);

	return true;
}

void NFCUIDialog::onEnter()
{
	CCLayer::onEnter();
}

void NFCUIDialog::onExit()
{
	CCLayer::onExit();
}