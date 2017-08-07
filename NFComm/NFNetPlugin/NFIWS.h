// -------------------------------------------------------------------------
//    @FileName			:		NFIWS.h
//    @Author			:		Stone.xin
//    @Date				:		2016-12-22
//    @Module			:		NFIWS
// -------------------------------------------------------------------------

#ifndef NFI_WS_H
#define NFI_WS_H

#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include "NFComm/NFPluginModule/NFPlatform.h"

#define ASIO_ENABLE_OLD_SERVICES
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_STL_

#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif

struct NF_WS_CONFIG : public websocketpp::config::asio {
	// pull default settings from our core config
	typedef websocketpp::config::asio core;

	typedef core::concurrency_type concurrency_type;
	typedef core::request_type request_type;
	typedef core::response_type response_type;
	typedef core::message_type message_type;
	typedef core::con_msg_manager_type con_msg_manager_type;
	typedef core::endpoint_msg_manager_type endpoint_msg_manager_type;

	typedef core::alog_type alog_type;
	typedef core::elog_type elog_type;
	typedef core::rng_type rng_type;
	typedef core::endpoint_base endpoint_base;

	static bool const enable_multithreading = true;

	struct transport_config : public core::transport_config {
		typedef core::concurrency_type concurrency_type;
		typedef core::elog_type elog_type;
		typedef core::alog_type alog_type;
		typedef core::request_type request_type;
		typedef core::response_type response_type;

		static bool const enable_multithreading = true;
	};

	typedef websocketpp::transport::asio::endpoint<transport_config>
		transport_type;

	static const websocketpp::log::level elog_level =
		websocketpp::log::elevel::none;
	static const websocketpp::log::level alog_level =
		websocketpp::log::alevel::none;
};

typedef websocketpp::server<NF_WS_CONFIG> NFWebSockConf;

enum NF_WS_EVENT
{
	NF_WS_EVENT_OPEN = 0x01,			//a client connected
	NF_WS_EVENT_CLOSE = 0x02,			//client connection disconnected
	NF_WS_EVENT_FAIL = 0x03,			//fail message
	NF_WS_EVENT_INTERRUPT = 0x04,		//when client interrupt 
	NF_WS_EVENT_PING = 0x05,			//recv ping event from client
	NF_WS_EVENT_PONG = 0x06,			//recv pong event from client
	NF_WS_EVENT_PONG_TIMEOUT = 0x07,	//server send ping event to client,and recv pong time out

	NF_WS_EVENT_MSG = 0x10				//websocket message 
};


enum NF_WS_CLOSE_CODE
{
	CLOSE_CODE_NORMAL = 1000,
	CLOSE_CODE_KICKOUT = 1001,
};

enum NF_WS_MSG_DATA_TYPE
{
	TEXT = 0x1,
	BINARY = 0x2,
};


typedef std::function<void(websocketpp::connection_hdl,const std::string&,NF_WS_MSG_DATA_TYPE) >			NF_WS_MSG_CALL_BACK;
typedef std::shared_ptr<NF_WS_MSG_CALL_BACK> NF_WS_MSG_CALL_BACK_PTR;

typedef std::function<void(websocketpp::connection_hdl,NF_WS_EVENT) > 					NF_WS_EVENT_CALL_BACK;
typedef std::shared_ptr<NF_WS_EVENT_CALL_BACK> NF_WS_EVENT_CALL_BACK_PTR;

struct WSObject
{
	bool	bNeedRemove = false;

	bool NeedRemove()
	{
		return bNeedRemove;
	}
	void SetNeedRemove(bool bRemove)
	{
		bNeedRemove = bRemove;
	}
};

typedef std::shared_ptr<WSObject> WSObjectPtr;

class NFIWS
{
public:
	virtual ~NFIWS() {}

    //need to call this function every frame to drive network library
    virtual bool Execute() = 0;

    virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;

    virtual bool Final() = 0;

	//send a message to all client
	virtual bool SendMsgToAllClient(const char* msg, const uint32_t nLen, NF_WS_MSG_DATA_TYPE msg_data_type = TEXT) = 0;

	//send a message to client list
	virtual bool SendMsgToClient(const char* msg, const uint32_t nLen, const std::vector<websocketpp::connection_hdl>& conn_list, NF_WS_MSG_DATA_TYPE msg_data_type = TEXT) =0;

	//send a message to client
	virtual bool SendMsgToClient(const char* msg, const uint32_t nLen, websocketpp::connection_hdl conn, NF_WS_MSG_DATA_TYPE msg_data_type = TEXT) = 0;

};

#endif
