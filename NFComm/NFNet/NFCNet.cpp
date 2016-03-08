// -------------------------------------------------------------------------
//    @FileName         ：    NFCNet.cpp
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-12-15
//    @Module           ：    NFIPacket
//    @Desc             :     CNet
// -------------------------------------------------------------------------
#pragma  comment(lib,"libevent.lib")
#pragma  comment(lib,"libevent_core.lib")
//#pragma  comment(lib,"libevent_extras.lib")

#include "NFCNet.h"
#include <string.h>

#ifdef _MSC_VER
#include <WS2tcpip.h>
#include <winsock2.h>
#pragma  comment(lib,"Ws2_32.lib")
#endif

#include "event2/bufferevent_struct.h"
#include "event2/event.h"
#include <atomic>

class NFLock
{
public:
    explicit NFLock()
    {
        flag.clear();
    }

    ~NFLock()
    {
    }
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

protected:
    mutable std::atomic_flag flag;

private:
    NFLock& operator=(const NFLock& src);
};

void NFCNet::conn_writecb(struct bufferevent* bev, void* user_data)
{
    //每次收到发送消息的时候事件
    //  struct evbuffer *output = bufferevent_get_output(bev);
}

void NFCNet::conn_eventcb(struct bufferevent* bev, short events, void* user_data)
{
    NetObject* pObject = (NetObject*)user_data;
    NFCNet* pNet = (NFCNet*)pObject->GetNet();
    if (pNet->mEventCB)
    {
        pNet->mEventCB(pObject->GetRealFD(), NF_NET_EVENT(events), pNet);
    }

    for (std::list<NET_EVENT_FUNCTOR_PTR>::iterator it = pNet->mxEventCallBack.begin(); it != pNet->mxEventCallBack.end(); ++it)
    {
        NET_EVENT_FUNCTOR_PTR& pFunPtr = *it;
        NET_EVENT_FUNCTOR* pFunc = pFunPtr.get();
        pFunc->operator()(pObject->GetRealFD(), NF_NET_EVENT(events), pNet);
    }

    if (events & BEV_EVENT_CONNECTED)
    {
        //printf("%d Connection successed\n", pObject->GetFd());/*XXX win32*/
    }
    else
    {
        pNet->CloseNetObject(pObject->GetRealFD());
    }
}

void NFCNet::listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data)
{
    //怕你们重了
    NFCNet* pNet = (NFCNet*)user_data;
    bool bClose = pNet->CloseNetObject(fd);
    if (bClose)
    {
        return;
    }

    if (pNet->mmObject.size() >= pNet->mnMaxConnect)
    {
        //应该T掉，拒绝
        return;
    }

    struct event_base* base = pNet->base;
    //创建一个基于socket的bufferevent
    struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
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
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    //模拟客户端已连接事件
    conn_eventcb(bev, BEV_EVENT_CONNECTED, (void*)pObject);
}


