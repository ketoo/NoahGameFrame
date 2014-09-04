// -------------------------------------------------------------------------
//    @FileName         ：    NFCNet.cpp
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-12-15
//    @Module           ：    NFIPacket
//    @Desc             :     CNet
// -------------------------------------------------------------------------

#include "NFCNet.h"
#include "NFCPacket.h"
#include <WS2tcpip.h>
#include <winsock2.h>
#include <string.h>
#include "event2\bufferevent_struct.h"
#include "event2\event.h"

void NFCNet::time_cb(evutil_socket_t fd, short _event, void *argc)
{
    //     NetObject* pObject = (NetObject*)argc;
    //     if (pObject && pObject->GetNet())
    //     {
    //         NFCNet* pNet = (NFCNet*)pObject->GetNet();
    //         pNet->HeartPack();
    // 
    //         evtimer_add(pNet->ev, &(pNet->tv));
    //     }

}

void NFCNet::conn_writecb(struct bufferevent *bev, void *user_data)
{
    //每次收到发送消息的时候事件
    // 	struct evbuffer *output = bufferevent_get_output(bev);
}

void NFCNet::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{

    NetObject* pObject = (NetObject*)user_data;
    NFCNet* pNet = (NFCNet*)pObject->GetNet();
    if(!pNet->mEventCB._Empty())
    {
        pNet->mEventCB(pObject->GetFd(), NF_NET_EVENT(events));
    }

    if (events & BEV_EVENT_EOF) 
    {
        printf("%d Connection closed.\n", pObject->GetFd());
        pNet->CloseNetObject(pObject->GetFd());
        if (!pNet->mbServer)
        {
            //客户端断线重连
            pNet->Reset();
        }
    } 
    else if (events & BEV_EVENT_ERROR) 
    {
        printf("%d Got an error on the connection: %d\n", pObject->GetFd(),	errno);/*XXX win32*/
        pNet->CloseNetObject(pObject->GetFd());
        if (!pNet->mbServer)
        {
            //客户端断线重连
            pNet->Reset();
        }
    }
    else if (events & BEV_EVENT_TIMEOUT)
    {
        printf("%d read timeout: %d\n", pObject->GetFd(), errno);/*XXX win32*/
        pNet->CloseNetObject(pObject->GetFd());

        if (!pNet->mbServer)
        {
            //客户端断线重连
            pNet->Reset();
        }
    }
    else if (events & BEV_EVENT_CONNECTED)
    {
        printf("%d Connection successed\n", pObject->GetFd());/*XXX win32*/
    }
}

void NFCNet::listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
    //怕你们重了
    NFCNet* pNet = (NFCNet*)user_data;
    bool bClose = pNet->CloseNetObject(fd);
    if (bClose)
    {
        //error
        return;
    }

    if (pNet->mmObject.size() >= pNet->mnMaxConnect)
    {
        //应该T掉，拒绝
        return;
    }

    struct event_base *base = pNet->base;
    //创建一个基于socket的bufferevent
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) 
    {
        //应该T掉，拒绝
        fprintf(stderr, "Error constructing bufferevent!");
        //event_base_loopbreak(base);
        return;
    }

    //我获得一个新连接。为其创建一个bufferevent--FD需要管理
    struct sockaddr_in* pSin = (sockaddr_in*)sa;

    NetObject* pObject = new NetObject(pNet, fd, *pSin, bev);
    pObject->GetNet()->AddNetObject(fd, pObject);

    //为bufferevent设置各种回调
    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, (void*)pObject);

    //开启bufferevent的读写
    bufferevent_enable(bev, EV_READ|EV_WRITE);

    //模拟客户端已连接事件
    conn_eventcb(bev, BEV_EVENT_CONNECTED, (void*)pObject);
    //////////////////////////////////////////////////////////////////////////

    struct timeval tv;
    /* 设置读超时120秒, 可做为心跳机制, 120秒没收到消息就T */
    tv.tv_sec = 120;
    tv.tv_usec = 0; 
    bufferevent_set_timeouts(bev, &tv, NULL);
}


