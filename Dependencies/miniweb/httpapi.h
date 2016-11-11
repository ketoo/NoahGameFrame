////////////////////////////////////////////////////////////////////////
//
// httpapi.h
//
// External API header file for http protocol
//
///////////////////////////////////////////////////////////////////////

#ifndef _HTTPAPI_H_
#define _HTTPAPI_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "httppil.h"

#define VER_MAJOR 0
#define VER_MINOR 8

#ifndef min
#define min(x,y) (x>y?y:x)
#endif

#ifdef HTTP_DEBUG
#define DBG printf
#else
#define DBG
#endif
#define LOG_ERR 1

#define ASSERT
#define GETDWORD(ptrData) (*(DWORD*)(ptrData))
#define SETDWORD(ptrData,data) (*(DWORD*)(ptrData)=data)
#define GETWORD(ptrData) (*(WORD*)(ptrData))
#define SETWORD(ptrData,data) (*(WORD*)(ptrData)=data)
#ifndef BIG_ENDINE
#define DEFDWORD(char1,char2,char3,char4) ((char1)+((char2)<<8)+((char3)<<16)+((char4)<<24))
#define DEFWORD(char1,char2) (char1+(char2<<8))
#else
#define DEFDWORD(char1,char2,char3,char4) ((char4)+((char3)<<8)+((char2)<<16)+((char1)<<24))
#define DEFWORD(char1,char2) (char2+(char1<<8))
#endif

///////////////////////////////////////////////////////////////////////
// Public definitions
///////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// file types
typedef enum {
  HTTPFILETYPE_UNKNOWN = 0,
  HTTPFILETYPE_HTML,
  HTTPFILETYPE_XML,
  HTTPFILETYPE_TEXT,
  HTTPFILETYPE_XUL,
  HTTPFILETYPE_CSS,
  HTTPFILETYPE_JS,
  HTTPFILETYPE_PNG,
  HTTPFILETYPE_JPEG,
  HTTPFILETYPE_GIF,
  HTTPFILETYPE_SWF,
  HTTPFILETYPE_MPA,
  HTTPFILETYPE_MPEG,
  HTTPFILETYPE_AVI,
  HTTPFILETYPE_MP4,
  HTTPFILETYPE_MOV,
  HTTPFILETYPE_264,
  HTTPFILETYPE_FLV,
  HTTPFILETYPE_TS,
  HTTPFILETYPE_3GP,
  HTTPFILETYPE_ASF,
  HTTPFILETYPE_OCTET,
  HTTPFILETYPE_STREAM,
  HTTPFILETYPE_M3U8,
  HTTPFILETYPE_SDP,
  HTTPFILETYPE_HEX,
} HttpFileType;

#define MAXPOSTPARAMS 50
#define MAXPOSTREDIRECTFILENAME (200)

/////////////////////////////////////////////////////////////////////////////
// typedefs
/////////////////////////////////////////////////////////////////////////////

typedef struct _tagPostParam {
  //  char* pchPageName;
  struct {
    char* pchParamName;
    char* pchParamValue;
  } stParams[MAXPOSTPARAMS];
  void *httpParam;
  int iNumParams;
  char *pchPath;
} PostParam;

// multipart file upload post (per socket) structure
typedef struct {
  char pchBoundaryValue[80];
  OCTET oFileuploadStatus;
  size_t writeLocation;
  PostParam pp;
  char pchFilename[MAX_PATH];
  void *pxCallBackData;
} HttpMultipart;

typedef struct _tagSubstParam {
  char* pchParamName;
  char* pchParamValue;	// returned
  int iMaxValueBytes;
} SubstParam;

#define FLAG_REQUEST_GET		0x1
#define FLAG_REQUEST_POST		0x2
#ifdef ENABLE_RTSP
#define FLAG_REQUEST_OPTIONS	0x4
#define FLAG_REQUEST_DESCRIBE	0x8
#define FLAG_REQUEST_SETUP		0x10
#define FLAG_REQUEST_PLAY		0x20
#define FLAG_REQUEST_TEARDOWN	0x40
#endif
#define FLAG_HEADER_SENT		0x80
#define FLAG_CONN_CLOSE			0x100
#define FLAG_SUBST				0x200
#define FLAG_AUTHENTICATION		0x400
#define FLAG_MORE_CONTENT		0x800
#define FLAG_TO_FREE			0x1000
#define FLAG_CHUNK				0x2000
#define FLAG_CLOSE_CALLBACK     0x4000

