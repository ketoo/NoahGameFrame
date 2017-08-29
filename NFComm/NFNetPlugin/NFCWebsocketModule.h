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
	

	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) override;

	virtual bool SendMsgToAllClient(const char* msg, const uint32_t nLen, NF_WS_MSG_DATA_TYPE msg_data_type = TEXT) override;
	virtual bool SendMsgToClient(const char* msg, const uint32_t nLen, const std::vector<websocketpp::connection_hdl>& conn_list, NF_WS_MSG_DATA_TYPE msg_data_type = TEXT) override;
	virtual bool SendMsgToClient(const char* msg, const uint32_t nLen, websocketpp::connection_hdl conn, NF_WS_MSG_DATA_TYPE msg_data_type = TEXT) override;


private:
	virtual void SetReceiveCallBack(NF_WS_MSG_CALL_BACK_PTR functorPtr) override;
	virtual void SetEventCallBack(NF_WS_EVENT_CALL_BACK_PTR functorPtr) override;

	void OnWebsocketMessage(websocketpp::connection_hdl, const std::string&,NF_WS_MSG_DATA_TYPE );
	void OnWebsocketEvent(websocketpp::connection_hdl, NF_WS_EVENT);

private:
	NFCWS*					m_pWSServer;	
	NF_WS_MSG_CALL_BACK_PTR	m_pRecvMsgCB;
	NF_WS_EVENT_CALL_BACK_PTR m_pEvtCB;

};

#endif