void NFCNet::conn_readcb(struct bufferevent *bev, void *user_data)
{
    //接受到消息
    NetObject* pObject = (NetObject*)user_data;
    if (!pObject)
    {
        return;
    }

    NFCNet* pNet = (NFCNet*)pObject->GetNet();
    if (!pNet)
    {
        return;
    }

    struct evbuffer *input = bufferevent_get_input(bev);
    if (!input)
    {
        return;
    }

    size_t len = evbuffer_get_length(input);

    //返回给客户端
    //  	struct evbuffer *output = bufferevent_get_output(bev);
    //  	evbuffer_add_buffer(output, input);
    //      SendMsg(1, strData,len, pObject->GetFd());
    //////////////////////////////////////////////////////////////////////////

    char* strData = new char[len];
    //char strData[NF_MAX_SERVER_PACKET_SIZE] = {0};
    if(evbuffer_remove(input, strData, len) > 0)
    {
        pObject->AddBuff(strData, len);

        while (1)
        {
            int len = pObject->GetBuffLen();
            if (len > pNet->mnHeadLength)
            {
                pNet->Dismantle(pObject);
            }
            else
            {
                break;
            }
        } 
    }

    delete[] strData;
}

//////////////////////////////////////////////////////////////////////////

bool NFCNet::Execute(const float fLasFrametime, const float fStartedTime)
{
    //std::cout << "Running:" << mbRuning << std::endl;
    if (base)
    {
        event_base_loop(base, EVLOOP_ONCE|EVLOOP_NONBLOCK);
    }

    return true;
}


int NFCNet::Initialization( const char* strIP, const unsigned short nPort)
{
    mstrIP = strIP;
    mnPort = nPort;

    return InitClientNet();
}

int NFCNet::Initialization( const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
    mnMaxConnect = nMaxClient;
    mnPort = nPort;
    mnCpuCount = nCpuCount;

    return InitServerNet();

}

bool NFCNet::Final()
{
    if (mbServer)
    {
        CloseSocketAll();
    }

    if (listener)
    {
        evconnlistener_free(listener);
        listener = NULL;
    }

    // bufferevent_socket_new用参数控制关闭socket自己释放
    //if (base)
    //{
    //    event_base_free(base);
    //    base = NULL;
    //}

    return true;
}

bool NFCNet::SendMsg( const NFIPacket& msg, const int nSockIndex, bool bBroadcast )
{
    return SendMsg(msg.GetPacketData(), msg.GetPacketLen(), nSockIndex, bBroadcast );
}

bool NFCNet::SendMsg(const char* msg, const uint32_t nLen, const int nSockIndex, bool bBroadcast /*= false*/ )
{
    if (nLen <= 0)
    {
        return false;
    }

    if (bBroadcast)
    {
        std::map<int, NetObject*>::iterator it = mmObject.begin();
        for (; it != mmObject.end(); ++it)
        {
            NetObject* pNetObject = (NetObject*)it->second;
            if (pNetObject)
            {
                bufferevent* bev = pNetObject->GetBuffEvent();
                if (NULL != bev)
                {
                    bufferevent_write(bev, msg, nLen);
                }
            }
        }

        return true;
    }
    else
    {
        std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
        if (it != mmObject.end())
        {
            NetObject* pNetObject = (NetObject*)it->second;
            if (pNetObject)
            {
                bufferevent* bev = pNetObject->GetBuffEvent();
                if (NULL != bev)
                {
                    bufferevent_write(bev, msg, nLen);

                    return true;
                }
            }
        }
    }

    return false;
}

bool NFCNet::CloseNetObject( const int nSockIndex )
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        NetObject* pObject = it->second;

        struct bufferevent* bev = pObject->GetBuffEvent();
        //bev->cbarg = NULL;

        bufferevent_free(bev);
        evutil_closesocket(nSockIndex);

        mmObject.erase(it);

        delete pObject;
        pObject = NULL;

        return true;
    }


    return false;
}


bool NFCNet::Dismantle(NetObject* pObject )
{
    bool bRet = true;
    NFCPacket packet(mnHeadLength);

    int len = pObject->GetBuffLen();
    if (len > packet.GetHeadSize())
    {
        int nUsedLen = packet.DeCode(pObject->GetBuff(), len);
        if (nUsedLen > 0)
        {
            packet.SetFd(pObject->GetFd());

            int nRet = 0;
            if (!mRecvCB._Empty())
            {
                mRecvCB(packet);
            }
            else
            {
                nRet = OnRecivePacket(pObject->GetFd(), packet.GetPacketData(), packet.GetPacketLen());
            }

            //添加到队列
            pObject->RemoveBuff(0, nUsedLen);
        }
        else if (0 == nUsedLen)
        {
            //长度不够(等待下次解包)
        }
        else
        {
            //累计错误太多了--可以适当清空给机会
            pObject->IncreaseError();

        }
        if (pObject->GetErrorCount() > 5)
        {
            CloseNetObject(pObject->GetFd());
        }
    }

    return bRet;
}

bool NFCNet::AddNetObject( const int nSockIndex, NetObject* pObject )
{
    return mmObject.insert(std::map<int, NetObject*>::value_type(nSockIndex, pObject)).second;
}