#define FLAG_DATA_FILE		0x10000
#define FLAG_DATA_RAW		0x20000
#define FLAG_DATA_FD		0x40000
#define FLAG_DATA_REDIRECT	0x80000
#define FLAG_DATA_STREAM	0x100000
#define FLAG_CUSTOM_HEADER	0x200000
#define FLAG_MULTIPART		0x400000

#define FLAG_RECEIVING		0x80000000
#define FLAG_SENDING		0x40000000

#define SETFLAG(hs,bit) (hs->flags|=(bit));
#define CLRFLAG(hs,bit) (hs->flags&=~(bit));
#define ISFLAGSET(hs,bit) (hs->flags&(bit))

typedef union {
	unsigned long laddr;
	unsigned short saddr[2];
	unsigned char caddr[4];
} IPADDR;

typedef struct {
	int iHttpVer;
	size_t startByte;
	char *pucPath;
	const char *pucReferer;
	char* pucHost;
	int headerSize;
	char* pucPayload;
	size_t payloadSize;
	int iCSeq;
	const char* pucTransport;
	const char* pucAuthInfo;
} HttpRequest;

typedef struct {
	int statusCode;
	int headerBytes;
	int sentBytes;
	size_t contentLength;
	HttpFileType fileType;
} HttpResponse;

typedef struct {
	char *name;
	char *value;
} HttpVariables;

// Callback function protos
typedef int (*PFNPOSTCALLBACK)(PostParam*);
typedef int (*PFNSUBSTCALLBACK)(SubstParam*);
typedef int (*PFNFILEUPLOADCALLBACK)(HttpMultipart*, OCTET*, size_t);
typedef int (*PFNIDLECALLBACK)(void* hp);

typedef enum {
	MW_INIT = 0,
	MW_UNINIT,
	MW_PARSE_ARGS,
} MW_EVENT;

typedef int (*PFNEVENTHANDLER)(MW_EVENT msg, void* handler, void* hp);

typedef struct {
	time_t startTime;
	WORD clientCount;
	WORD clientCountMax;
	size_t reqCount;
	size_t fileSentCount;
	size_t fileSentBytes;
	int varSubstCount;
	int urlProcessCount;
	int timeOutCount;
	int authFailCount;
	int fileUploadCount;
} HttpStats;

#define HTTP_BUFFER_SIZE (128*1024 /*bytes*/)
#define HTTP_MAX_CLIENTS 32

// per connection/socket structure
typedef struct _HttpSocket{
	SOCKET socket;
	IPADDR ipAddr;

	HttpRequest request;
	HttpResponse response;
	char *pucData;
	int bufferSize;			// the size of buffer pucData pointing to
	int dataLength;
#ifdef WINCE
	HANDLE fd;
#else
	int fd;
#endif
	unsigned int flags;
	void* handler;				// http handler function address
	void* ptr;
	time_t tmAcceptTime;
	time_t tmExpirationTime;
	DWORD dwResumeTick;
	int iRequestCount;
	char* mimeType;
	HttpMultipart* pxMP;
	char* buffer;
} HttpSocket;

typedef struct {
	void* hp;
	HttpSocket* hs;
	const char *pucRequest;
	HttpVariables* pxVars;
	int iVarCount;
	char *pucHeader;
	char *pucBuffer;
	char *pucPayload;
	int dataBytes;
	int contentBytes;
	HttpFileType fileType;
	void *p_sys;
} UrlHandlerParam;

typedef int (*PFNURLCALLBACK)(UrlHandlerParam*);

typedef struct {
	const char* pchUrlPrefix;
	PFNURLCALLBACK pfnUrlHandler;
	PFNEVENTHANDLER pfnEventHandler;
	void *p_sys;
} UrlHandler;

#define AUTH_NO_NEED (0)
#define AUTH_SUCCESSED (1)
#define AUTH_REQUIRED (2)
#define AUTH_FAILED (-1)

#define MAX_AUTH_INFO_LEN 64

typedef struct {
	const char* pchUrlPrefix;
	const char* pchUsername;
	const char* pchPassword;
	const char* pchOtherInfo;
	char pchAuthString[MAX_AUTH_INFO_LEN];
} AuthHandler;

#ifndef DISABLE_VIRTUAL_PATH
typedef struct {
	char* pchUrlPrefix;
	char pchLocalRealPath[MAX_PATH];
} VirtPathHandler;
#endif

#define FLAG_DIR_LISTING 1
#define FLAG_DISABLE_RANGE 2

