// -------------------------------------------------------------------------
//    @FileName			:    NFCUIChatPanel.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUIChatPanel
//
// -------------------------------------------------------------------------
#ifndef NFCUIChatPanel_H
#define NFCUIChatPanel_H

#include "NFCUIManager.h"

class NFCUIChatPanel : public IUniquePanel<NFCUIChatPanel>
{
public:
	NFCUIChatPanel();
	~NFCUIChatPanel();
	virtual const char* getUIName() { return "UIChatPanel"; }
	virtual const char* getUIPath() { return "UIChatPanel.csb"; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void initData(void *customData);

private:

private:
	void onLoginTouch(Ref *sender);
	
private:
	ui::Button *m_pSend;
	ui::TextField *m_pInputText;

};

#endif // __HELLOWORLD_SCENE_H__
