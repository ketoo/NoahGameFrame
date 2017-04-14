// -------------------------------------------------------------------------
//    @FileName			:    NFCWebsocketModule.h
//    @Author           :    Stone.xin<xinxinst@163.com>
//    @Date             :    2016-12-22
//    @Module           :    NFCWebsocketModule
//    @Desc             :
// -------------------------------------------------------------------------
#ifndef NFC_WEBSOCKET_MODULE_H
#define NFC_WEBSOCKET_MODULE_H

#include "NFCWS.h"
#include "NFComm/NFPluginModule/NFIWebsocketModule.h"


class NFCWebsocketModule : public NFIWebsocketModule
{
public:
	NFCWebsocketModule(NFIPluginManager* p);

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Shut() override;

	virtual bool BeforeShut() override;

	virtual bool Execute() override;
	
	virtual void SetReceiveCallBack(NF_WS_MSG_CALL_BACK_PTR functorPtr) override;
	virtual void SetEventCallBack(NF_WS_EVENT_CALL_BACK_PTR functorPtr) override;

	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) override;

private:
	void OnWebsocketMessage(websocketpp::connection_hdl, const std::string&);
	void OnWebsocketEvent(websocketpp::connection_hdl, NF_WS_EVENT);

private:
	NFCWS*					m_pWSServer;	
	NF_WS_MSG_CALL_BACK_PTR	m_pRecvMsgCB;
	NF_WS_EVENT_CALL_BACK_PTR m_pEvtCB;

};

#endif