typedef struct _httpParam {
	HttpSocket* hsSocketQueue;				/* socket queue*/
	int maxClients;
	int maxClientsPerIP;
	int bKillWebserver;
	int bWebserverRunning;
	unsigned int flags;
	SOCKET listenSocket;
	int httpPort;
	int socketRcvBufSize;	/* socket receive buffer size in KB */
	char pchWebPath[128];
	UrlHandler *pxUrlHandler;		/* pointer to URL handler array */
	AuthHandler *pxAuthHandler;     /* pointer to authorization handler array */
#ifndef DISABLE_VIRTUAL_PATH
	VirtPathHandler *pxVirtPathHandler;
#endif
	// substitution callback
	PFNSUBSTCALLBACK pfnSubst;
	// post callbacks
	PFNFILEUPLOADCALLBACK pfnFileUpload;
	PFNPOSTCALLBACK pfnPost;
	// idle callback
	PFNIDLECALLBACK pfnIdleCallback;
	// misc
	DWORD dwAuthenticatedNode;
	time_t tmAuthExpireTime;
	time_t tmSocketExpireTime;
	int maxDownloadSpeed; /* maximum download speed in KB/s */
	HttpStats stats;
	u_long hlBindIP;
	void* szctx;
} HttpParam;

typedef struct {
	const char* pchRootPath;
	const char* pchHttpPath;
	char cFilePath[MAX_PATH];
	char* pchExt;
	int fTailSlash;
} HttpFilePath;

///////////////////////////////////////////////////////////////////////
// Return codes
///////////////////////////////////////////////////////////////////////
// for post callback
#define WEBPOST_OK                (0)
#define WEBPOST_AUTHENTICATED     (1)
#define WEBPOST_NOTAUTHENTICATED  (2)
#define WEBPOST_AUTHENTICATIONON  (3)
#define WEBPOST_AUTHENTICATIONOFF (4)

// for multipart file uploads
#define HTTPUPLOAD_MORECHUNKS     (0)
#define HTTPUPLOAD_FIRSTCHUNK     (1)
#define HTTPUPLOAD_LASTCHUNK      (2)

///////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// mwInitParam. Init the context structure with default values
///////////////////////////////////////////////////////////////////////
void mwInitParam(HttpParam* hp);

///////////////////////////////////////////////////////////////////////
// mwServerStart. Startup the webserver
///////////////////////////////////////////////////////////////////////
int mwServerStart(HttpParam* hp);

///////////////////////////////////////////////////////////////////////
// mwHttpLoop. Enter webserver loop
///////////////////////////////////////////////////////////////////////
void* mwHttpLoop(void* _hp);

///////////////////////////////////////////////////////////////////////
// mwServerShutdown. Shutdown the webserver (closes connections and
// releases resources)
///////////////////////////////////////////////////////////////////////
int mwServerShutdown(HttpParam* hp);

///////////////////////////////////////////////////////////////////////
// mwSetRcvBufSize. Change the TCP windows size of acceped sockets
///////////////////////////////////////////////////////////////////////
int mwSetRcvBufSize(WORD wSize);

///////////////////////////////////////////////////////////////////////
// mwPostRegister. Specify the callback to be called when a POST is
// recevied.
///////////////////////////////////////////////////////////////////////
PFNPOSTCALLBACK mwPostRegister(HttpParam *httpParam, PFNPOSTCALLBACK);

///////////////////////////////////////////////////////////////////////
// mwFileUploadRegister. Specify the callback to be called whenever the
// server has the next data chunk available from a multipart file upload.
///////////////////////////////////////////////////////////////////////
PFNFILEUPLOADCALLBACK mwFileUploadRegister(HttpParam *httpParam, PFNFILEUPLOADCALLBACK);

///////////////////////////////////////////////////////////////////////
// Default subst, post and file-upload callback processing
///////////////////////////////////////////////////////////////////////
int DefaultWebSubstCallback(SubstParam* sp);
int DefaultWebPostCallback(PostParam* pp);
int DefaultWebFileUploadCallback(HttpMultipart *pxMP, OCTET *poData, size_t dataChunkSize);

int mwGetHttpDateTime(time_t tm, char *buf, int bufsize);
int mwGetLocalFileName(HttpFilePath* hfp);
char* mwGetVarValue(HttpVariables* vars, const char *varname, const char *defval);
int mwGetVarValueInt(HttpVariables* vars, const char *varname, int defval);
unsigned int mwGetVarValueHex(HttpVariables* vars, const char *varname, unsigned int defval);
int mwParseQueryString(UrlHandlerParam* up);
int mwGetContentType(const char *pchExtname);
void mwDecodeString(char* s);

#ifdef __cplusplus
}
#endif

#endif // _HTTPAPI_H

////////////////////////// END OF FILE ////////////////////////////////