void NFCNet::conn_readcb(struct bufferevent* bev, void* user_data)
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

    if (pObject->NeedRemove())
    {
        return;
    }

    struct evbuffer* input = bufferevent_get_input(bev);
    if (!input)
    {
        return;
    }

    size_t len = evbuffer_get_length(input);

    //返回给客户端
    //      struct evbuffer *output = bufferevent_get_output(bev);
    //      evbuffer_add_buffer(output, input);
    //      SendMsg(1, strData,len, pObject->GetFd());
    //////////////////////////////////////////////////////////////////////////

    //不能用成员静态等
    char* strMsg = new char[len];

    if (evbuffer_remove(input, strMsg, len) > 0)
    {
        pObject->AddBuff(strMsg, len);
    }

    delete[] strMsg;

    while (1)
    {
        if (!pNet->Dismantle(pObject))
        {
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

bool NFCNet::Execute()
{
    ExecuteClose();

    //std::cout << "Running:" << mbRuning << std::endl;
    if (base)
    {
        event_base_loop(base, EVLOOP_ONCE | EVLOOP_NONBLOCK);
    }

    return true;
}


void NFCNet::Initialization(const char* strIP, const unsigned short nPort)
{
    mstrIP = strIP;
    mnPort = nPort;

    InitClientNet();
}

int NFCNet::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
    mnMaxConnect = nMaxClient;
    mnPort = nPort;
    mnCpuCount = nCpuCount;

    return InitServerNet();
}

bool NFCNet::Final()
{

    CloseSocketAll();

    if (listener)
    {
        evconnlistener_free(listener);
        listener = NULL;
    }

    if (!mbServer)
    {
        if (base)
        {
            event_base_free(base);
            base = NULL;
        }
    }

    return true;
}

bool NFCNet::SendMsgToAllClient(const char* msg, const uint32_t nLen)
{
    if (nLen <= 0)
    {
        return false;
    }

    std::map<int, NetObject*>::iterator it = mmObject.begin();
    for (; it != mmObject.end(); ++it)
    {
        NetObject* pNetObject = (NetObject*)it->second;
        if (pNetObject && !pNetObject->NeedRemove())
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


bool NFCNet::SendMsg(const char* msg, const uint32_t nLen, const int nSockIndex)
{
    if (nLen <= 0)
    {
        return false;
    }

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

    return false;
}

bool NFCNet::SendMsg(const char* msg, const uint32_t nLen, const std::list<int>& fdList)
{
    std::list<int>::const_iterator it = fdList.begin();
    for (; it != fdList.end(); ++it)
    {
        SendMsg(msg, nLen, *it);
    }

    return true;
}

bool NFCNet::CloseNetObject(const int nSockIndex)
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        NetObject* pObject = it->second;

        pObject->SetNeedRemove(true);
        mvRemoveObject.push_back(nSockIndex);

        return true;
    }

    return false;
}

bool NFCNet::Dismantle(NetObject* pObject)
{
    bool bNeedDismantle = false;

    int len = pObject->GetBuffLen();
    if (len > NFIMsgHead::NF_Head::NF_HEAD_LENGTH)
    {
        NFCMsgHead xHead;
        int nMsgBodyLength = DeCode(pObject->GetBuff(), len, xHead);
        if (nMsgBodyLength > 0 && xHead.GetMsgID() > 0)
        {
            int nRet = 0;

            if (mRecvCB)
            {
                mRecvCB(pObject->GetRealFD(), xHead.GetMsgID(), pObject->GetBuff() + NFIMsgHead::NF_Head::NF_HEAD_LENGTH, nMsgBodyLength);
            }

            //msg callback list
            std::map<int, NET_RECIEVE_FUNCTOR_PTR>::iterator it = mxReciveCallBack.find(xHead.GetMsgID());
            if (mxReciveCallBack.end() != it)
            {
                NET_RECIEVE_FUNCTOR_PTR& pFunPtr = it->second;
                NET_RECIEVE_FUNCTOR* pFunc = pFunPtr.get();
                pFunc->operator()(pObject->GetRealFD(), xHead.GetMsgID(), pObject->GetBuff() + NFIMsgHead::NF_Head::NF_HEAD_LENGTH, nMsgBodyLength);
            }

            pObject->RemoveBuff(0, nMsgBodyLength + NFIMsgHead::NF_Head::NF_HEAD_LENGTH);

            Dismantle(pObject);
        }
        else if (0 == nMsgBodyLength)
        {
            //长度不够(等待下次解包)

            bNeedDismantle = false;
        }
        else
        {
            //累计错误太多了--可以适当清空给机会
            //pObject->IncreaseError();

            bNeedDismantle = false;

        }
    }

    return bNeedDismantle;
}

bool NFCNet::AddNetObject(const int nSockIndex, NetObject* pObject)
{
    //lock
    return mmObject.insert(std::map<int, NetObject*>::value_type(nSockIndex, pObject)).second;
}

int NFCNet::InitClientNet()
{
    std::string strIP = mstrIP;
    int nPort = mnPort;

    struct sockaddr_in addr;
    struct bufferevent* bev = NULL;

#ifdef _MSC_VER
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

    int bRet = bufferevent_socket_connect(bev, (struct sockaddr*)&addr, sizeof(addr));
    if (0 != bRet)
    {
        //int nError = GetLastError();
        printf("bufferevent_socket_connect error");
        return -1;
    }

    int sockfd = bufferevent_getfd(bev);
    NetObject* pObject = new NetObject(this, sockfd, addr, bev);
    if (!AddNetObject(0, pObject))
    {
        assert(0);
        return -1;
    }

    mbServer = false;

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, (void*)pObject);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    event_set_log_callback(&NFCNet::log_cb);
    //event_base_loop(base, EVLOOP_ONCE|EVLOOP_NONBLOCK);

    return sockfd;
}

int NFCNet::InitServerNet()
{
    int nMaxClient = mnMaxConnect;
    int nCpuCount = mnCpuCount;
    int nPort = mnPort;

    struct sockaddr_in sin;

#ifdef _MSC_VER
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);

#endif
    //////////////////////////////////////////////////////////////////////////

    struct event_config* cfg = event_config_new();

#ifdef _MSC_VER

    //event_config_avoid_method(cfg, "iocp");
    //event_config_require_features(cfg, event_method_feature.EV_FEATURE_ET);//触发方式
    //evthread_use_windows_threads();
    //if(event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP) < 0)
    //{
    //    //使用IOCP
    //    return -1;
    //}

    //if(event_config_set_num_cpus_hint(cfg, nCpuCount) < 0)
    //{
    //    return -1;
    //}

    base = event_base_new_with_config(cfg);

