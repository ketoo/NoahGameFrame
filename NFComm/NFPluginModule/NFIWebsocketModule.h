// -------------------------------------------------------------------------
//    @FileName			:    NFIWebsocketModule.h
//    @Author           :    Stone.xin<xinxinst@163.com>
//    @Date             :    2016-12-22
//    @Module           :    NFIWebsocketModule
//    @Desc             :
// -------------------------------------------------------------------------
#ifndef NFI_WEBSOCKET_MODULE_H__
#define NFI_WEBSOCKET_MODULE_H__

#include "NFComm/NFNetPlugin/NFIWS.h"
#include "NFIModule.h"

class NFIWebsocketModule : public NFIModule
{
public:
	template<typename BaseType>
	void SetReceiveCallBack(BaseType* pBaseType, void (BaseType::*handleRecieve)(websocketpp::connection_hdl, const std::string&))
	{
		NF_WS_MSG_CALL_BACK functor = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2);
		NF_WS_MSG_CALL_BACK_PTR functorPtr(new NF_WS_MSG_CALL_BACK(functor));

		SetReceiveCallBack(functorPtr);
	}

	template<typename BaseType>
	void SetEventCallBack(BaseType* pBaseType, void (BaseType::*handleEvent)(websocketpp::connection_hdl, NF_WS_EVENT))
	{
		NF_WS_EVENT_CALL_BACK functor = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2);
		NF_WS_EVENT_CALL_BACK_PTR functorPtr(new NF_WS_EVENT_CALL_BACK(functor));

		SetEventCallBack(functorPtr);
	}
	
	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;

	virtual void SetReceiveCallBack(NF_WS_MSG_CALL_BACK_PTR functorPtr) = 0;
	virtual void SetEventCallBack(NF_WS_EVENT_CALL_BACK_PTR functorPtr) = 0;
};

#endif // NFI_WEBSOCKET_MODULE_H__