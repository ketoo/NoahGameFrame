#include <thread>
#include "NFCHttpServer.h"


void NFCHttpServer::AddFilter(const HTTP_FILTER_FUNCTOR_PTR& ptr)
{
	mFilter = ptr;
}

bool NFCHttpServer::Execute()
{
    if (mxBase)
    {
        event_base_loop(mxBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
    }

    return true;
}


int NFCHttpServer::InitServer(const unsigned short port)
{
	/*
    event_init();  
    struct evhttp *httpserv = evhttp_start(addr,port);  
	evhttp_set_gencb(httpserv, reqHandler,NULL);
	event_dispatch();
	*/

    //struct event_base *base;
    struct evhttp* http;
    struct evhttp_bound_socket* handle;

#if NF_PLATFORM == NF_PLATFORM_WIN
    WSADATA WSAData;
    WSAStartup(0x101, &WSAData);
#else
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return (1);
#endif

    mxBase = event_base_new();
    if (!mxBase)
    {
        std::cout << "create event_base fail" << std::endl;;
        return 1;
    }

    http = evhttp_new(mxBase);
    if (!http)
    {
        std::cout << "create evhttp fail" << std::endl;;
        return 1;
    }

    handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);
    if (!handle)
    {
        std::cout << "bind port :" << port << " fail" << std::endl;;
        return 1;
    }

    evhttp_set_gencb(http, listener_cb, (void*) this);

    return 0;
}


void NFCHttpServer::listener_cb(struct evhttp_request* req, void* arg)
{
	std::cout << "threadid " << std::this_thread::get_id() << std::endl;

    NFCHttpServer* pNet = (NFCHttpServer*) arg;
    NFHttpRequest request;
    request.req = req;

	//headers
	struct evkeyvalq * header = evhttp_request_get_input_headers(req);
	struct evkeyval* kv = header->tqh_first;
	while (kv) 
	{
		request.headers.insert(std::map<std::string, std::string>::value_type(kv->key, kv->value));

		kv = kv->next.tqe_next;
	}

    //uri
    const char* uri = evhttp_request_get_uri(req);
	request.url = uri;
	request.remoteHost = evhttp_request_get_host(req);
	request.type = (NFHttpType)evhttp_request_get_command(req);

    //get decodeUri
    struct evhttp_uri* decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
		std::cout << "It's not a good URI. Sending BADREQUEST" << std::endl;
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }
    
	//path
	request.path = evhttp_uri_get_path(decoded);
	evhttp_uri_free(decoded);

	//std::cout << "Got a GET request:" << uri << std::endl;
	if (evhttp_request_get_command(req) == evhttp_cmd_type::EVHTTP_REQ_GET)
	{
		//OnGetProcess(request, );
		std::cout << "EVHTTP_REQ_GET" << std::endl;

		struct evkeyvalq params;
		evhttp_parse_query(uri, &params);
		struct evkeyval* kv = params.tqh_first;
		while (kv)
		{
			request.params.insert(std::map<std::string, std::string>::value_type(kv->key, kv->value));

			kv = kv->next.tqe_next;
		}
	}

	struct evbuffer *in_evb = evhttp_request_get_input_buffer(req);
	size_t len = evbuffer_get_length(in_evb);
	if (len > 0)
	{
		unsigned char *pData = evbuffer_pullup(in_evb, len);
		request.body.clear();
		request.body.append((const char *)pData, len);
	}

	if (pNet->mFilter)
	{
        HTTP_FILTER_FUNCTOR_PTR pFunc = pNet->mFilter;
		if (pFunc)
		{
			//return 401
			NFWebStatus xWebStatus = pFunc->operator()(request);
			if (xWebStatus != NFWebStatus::WEB_OK)
			{
				//401
				pNet->ResponseMsg(request, "Filter error", xWebStatus);
				return;
			}
		}
	}

    // call cb
    if (pNet->mReceiveCB)
    {
        pNet->mReceiveCB(request);
    }
	else
    {
        pNet->ResponseMsg(request, "NO PROCESSER", NFWebStatus::WEB_ERROR);
    }
}

bool NFCHttpServer::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
                                const std::string& strReason)
{
    evhttp_request* pHttpReq = (evhttp_request*) req.req;
    //create buffer
    struct evbuffer* eventBuffer = evbuffer_new();

    //send data
    evbuffer_add_printf(eventBuffer, strMsg.c_str());

    evhttp_add_header(evhttp_request_get_output_headers(pHttpReq), "Content-Type", "application/json");
	
	evhttp_send_reply(pHttpReq, code, strReason.c_str(), eventBuffer);

    //free
    evbuffer_free(eventBuffer);

    return true;
}

bool NFCHttpServer::Final()
{
    if (mxBase)
    {
        event_base_free(mxBase);
        mxBase = NULL;
    }

    return true;
}