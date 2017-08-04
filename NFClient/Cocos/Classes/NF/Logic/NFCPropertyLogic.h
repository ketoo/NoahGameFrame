// -------------------------------------------------------------------------
//    @FileName			:    NFCPropertyLogic.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCPropertyLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_PropertyLogic_MODULE_H
#define NFC_PropertyLogic_MODULE_H

#include "NFCLogicBase.h"

enum PropertyLogicEvent
{
};

class NFCPropertyLogic
    : public NFCLogicBase, public NFSingleton<NFCPropertyLogic>
{
public:
	NFCPropertyLogic() {};
	virtual ~NFCPropertyLogic() {};
    NFCPropertyLogic(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool ReadyExecute();
    virtual bool Execute();

    virtual bool AfterInit();

	// 发送消息
public:
	// 接收消息
private:
	// 属性
	void OnPropertyInt(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyFloat(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyString(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnPropertyObject(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectPropertyEntry(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
};

#define g_pPropertyLogic (NFCPropertyLogic::Instance())

#endif