int NFCNet::InitClientNet()
{
    std::string strIP = mstrIP;
    int nPort = mnPort;

    struct sockaddr_in addr;
    struct bufferevent *bev = NULL;

#if NF_PLATFORM == NF_PLATFORM_WIN
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(nPort);

    if (inet_pton(AF_INET, strIP.c_str(), &addr.sin_addr) <= 0)
    {
        printf("inet_pton");
        return -1;
    }

    base = event_base_new();
    if (base == NULL) 
    {
        printf("event_base_new ");
        return -1;
    }

    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    if (bev == NULL) 
    {
        printf("bufferevent_socket_new ");
        return -1;
    }

    int sockfd = bufferevent_socket_connect(bev, (struct sockaddr *)&addr, sizeof(addr));
    if (0 != sockfd)
    {
        int nError = GetLastError();
        printf("bufferevent_socket_connect error");
        return -1;
    }

    //sockfd = bufferevent_getfd(bev);
    NetObject* pObject = new NetObject(this, sockfd, addr, bev);
    if (!AddNetObject(sockfd, pObject))
    {
        assert(0);
        return -1;
    }

    mbServer = false;

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, (void*)pObject);
    bufferevent_enable(bev, EV_READ|EV_WRITE);

    ev = evtimer_new(base, time_cb, (void*)pObject);
    evutil_timerclear(&tv); 
    tv.tv_sec = 10; //间隔
    tv.tv_usec = 0;

    evtimer_add(ev, &tv);

    //event_base_loop(base, EVLOOP_ONCE|EVLOOP_NONBLOCK);

    return sockfd;
}

int NFCNet::InitServerNet()
{
    int nMaxClient = mnMaxConnect;
    int nCpuCount = mnCpuCount;
    int nPort = mnPort;

    struct sockaddr_in sin;

#if NF_PLATFORM == NF_PLATFORM_WIN
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);

#endif
    //////////////////////////////////////////////////////////////////////////

    struct event_config *cfg = event_config_new(); 

#if NF_PLATFORM == NF_PLATFORM_WIN

    //event_config_avoid_method(cfg, "iocp");
    //event_config_require_features(cfg, event_method_feature.EV_FEATURE_ET);//触发方式
    evthread_use_windows_threads();
    if(event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP) < 0)
    {
        //使用IOCP
        return -1;
    }

    if(event_config_set_num_cpus_hint(cfg, nCpuCount) < 0)
    {
        return -1;
    }

    base = event_base_new_with_config(cfg);

#else

    //event_config_avoid_method(cfg, "epoll"); 
    if(event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST) < 0)
    {
        //使用EPOLL
        return -1;
    }

    if(event_config_set_num_cpus_hint(cfg, nCpuCount) < 0)
    {
        return -1;
    }

    base = event_base_new_with_config(cfg);//event_base_new()

#endif
    event_config_free(cfg);

    //////////////////////////////////////////////////////////////////////////

    if (!base) 
    {
        fprintf(stderr, "Could not initialize libevent!\n");
        Final();

        return -1;
    }

    //初始化时间
    //gettime(base, &base->event_tv);

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(nPort);

    printf("server started with %d\n", nPort);

    listener = evconnlistener_new_bind(base, listener_cb, (void *)this,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));

    if (!listener)
    {
        fprintf(stderr, "Could not create a listener!\n");
        Final();

        return -1;
    }

    //     signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)this);
    // 
    //     if (!signal_event || event_add(signal_event, NULL)<0)
    //     {
    //         fprintf(stderr, "Could not create/add a signal event!\n");
    //         Final();
    //         return -1;
    //     }

    mbServer = true;

    return mnMaxConnect;
}

bool NFCNet::Reset()
{

    if (!mbServer)
    {
        Final();
        InitClientNet();
    }

    return true;
}

void NFCNet::HeartPack()
{
    NFCPacket msg(mnHeadLength);
    msg.EnCode(0, "", 0);

    SendMsg(msg, 0);
}

bool NFCNet::CloseSocketAll()
{
    std::map<int, NetObject*>::iterator it = mmObject.begin();
    for (it; it != mmObject.end(); it++)
    {
        NetObject* pObject = it->second;

        struct bufferevent* bev = pObject->GetBuffEvent();
        bev->cbarg = NULL;

        bufferevent_free(bev);
        evutil_closesocket(pObject->GetFd());

        mmObject.erase(it);

        delete pObject;
        pObject = NULL;
    }

    mmObject.clear();

    return true;
}

NetObject* NFCNet::GetNetObject( const int nSockIndex )
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        return it->second;
    }

    return NULL;
}
