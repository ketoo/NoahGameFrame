// -------------------------------------------------------------------------
//    @FileName			:    NFCRecordLogic.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCRecordLogic
//
// -------------------------------------------------------------------------

#ifndef NFC_RECORDLOGIC_H
#define NFC_RECORDLOGIC_H

#include "NFCLogicBase.h"

enum RecordLogicEvent
{
};

class NFCRecordLogic
    : public NFCLogicBase, public NFSingleton<NFCRecordLogic>
{
public:
	NFCRecordLogic() {};
	virtual ~NFCRecordLogic() {};
    NFCRecordLogic(NFIPluginManager* p)
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
	void OnRecordInt(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordFloat(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordString(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRecordObject(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSwapRow(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAddRow(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRemoveRow(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);	
	void OnObjectRecordEntry(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	void AddRecord(const NF_SHARE_PTR<NFIObject>& object, const std::string &strRecordName, const NFMsg::RecordAddRowStruct &data);
};

#define g_pRecordLogic (NFCRecordLogic::Instance())

#endif