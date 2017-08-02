#include "HelloWorld.h"
#include "NFComm/NFCore/NFCObject.h"
#include "NFComm/NFCore/NFIComponent.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFNetPlugin/NFCWebsocketModule.h"

bool NFCHelloWorld::Init()
{ 

    std::cout << "Hello, world, Init" << std::endl;

    return true;
}

bool NFCHelloWorld::AfterInit()
{
	m_pWSModule = GetPluginManager()->FindModule<NFIWebsocketModule>();
	m_pWSModule->Initialization(1000,9004);

	std::cout << "websocket module init with port"<< 9004 << std::endl;

	NF_WS_MSG_CALL_BACK msg_cb = std::bind(&NFCHelloWorld::OnWebsocketMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NF_WS_EVENT_CALL_BACK evt_cb = std::bind(&NFCHelloWorld::OnWebsocketEvent, this, std::placeholders::_1, std::placeholders::_2);

	NF_WS_MSG_CALL_BACK_PTR pMsgCB(new NF_WS_MSG_CALL_BACK(msg_cb));
	NF_WS_EVENT_CALL_BACK_PTR pEvtCB(new NF_WS_EVENT_CALL_BACK(evt_cb));

	m_pWSModule->SetEventCallBack(pEvtCB);
	m_pWSModule->SetReceiveCallBack(pMsgCB);

    return true;
}

bool NFCHelloWorld::Execute()
{
	m_pWSModule->Execute();
 
    return true;
}

bool NFCHelloWorld::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFCHelloWorld::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}

void NFCHelloWorld::OnWebsocketMessage(websocketpp::connection_hdl conn, const std::string& strMsg, NF_WS_MSG_DATA_TYPE msg_data_type)
{
	std::cout << "OnWebsocketMessage " << strMsg << std::endl;
	m_pWSModule->SendMsgToClient(strMsg.c_str(), strMsg.length(), conn, msg_data_type);
}

void NFCHelloWorld::OnWebsocketEvent(websocketpp::connection_hdl, NF_WS_EVENT evt)
{
	std::cout << "OnWebsocketEvent " << evt << std::endl;
}
