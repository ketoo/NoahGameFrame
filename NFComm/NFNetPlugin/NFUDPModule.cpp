/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFUDPModule.h"
#include "NFComm/NFNetPlugin/NFINet.h"

#define BUF_SIZE                        14500
//500

static void udp_cb(const int sock, short int which, void *arg)
{
	NFUDPModule* udpModule = (NFUDPModule*)arg;


	struct sockaddr_in client_addr;
	socklen_t size = sizeof(client_addr);
	char buf[BUF_SIZE];
	std::string  data(buf);
	std::cout << std::this_thread::get_id() << " received:" << data.length() << std::endl;

	/* Recv the data, store the address of the sender in server_sin */
	if (recvfrom(sock, &buf, sizeof(buf) - 1, 0, (struct sockaddr *) &client_addr, &size) == -1)
	{
		perror("recvfrom()");
		//event_loopbreak();
	}

	/* Send the data back to the client */
	if (sendto(sock, data.c_str(), data.length(), 0, (struct sockaddr *) &client_addr, size) == -1 )
	{
		perror("sendto()");
		//event_loopbreak();
	}
}

int bind_socket(struct event *ev, int port, void* p)
{
	int                 sock_fd;
	int                 flag = 1;
	struct sockaddr_in  sin;
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		perror("socket()");
		return -1;
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0)
	{
		perror("setsockopt()");
		return 1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);

	if (::bind(sock_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("bind()");
		return -1;
	}
	else
	{
		printf("bind() success - [%u]\n", port);
	}

	event_set(ev, sock_fd, EV_READ | EV_PERSIST, &udp_cb, p);
	if (event_add(ev, NULL) == -1)
	{
		printf("event_add() failed\n");
	}

	return 0;
}

NFUDPModule::NFUDPModule(NFIPluginManager* p)
{
	pPluginManager = p;

	mnBufferSize = 0;
}

NFUDPModule::~NFUDPModule()
{

}

bool NFUDPModule::Init()
{
	return true;
}

bool NFUDPModule::AfterInit()
{

	return true;
}

int NFUDPModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
	/* Init. event */
	mxBase = event_init();
	if (mxBase == NULL)
	{
		printf("event_init() failed\n");
		return -1;
	}

	/* Bind socket */
	if (bind_socket(&udp_event, nPort, this) != 0)
	{
		printf("bind_socket() failed\n");
		return -1;
	}

	return 0;
}

int NFUDPModule::ExpandBufferSize(const unsigned int size)
{
	return 0;
}

void NFUDPModule::RemoveReceiveCallBack(const int nMsgID)
{

}

bool NFUDPModule::AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR &cb)
{
	return true;
}

bool NFUDPModule::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR &cb)
{
	return true;
}

bool NFUDPModule::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR &cb)
{
	return true;
}

bool NFUDPModule::Execute()
{
	if (mxBase)
	{
		event_base_loop(mxBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}

	return true;
}

bool NFUDPModule::SendMsgWithOutHead(const int nMsgID, const std::string &msg, const NFSOCK nSockIndex)
{
	return true;
}

bool NFUDPModule::SendMsgToAllClientWithOutHead(const int nMsgID, const std::string &msg)
{
	return true;
}

bool NFUDPModule::SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message &xData, const NFSOCK nSockIndex)
{
	return true;
}

bool NFUDPModule::SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message &xData, const NFSOCK nSockIndex,
							const NFGUID id)
{
	return true;
}

bool NFUDPModule::SendMsg(const uint16_t nMsgID, const std::string &xData, const NFSOCK nSockIndex)
{
	return true;
}

bool NFUDPModule::SendMsg(const uint16_t nMsgID, const std::string &xData, const NFSOCK nSockIndex, const NFGUID id)
{
	return true;
}

bool NFUDPModule::SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message &xData)
{
	return true;
}

bool NFUDPModule::SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message &xData, const NFSOCK nSockIndex, const std::vector<NFGUID> *pClientIDList)
{
	return true;
}

bool NFUDPModule::SendMsgPB(const uint16_t nMsgID, const std::string &strData, const NFSOCK nSockIndex, const std::vector<NFGUID> *pClientIDList)
{
	return true;
}

NFINet *NFUDPModule::GetNet()
{
	return nullptr;
}

void NFUDPModule::OnReceiveNetPack(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}

void NFUDPModule::OnSocketNetEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet *pNet)
{

}
