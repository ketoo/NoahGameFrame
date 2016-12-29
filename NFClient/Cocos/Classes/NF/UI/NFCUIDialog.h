// -------------------------------------------------------------------------
//    @FileName			:    NFCUIDialog.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCUIDialog
//
// -------------------------------------------------------------------------
#ifndef NFCUIDialog_H
#define NFCUIDialog_H

class NFCUIDialog : public cocos2d::Layer
{
public:
	NFCUIDialog();
	~NFCUIDialog();
		
    Ref *GetObjectByName(const char *pName);
    template <typename T>
    T GetObjectByName(T *ppControl, const char *pName)
    {
        *ppControl = dynamic_cast<T>(GetObjectByName(pName));
        return *ppControl;
    }

	virtual const char* getUIName() { return ""; }
	virtual const char* getUIPath() { return ""; }
	virtual int getUIPriority() { return 0; }

	virtual bool init();
    virtual bool initLayout();
	virtual void initData(void *customData) {};
	
    virtual void onEnter();
    virtual void onExit();

protected:
	std::string m_strFileName;
	Node *m_pContent;
};

#endif // _NFCUIDialog_H__