#else

    //event_config_avoid_method(cfg, "epoll");
    if (event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST) < 0)
    {
        //使用EPOLL
        return -1;
    }

    if (event_config_set_num_cpus_hint(cfg, nCpuCount) < 0)
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

    listener = evconnlistener_new_bind(base, listener_cb, (void*)this,
                                       LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
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

    event_set_log_callback(&NFCNet::log_cb);

    return mnMaxConnect;
}

bool NFCNet::CloseSocketAll()
{
    std::map<int, NetObject*>::iterator it = mmObject.begin();
    for (it; it != mmObject.end(); ++it)
    {
        int nFD = it->first;
        mvRemoveObject.push_back(nFD);
    }

    ExecuteClose();

    mmObject.clear();

    return true;
}

NetObject* NFCNet::GetNetObject(const int nSockIndex)
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        return it->second;
    }

    return NULL;
}

void NFCNet::CloseObject(const int nSockIndex)
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        NetObject* pObject = it->second;

        struct bufferevent* bev = pObject->GetBuffEvent();

        bufferevent_free(bev);

        mmObject.erase(it);

        delete pObject;
        pObject = NULL;
    }
}

void NFCNet::ExecuteClose()
{
    for (int i = 0; i < mvRemoveObject.size(); ++i)
    {
        int nSocketIndex = mvRemoveObject[i];
        CloseObject(nSocketIndex);
    }

    mvRemoveObject.clear();
}

void NFCNet::log_cb(int severity, const char* msg)
{

}

bool NFCNet::IsServer()
{
	return mbServer;
}

bool NFCNet::Log(int severity, const char* msg)
{
    log_cb(severity, msg);
    return true;
}

bool NFCNet::SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const int nSockIndex /*= 0*/)
{
    std::string strOutData;
    int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
    if (nAllLen == nLen + NFIMsgHead::NF_Head::NF_HEAD_LENGTH)
    {
        //打包成功
        return SendMsg(strOutData.c_str(), strOutData.length(), nSockIndex);
    }

    return false;
}

bool NFCNet::SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const std::list<int>& fdList)
{
    std::string strOutData;
    int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
    if (nAllLen == nLen + NFIMsgHead::NF_Head::NF_HEAD_LENGTH)
    {
        //打包成功
        return SendMsg(strOutData.c_str(), strOutData.length(), fdList);
    }

    return false;
}

bool NFCNet::SendMsgToAllClientWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen)
{
    std::string strOutData;
    int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
    if (nAllLen == nLen + NFIMsgHead::NF_Head::NF_HEAD_LENGTH)
    {
        //打包成功
        return SendMsgToAllClient(strOutData.c_str(), strOutData.length());
    }

    return false;
}

int NFCNet::EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData)
{
    NFCMsgHead xHead;
    xHead.SetMsgID(unMsgID);
    xHead.SetBodyLength(unDataLen);

    char szHead[NFIMsgHead::NF_Head::NF_HEAD_LENGTH] = { 0 };
    xHead.EnCode(szHead);

    strOutData.clear();
    strOutData.append(szHead, NFIMsgHead::NF_Head::NF_HEAD_LENGTH);
    strOutData.append(strData, unDataLen);

    return xHead.GetBodyLength() + NFIMsgHead::NF_Head::NF_HEAD_LENGTH;
}

int NFCNet::DeCode(const char* strData, const uint32_t unAllLen, NFCMsgHead& xHead)
{
    //解密--unLen为buff总长度,解包时能用多少是多少
    if (unAllLen < NFIMsgHead::NF_Head::NF_HEAD_LENGTH)
    {
        //长度不够
        return -1;
    }


    if (NFIMsgHead::NF_Head::NF_HEAD_LENGTH != xHead.DeCode(strData))
    {
        //取包头失败
        return -2;
    }

    if (xHead.GetBodyLength() > unAllLen)
    {
        //总长度不够
        return -3;
    }

    //copy包头+包体
    //      strOutData.clear();
    //      strOutData.append(strData, xHead.GetMsgLength());

    //返回使用过的量
    return xHead.GetBodyLength();
}

bool NFCNet::AddReciveCallBack(const int nMsgID, const NET_RECIEVE_FUNCTOR_PTR& cb)
{
    std::map<int, NET_RECIEVE_FUNCTOR_PTR>::iterator it = mxReciveCallBack.find(nMsgID);
    if (mxReciveCallBack.end() == it)
    {
        mxReciveCallBack.insert(std::map<int, NET_RECIEVE_FUNCTOR_PTR>::value_type(nMsgID, cb));

        return true;
    }

    assert(nMsgID);

    return false;
}

bool NFCNet::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb)
{
    mxEventCallBack.push_back(cb);

    return true;
}
