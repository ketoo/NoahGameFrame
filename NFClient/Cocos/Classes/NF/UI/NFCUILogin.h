// -------------------------------------------------------------------------
//    @FileName			:    NFCUILogin.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUILogin
//
// -------------------------------------------------------------------------
#ifndef NFCUILogin_H
#define NFCUILogin_H

#include "NFCUIManager.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"

class NFCUILogin : public IUniqueDialog<NFCUILogin>
{
public:
	NFCUILogin();
	~NFCUILogin();
	virtual const char* getUIName() { return "UILogin"; }
	virtual const char* getUIPath() { return "UILogin.csb"; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void initData(void *customData);

private:
	void OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	int OnLoginEvent(const int, const NFDataList&);

private:
	void onLoginTouch(Ref *sender);
	
private:
	ui::Button *m_pLoginButton;
	ui::TextField *m_pUserName;
	ui::TextField *m_pUserPWD;

};

#endif // __HELLOWORLD_SCENE_H__
