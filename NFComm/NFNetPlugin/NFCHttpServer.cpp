#include "NFCHttpServer.h"

bool NFCHttpServer::Execute()
{
	if (base)
	{
		event_base_loop(base, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}

	return true;
}


int NFCHttpServer::InitServer(const unsigned short port)
{
	mPort = port;
	//struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;

#if NF_PLATFORM == NF_PLATFORM_WIN
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return (1);
#endif

	base = event_base_new();
	if (!base)
	{
		std::cout << "create event_base fail" << std::endl;;
		return 1;
	}

	http = evhttp_new(base);
	if (!http) {
		std::cout << "create evhttp fail" << std::endl;;
		return 1;
	}

	evhttp_set_gencb(http, listener_cb, (void*)this);
	handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", mPort);
	if (!handle) {
		std::cout << "bind port :" << mPort << " fail" << std::endl;;
		return 1;
	}
	return 0;
}


void NFCHttpServer::listener_cb(struct evhttp_request *req, void *arg)
{
	NFCHttpServer* pNet = (NFCHttpServer*)arg;
	NFHttpRequest request;
	request.req = req;
	//uri
	const char *uri = evhttp_request_get_uri(req);
	//std::cout << "Got a GET request:" << uri << std::endl;

	//get decodeUri
	struct evhttp_uri *decoded = evhttp_uri_parse(uri);
	if (!decoded) {
		printf("It's not a good URI. Sending BADREQUEST\n");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}
	const char *decode1 = evhttp_uri_get_path(decoded);
	if (!decode1) decode1 = "/";

	//The returned string must be freed by the caller.
	const char* decodeUri = evhttp_uridecode(decode1, 0, NULL);

	if (decodeUri == NULL)
	{
		printf("uri decode error\n");
		evhttp_send_error(req, HTTP_BADREQUEST, "uri decode error");
		return;
	}
	std::string strUri;
	if (decodeUri[0] == '/')
	{
		strUri = decodeUri;
		strUri.erase(0, 1);
		decodeUri = strUri.c_str();
	}
	//get strCommand
	auto cmdList = Split(strUri, "/");
	std::string strCommand = "";
	if (cmdList.size() > 0)
	{
		strCommand = cmdList[0];
	}

	request.url = decodeUri;

	// call cb
	if (pNet->mRecvCB)
	{
		pNet->mRecvCB(request, strCommand, decodeUri);
	}
	else
	{
		pNet->ResponseMsg(request, "mRecvCB empty", NFWebStatus::WEB_ERROR);
	}



	//close
	/*{
	if (decoded)
	evhttp_uri_free(decoded);
	if (decodeUri)
	free(decodeUri);
	if (eventBuffer)
	evbuffer_free(eventBuffer);
	}*/
}
bool NFCHttpServer::ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason)
{
	evhttp_request* pHttpReq = (evhttp_request*)req.req;
	//create buffer
	struct evbuffer *eventBuffer = evbuffer_new();
	//send data
	evbuffer_add_printf(eventBuffer, strMsg.c_str());
	evhttp_add_header(evhttp_request_get_output_headers(pHttpReq), "Content-Type", "text/html");
	evhttp_send_reply(pHttpReq, code, strReason.c_str(), eventBuffer);

	//free
	evbuffer_free(eventBuffer);
	return true;
}

bool NFCHttpServer::ResponseFile(const NFHttpRequest & req, const std::string & strPath, const std::string & strFileName)
{
	//Add response type
	std::map<std::string, std::string> typeMap;
	typeMap["txt"] = "text/plain";
	typeMap["txt"] = "text/plain";
	typeMap["c"] = "text/plain";
	typeMap["h"] = "text/plain";
	typeMap["html"] = "text/html";
	typeMap["htm"] = "text/htm";
	typeMap["css"] = "text/css";
	typeMap["gif"] = "image/gif";
	typeMap["jpg"] = "image/jpeg";
	typeMap["jpeg"] = "image/jpeg";
	typeMap["png"] = "image/png";
	typeMap["pdf"] = "application/pdf";
	typeMap["ps"] = "application/postsript";

	std::string strFilePath = strPath;
	if (strFilePath.find_last_of("/") != strFilePath.size())
	{
		strFilePath += "/";
	}

	strFilePath = strFilePath + req.url;

	int fd = -1;
	struct stat st;
	if (stat(strFilePath.c_str(), &st) < 0)
	{
		std::string errMsg = strFilePath + strFileName;
		ResponseMsg(req, errMsg.c_str(), NFWebStatus::WEB_ERROR, errMsg.c_str());

		return false;
	}

	if (S_ISDIR(st.st_mode))
	{
		strFilePath += "/" + strFileName;
	}

	if (stat(strFilePath.c_str(), &st) < 0)
	{
		std::string errMsg = strFilePath + strFilePath;
		ResponseMsg(req, errMsg.c_str(), NFWebStatus::WEB_ERROR, errMsg.c_str());
		return false;
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	if ((fd = open(strFilePath.c_str(), O_RDONLY | O_BINARY)) < 0) {
#else
	if ((fd = open(strFilePath.c_str(), O_RDONLY)) < 0) {
#endif
		ResponseMsg(req, "error", NFWebStatus::WEB_ERROR, "error");
		return false;
	}

	if (fstat(fd, &st) < 0) {
		ResponseMsg(req, "error", NFWebStatus::WEB_ERROR, "error");
		return false;
	}

	const char* last_period = strrchr(strFilePath.c_str(), '.');
	std::string strType = last_period + 1;
	if (typeMap.find(strType) == typeMap.end())
	{
		strType = "application/misc";
	}
	else
	{
		strType = typeMap[strType];
	}
	ResponseFile(req, fd, st, strType);
	return false;
}

bool NFCHttpServer::ResponseFile(const NFHttpRequest& req, const int fd, struct stat st, const std::string& strType)
{
	evhttp_request* pHttpReq = (evhttp_request*)req.req;

	//create buffer
	struct evbuffer *eventBuffer = evbuffer_new();
	//send data
	evbuffer_add_file(eventBuffer, fd, 0, st.st_size);
	evhttp_add_header(evhttp_request_get_output_headers(pHttpReq), "Content-Type", strType.c_str());
	evhttp_send_reply(pHttpReq, 200, "OK", eventBuffer);

	//free
	evbuffer_free(eventBuffer);
	return true;
}


bool NFCHttpServer::Final()
{
	if (base)
	{
		event_base_free(base);
		base = NULL;
	}
	return true;
}

std::vector<std::string> NFCHttpServer::Split(const std::string& str, std::string delim)
{
	std::vector<std::string> result;
	if (str.empty() || delim.empty())
	{
		return result;
	}

	std::string tmp;
	size_t pos_begin = str.find_first_not_of(delim);
	size_t pos = 0;
	while (pos_begin != std::string::npos)
	{
		pos = str.find(delim, pos_begin);
		if (pos != std::string::npos)
		{
			tmp = str.substr(pos_begin, pos - pos_begin);
			pos_begin = pos + delim.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = pos;
		}

		if (!tmp.empty())
		{
			result.push_back(tmp);
			tmp.clear();
		}
	}
	return result;
}
