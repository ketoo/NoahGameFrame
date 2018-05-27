#include <thread>
#include "NFCHttpServer.h"

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
    /* 创建SSL上下文环境 ，可以理解为 SSL句柄 */
    /*
    SSL_CTX *ctx = SSL_CTX_new (SSLv23_server_method ());
    SSL_CTX_set_options (ctx,
            SSL_OP_SINGLE_DH_USE |
            SSL_OP_SINGLE_ECDH_USE |
            SSL_OP_NO_SSLv2);
*/
    /* Cheesily pick an elliptic curve to use with elliptic curve ciphersuites.
     * We just hardcode a single curve which is reasonably decent.
     * See http://www.mail-archive.com/openssl-dev@openssl.org/msg30957.html */
	/*
    EC_KEY *ecdh = EC_KEY_new_by_curve_name (NID_X9_62_prime256v1);
    if (! ecdh)
        die_most_horribly_from_openssl_error ("EC_KEY_new_by_curve_name");
    if (1 != SSL_CTX_set_tmp_ecdh (ctx, ecdh))
        die_most_horribly_from_openssl_error ("SSL_CTX_set_tmp_ecdh");
*/
    /* 选择服务器证书 和 服务器私钥. */
	/*
    const char *certificate_chain = "server-certificate-chain.pem";
    const char *private_key = "server-private-key.pem";
    */
    /* 设置服务器证书 和 服务器私钥 到 
     OPENSSL ctx上下文句柄中 */
    //server_setup_certs (ctx, certificate_chain, private_key);

    /* 
        使我们创建好的evhttp句柄 支持 SSL加密
        实际上，加密的动作和解密的动作都已经帮
        我们自动完成，我们拿到的数据就已经解密之后的
    */
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
	NFCHttpServer* pNet = (NFCHttpServer*)arg;
	NFHttpRequest* pRequest = pNet->AllowHttpRequest();

	pRequest->req = req;

	//headers
	struct evkeyvalq * header = evhttp_request_get_input_headers(req);
	struct evkeyval* kv = header->tqh_first;
	while (kv)
	{
		pRequest->headers.insert(std::map<std::string, std::string>::value_type(kv->key, kv->value));

		kv = kv->next.tqe_next;
	}

	//uri
	const char* uri = evhttp_request_get_uri(req);
	pRequest->url = uri;
	pRequest->remoteHost = evhttp_request_get_host(req);
	pRequest->type = (NFHttpType)evhttp_request_get_command(req);

	//get decodeUri
	struct evhttp_uri* decoded = evhttp_uri_parse(uri);
	if (!decoded)
	{
		std::cout << "It's not a good URI. Sending BADREQUEST" << std::endl;
		evhttp_send_error(req, HTTP_BADREQUEST, 0);

		pNet->mxHttpRequestPool.push_back(pRequest);
		
		return;
	}

	//path
	pRequest->path = evhttp_uri_get_path(decoded);
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
			pRequest->params.insert(std::map<std::string, std::string>::value_type(kv->key, kv->value));

			kv = kv->next.tqe_next;
		}
	}

	struct evbuffer *in_evb = evhttp_request_get_input_buffer(req);
	size_t len = evbuffer_get_length(in_evb);
	if (len > 0)
	{
		unsigned char *pData = evbuffer_pullup(in_evb, len);
		pRequest->body.clear();
		pRequest->body.append((const char *)pData, len);
	}

	if (pNet->mFilter)
	{
		//return 401
		NFWebStatus xWebStatus = pNet->mFilter(*pRequest);
		if (xWebStatus != NFWebStatus::WEB_OK)
		{

			pNet->mxHttpRequestPool.push_back(pRequest);

			//401
			pNet->ResponseMsg(*pRequest, "Filter error", xWebStatus);
			return;
		}
	}

	// call cb
	if (pNet->mReceiveCB)
	{
		pNet->mReceiveCB(*pRequest);
	}
	else
	{
		pNet->ResponseMsg(*pRequest, "NO PROCESSER", NFWebStatus::WEB_ERROR);
	}
}

NFHttpRequest* NFCHttpServer::AllowHttpRequest()
{
	if (mxHttpRequestPool.size() <= 0)
	{
		for (int i = 0; i < 100; ++i)
		{
			mxHttpRequestPool.push_back(NF_NEW NFHttpRequest());
		}
	}

	NFHttpRequest* pRequest = mxHttpRequestPool.front();
	mxHttpRequestPool.pop_front();
	pRequest->Reset();

	pRequest->id = ++mnIndex;

	return pRequest;
}

bool NFCHttpServer::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code,
                                const std::string& strReason)
{
	auto it = mxHttpRequestMap.find(req.id);
	if (it != mxHttpRequestMap.end())
	{
		mxHttpRequestPool.push_back(it->second);
		mxHttpRequestMap.erase(it);
	}

	evhttp_request* pHttpReq = (evhttp_request*)req.req;
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
