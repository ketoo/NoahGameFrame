// -------------------------------------------------------------------------
//    @FileName			:    NFCUISelectRole.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUISelectRole
//
// -------------------------------------------------------------------------
#ifndef NFCUISelectRole_H
#define NFCUISelectRole_H

#include "NFCUIManager.h"

class NFCUISelectRole : public IUniqueDialog<NFCUISelectRole>
{
public:
	NFCUISelectRole();
	~NFCUISelectRole();
	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

    virtual bool initLayout();
	virtual void initData(void *customData);

private:
	int OnRoleListEvent(const int nEventID, const NFDataList& varDataList);

private:
	void OnRoleSelected(Ref* sender);
	void OnRoleCreate(Ref* sender);
	
private:
	ui::ListView *m_pRoleList;

};

#endif // _NFCUISelectRole_H__
