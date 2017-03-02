// -------------------------------------------------------------------------
//    @FileName			:    NFCUISelectServer.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUISelectServer
//
// -------------------------------------------------------------------------
#ifndef NFCUISelectServer_H
#define NFCUISelectServer_H

#include "NFCUIManager.h"

class NFCUISelectServer : public IUniqueDialog<NFCUISelectServer>
{
public:
	NFCUISelectServer();
	~NFCUISelectServer();
	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void initData(void *customData);

private:
	int OnWorldListEvent(const int nEventID, const NFDataList& varDataList);
	int OnServerListEvent(const int nEventID, const NFDataList& varDataList);

private:
	void OnWorldSelected(Ref *sender);
	void OnServerSelected(Ref *sender);
	
private:
	ui::ListView *m_pServerList;

};

#endif // _NFCUISelectServer_H__
