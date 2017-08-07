// -------------------------------------------------------------------------
//    @FileName			:    NFCUIManager.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUIManager
// -------------------------------------------------------------------------
#ifndef _NFCUIManager_H_
#define _NFCUIManager_H_

#include "NFCUIDialog.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"

class NFCUIManager : public NFIModule, public NFSingleton<NFCUIManager>
{
public:
	NFCUIManager() {}
	~NFCUIManager() {}

    NFCUIManager(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

	// ---------  ------------
    virtual Node* RootNode();
    
    virtual NFCUIDialog* ShowDialog(NFCUIDialog *pDialog, bool bPushToHistory = true);
    virtual void CloseDialog(NFCUIDialog *pDialog);
	virtual void ChangeBackDialog();
	virtual bool HasBackDialog();
	virtual void CloseDialog();

	virtual void ShowPanel(NFCUIDialog *pPanel);
	virtual void ClosePanel(NFCUIDialog *pDialog);
	virtual void CloseAllPanel();

public:
	void onBackKeyClicked();
	void onMenuKeyClicked();

private:
    std::vector<NFCUIDialog*> m_vecDialog;
	
    // UI的根節點
	CC_SYNTHESIZE_READONLY(Node*, m_pRootNode, RootNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pPanelNode, PanelNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pDialogNode, DialogNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pBoxNode, BoxNode);
	CC_SYNTHESIZE_READONLY(Node*, m_pEffectNode, EffectNode);

private:
    Vector<NFCUIDialog*> m_pDialogQueue;
	Vector<NFCUIDialog*> m_pPanelQueue;
	NFCUIDialog *m_pCurrentDialog;
};

#define g_pNFCUIManager (NFCUIManager::Instance())

// 与管理器逻辑无关的纯UI逻辑类
template<typename T>
class IUniqueDialog: public NFCUIDialog
{	
public:
	// 显示窗口(窗口没创建时创建窗口并显示，已经创建则只显示)
	static void showUI(bool bPushToHistory = true, const void *customData = nullptr) 
	{ 
		IUniqueDialog **p = _getDialog();
		if (!*p) {
			*p = new T;
			(*p)->initLayout();
			(*p)->autorelease();
		}
		
		(*p)->initData((void*)customData);
		g_pNFCUIManager->ShowDialog(*p, bPushToHistory);
	}
	static void closeUI(const void *customData = nullptr) 
	{ 
		IUniqueDialog **p = _getDialog();
		if (*p) {
			g_pNFCUIManager->CloseDialog(*p);
		}
	}
	IUniqueDialog() { // 不允许外部手动new
		*_getDialog() = this;
	}
	virtual ~IUniqueDialog() { // 不允许外部手动delete
		if(*_getDialog() == this)
			*_getDialog() = nullptr;
	}

private:
	static IUniqueDialog** _getDialog() {
		static T* _instance;
		return (IUniqueDialog**)(&_instance);
	}
};

template<typename T>
class IUniquePanel : public NFCUIDialog
{
public:
	// 显示窗口(窗口没创建时创建窗口并显示，已经创建则只显示)
	static void showPanel(const void *customData = nullptr)
	{
		IUniquePanel **p = _getPanel();
		if (!*p) {
			*p = new T;
			(*p)->initLayout();
			(*p)->autorelease();
		}

		(*p)->initData((void*)customData);
		g_pNFCUIManager->ShowPanel(*p);
	}
	static void closePanel(const void *customData = nullptr)
	{
		IUniquePanel **p = _getPanel();
		if (*p) {
			g_pNFCUIManager->ClosePanel(*p);
		}
	}
	IUniquePanel() { // 不允许外部手动new
		*_getPanel() = this;
	}
	virtual ~IUniquePanel() { // 不允许外部手动delete
		if (*_getPanel() == this)
			*_getPanel() = nullptr;
	}

private:
	static IUniquePanel** _getPanel() {
		static T* _instance;
		return (IUniquePanel**)(&_instance);
	}
};

#endif
