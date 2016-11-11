/*******************************************************************
* HTTP helper
* Distributed under GPL license
* Copyright (c) 2005-06 Stanley Huang <stanleyhuangyc@yahoo.com.cn>
* All rights reserved.
*******************************************************************/

#ifndef _HTTPCLIENT_H
#define _HTTPCLIENT_H
#define FLAG_REQUEST_ONLY 0x1
#define FLAG_KEEP_ALIVE 0x2
#define FLAG_KEEP_HEADER 0x4
#define FLAG_CHUNKED 0x8

typedef enum {
	HS_IDLE=0,
	HS_REQUESTING,
	HS_RECEIVING,
	HS_STOPPING,
} HTTP_STATES;

typedef enum {
	HM_GET = 0,
	HM_HEAD,
	HM_POST,
	HM_POST_STREAM,
	HM_POST_MULTIPART,
} HTTP_METHOD;

#define postPayload_STRING 0
#define postPayload_BINARY 1
#define postPayload_FD 2
#define postPayload_CALLBACK 3

#define POST_BUFFER_SIZE 1024
typedef int (*PFNpostPayloadCALLBACK)(void* buffer, int bufsize);

typedef struct {
	void* data;
	int type;
	size_t length;
} POST_CHUNK;

typedef struct {
	unsigned short flags;
	HTTP_METHOD method;
	int sockfd;
	HTTP_STATES state;
	const char *url;
	const char *proxy;
	unsigned short port;
	char* referer;
	char* header;
	char* buffer;
	int bufferSize;
	char* postPayload;
	char* hostname;
	int postPayloadBytes;
	int dataSize;
	int bytesStart;
	int bytesEnd;
	int payloadSize;
	//info parsed from response header
	char* contentType;
	char* location;
	int httpVer;	// 0 for 1.0, 1 for 1.1
	int httpCode;
	//Multipart-Post 
	POST_CHUNK* chunk;
	int chunkCount;
	const char* filename;
} HTTP_REQUEST;

#ifdef __cplusplus
extern "C" {
#endif

void httpInitReq(HTTP_REQUEST* req, char* proxy);
int httpRequest(HTTP_REQUEST* param, const char* url);
int httpGetResponse(HTTP_REQUEST* param);
void httpClean(HTTP_REQUEST* param);
size_t httpSend(HTTP_REQUEST* param, char* data, size_t length);
int httpPostFile(HTTP_REQUEST* req, char* url, char* fieldname, const char* filename);
int PostFileStream(char* url, const char* filename);

#ifdef __cplusplus
}
#endif

#endif
