/////////////////////////////////////////////////////////////////////////////
//
// http.c
//
// MiniWeb - mini webserver implementation
//
/////////////////////////////////////////////////////////////////////////////
#ifndef WINCE
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "httpapi.h"
#include "httpint.h"

#ifdef _7Z
#include "7zDec/7zInc.h"
#endif

////////////////////////////////////////////////////////////////////////////
// global variables
////////////////////////////////////////////////////////////////////////////
const char* status200[] = {
	"OK",		/* 200 */
	"Created",	/* 201 */
	"Accepted", /* 202 */
	"Non-Authoritative Information", /* 203 */
	"No Content", /* 204 */
	"Reset Content", /* 205 */
	"Partial Content", /* 206 */
};

const char* status300[] = {
	"Multiple Choices", /* 300 */
	"Moved Permanently", /* 301 */
	"Found", /* 302 */
	"See Other", /* 303 */
	"Not Modified", /* 304 */
	"Use Proxy", /* 305 */
	"", /* 306 */
	"Temporary Redirect", /* 307 */
};

const char* status400[] = {
	"Bad Request", /* 400 */
	"Unauthorized", /* 401 */
	"", /* 402 */
	"Forbidden", /* 403 */
	"Not Found", /* 404 */
	"Method Not Allowed", /* 405 */
	"Not Acceptable", /* 406 */
	"Proxy Authentication Required", /* 407 */
	"Request Timeout", /* 408 */
	"Conflict", /* 409 */
	"Gone", /* 410 */
	"Length Required", /* 411 */
	"Precondition Failed", /* 412 */
	"Request Entity Too Large", /* 413 */
	"Request-URI Too Long", /* 414 */
};

const char* status500[] = {
	"Internal Server Error", /* 500 */
	"Not Implemented", /* 501 */
	"Bad Gateway", /* 502 */
	"Service Unavailable", /* 503 */
	"Gateway Timeout", /* 504 */
};

const char* contentTypeTable[]={
	"application/octet-stream",
	"text/html",
	"text/xml",
	"text/plain",
	"application/vnd.mozilla.xul+xml",
	"text/css",
	"application/x-javascript",
	"image/png",
	"image/jpeg",
	"image/gif",
	"application/x-shockwave-flash",
	"audio/mpeg",
	"video/mpeg",
	"video/avi",
	"video/mp4",
	"video/quicktime",
	"video/x-mpeg-avc",
	"video/flv",
	"video/MP2T",
	"video/3gpp",
	"video/x-ms-asf",
	"application/octet-stream",
	"application/x-datastream",
	"application/x-mpegURL",
	"application/sdp",
	"application/binhex",
};

char* defaultPages[]={"index.htm","index.html","default.htm","main.xul"};

FILE *fpLog=NULL;

////////////////////////////////////////////////////////////////////////////
// API callsc
////////////////////////////////////////////////////////////////////////////

const char *dayNames="Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat";
const char *monthNames="Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec";
const char *httpDateTimeFormat="%s, %02d %s %d %02d:%02d:%02d GMT";

char* mwGetVarValue(HttpVariables* vars, const char *varname, const char *defval)
{
	int i;
	if (vars && varname) {
		for (i=0; (vars+i)->name; i++) {
			if (!strcmp((vars+i)->name,varname)) {
				return (vars+i)->value;
			}
		}
	}
	return (char*)defval;
}

int mwGetVarValueInt(HttpVariables* vars, const char *varname, int defval)
{
	int i;
	if (vars && varname) {
		for (i=0; (vars+i)->name; i++) {
			if (!strcmp((vars+i)->name,varname)) {
				char *p = (vars+i)->value;
				return p ? atoi(p) : defval;
			}
		}
	}
	return defval;
}

static unsigned int hex2uint32(const char *p)
{
	register char c;
	register unsigned int i = 0;
	for(c=*p;;){
		if (c>='A' && c<='F')
			c-=7;
		else if (c>='a' && c<='f')
			c-=39;
		else if (c<'0' || c>'9')
			break;
		i=(i<<4)|(c&0xF);
		c=*(++p);
	}
	return i;
}

unsigned int mwGetVarValueHex(HttpVariables* vars, const char *varname, unsigned int defval)
{
	int i;
	if (vars && varname) {
		for (i=0; (vars+i)->name; i++) {
			if (!strcmp((vars+i)->name,varname)) {
				char *p = (vars+i)->value;
				return p ? hex2uint32(p) : defval;
			}
		}
	}
	return defval;
}

int mwGetHttpDateTime(time_t timer, char *buf, int bufsize)
{
#ifndef WINCE
	struct tm *btm;
	btm=gmtime(&timer);
	return snprintf(buf, bufsize, httpDateTimeFormat,
		dayNames+(btm->tm_wday<<2),
		btm->tm_mday,
		monthNames+(btm->tm_mon<<2),
		1900+btm->tm_year,
		btm->tm_hour,
		btm->tm_min,
		btm->tm_sec);
#else
	return 0;
#endif
}

void mwInitParam(HttpParam* hp)
{
	memset(hp, 0, sizeof(HttpParam));
	hp->httpPort = 80;
	hp->tmSocketExpireTime = 60;
	hp->maxClients = HTTP_MAX_CLIENTS;
}

////////////////////////////////////////////////////////////////////////////
// mwServerStart
// Start the webserver
////////////////////////////////////////////////////////////////////////////
int mwServerStart(HttpParam* hp)
{
	if (hp->bWebserverRunning) {
		DBG("Error: Webserver thread already running\n");
		return -1;
	}

	if (!fpLog) fpLog=stderr;

	if (hp->maxClients == 0) {
		SYSLOG(LOG_INFO,"Maximum clients not set\n");
		return -1;
	}
	hp->hsSocketQueue = calloc(hp->maxClients, sizeof(HttpSocket));

	if (hp->pxUrlHandler) {
		int i;
		for (i=0; hp->pxUrlHandler[i].pchUrlPrefix;i++) {
			if (hp->pxUrlHandler[i].pfnEventHandler &&
				hp->pxUrlHandler[i].pfnEventHandler(MW_INIT, &hp->pxUrlHandler[i], hp)) {
				//remove the URL handler
				hp->pxUrlHandler[i].pfnUrlHandler=NULL;
			}
		}
	}

#ifdef _7Z
	hp->szctx = SzInit();
#endif

	if (!(hp->listenSocket=_mwStartListening(hp))) {
		DBG("Error listening on port %d\n", hp->httpPort);
		return -1;
	}

#ifndef WINCE
	hp->stats.startTime=time(NULL);
#else
	hp->stats.startTime = GetTickCount()>>10;
#endif
	hp->bKillWebserver=FALSE;
	hp->bWebserverRunning=TRUE;
	if (!hp->tmSocketExpireTime) hp->tmSocketExpireTime = HTTP_EXPIRATION_TIME;
	return 0;
}

////////////////////////////////////////////////////////////////////////////
// mwServerShutdown
// Shutdown the webserver
////////////////////////////////////////////////////////////////////////////
int mwServerShutdown(HttpParam* hp)
{
	int i;
	if (hp->bKillWebserver || !hp->bWebserverRunning) return -1;

	DBG("Shutting down...\n");
	_mwCloseAllConnections(hp);

	// signal webserver thread to quit
	hp->bKillWebserver=TRUE;

	// and wait for thread to exit
	for (i=0;hp->bWebserverRunning && i<30;i++) msleep(100);

#ifdef _7Z
	SzUninit(hp->szctx);
#endif

	if (!hp->bWebserverRunning)
		DBG("Webserver shutdown complete\n");

	return 0;
}

int mwGetLocalFileName(HttpFilePath* hfp)
{
	char ch;
	char *p = (char*)hfp->cFilePath;
	char *s = (char*)hfp->pchHttpPath;
	char *upLevel = NULL;

	hfp->pchExt=NULL;
	hfp->fTailSlash=0;
	if (*s == '~') {
		s++;
	} else if (hfp->pchRootPath) {
		p+=_mwStrCopy(hfp->cFilePath,hfp->pchRootPath);
		if (*(p-1)!=SLASH) {
			*p=SLASH;
			*(++p)=0;
		}
	}
	while ((ch=*s) && ch!='?' && (int)(p-hfp->cFilePath)<sizeof(hfp->cFilePath)-1) {
		if (ch=='%') {
			*(p++) = _mwDecodeCharacter(++s);
			s += 2;
		} else if (ch=='/') {
			*p=SLASH;
			upLevel=(++p);
			while (*(++s)=='/');
			continue;
		} else if (ch=='+') {
			*(p++)=' ';
			s++;
		} else if (ch=='.') {
			if (upLevel && !memcmp(s+1, "./", 2)) {
				s+=2;
				p=upLevel;
			} else {
				*(p++)='.';
				hfp->pchExt=p;
				while (*(++s)=='.');	//avoid '..' appearing in filename for security issue
			}
		} else {
			*(p++)=*(s++);
		}
	}
	if (*(p-1)==SLASH) {
		p--;
		hfp->fTailSlash=1;
	}
	*p=0;
	return (int)(p - hfp->cFilePath);
}

////////////////////////////////////////////////////////////////////////////
// Internal (private) helper functions
////////////////////////////////////////////////////////////////////////////

SOCKET _mwStartListening(HttpParam* hp)
{
	SOCKET listenSocket;
	int iRc;

    // create a new socket
    listenSocket=socket(AF_INET,SOCK_STREAM,0);
	if (listenSocket <= 0) {
		DBG("Error creating socket\n");
		return 0;
	}

#if 0
    // allow reuse of port number
    {
      int iOptVal=1;
      iRc=setsockopt(listenSocket,SOL_SOCKET,SO_REUSEADDR,
                     (char*)&iOptVal,sizeof(iOptVal));
      if (iRc<0) return 0;
    }
#endif

    // bind it to the http port
    {
      struct sockaddr_in sinAddress;
      memset(&sinAddress,0,sizeof(struct sockaddr_in));
      sinAddress.sin_family=AF_INET;
	  // INADDR_ANY is 0
	  //sinAddress.sin_addr.s_addr=htonl(hp->dwBindIP);
	  sinAddress.sin_addr.s_addr = hp->hlBindIP;
      sinAddress.sin_port = htons(hp->httpPort); // http port
      iRc=bind(listenSocket,(struct sockaddr*)&sinAddress,
               sizeof(struct sockaddr_in));
	  if (iRc<0) {
		DBG("Error binding on port %d\n",hp->httpPort);
		return 0;
	  }
    }

#ifndef WIN32
    // set to non-blocking to avoid lockout issue (see Section 15.6
    // in Unix network programming book)
    {
      int iSockFlags;
      iSockFlags = fcntl(listenSocket, F_GETFL, 0);
      iSockFlags |= O_NONBLOCK;
      iRc = fcntl(listenSocket, F_SETFL, iSockFlags);
    }
#endif

    // listen on the socket for incoming calls
	DBG("Attempting to listening on port %d with socket %d\n", hp->httpPort, listenSocket);
	if (listen(listenSocket,hp->maxClients-1)) {
		DBG("Unable to listen on socket %d\n",listenSocket);
		return 0;
	}

    DBG("Http listen socket %d opened\n",listenSocket);
	return listenSocket;
}

void _mwInitSocketData(HttpSocket *phsSocket)
{
	memset(&phsSocket->response,0,sizeof(HttpResponse));
	if (!phsSocket->buffer)
		phsSocket->buffer = malloc(HTTP_BUFFER_SIZE);
	phsSocket->request.startByte = 0;
	phsSocket->request.pucHost = 0;
	phsSocket->request.pucReferer = 0;
	phsSocket->request.pucTransport = 0;
	phsSocket->request.pucPath = 0;
	phsSocket->request.headerSize = 0;
	phsSocket->request.payloadSize = 0;
	phsSocket->request.iCSeq = 0;
	phsSocket->request.pucAuthInfo = NULL;
	phsSocket->response.statusCode = 200;
	phsSocket->fd = 0;
	phsSocket->flags = FLAG_RECEIVING;
	phsSocket->pucData = phsSocket->buffer;
	phsSocket->dataLength = 0;
	phsSocket->bufferSize = HTTP_BUFFER_SIZE;
	phsSocket->handler = NULL;
	phsSocket->pxMP = NULL;
	phsSocket->mimeType = NULL;
}

static int _mwGetConnFromIP(HttpParam* hp, IPADDR ip)
{
	int i;
	int connThisIP = 0;
	for (i = 0; i < hp->maxClients; i++) {
		if (!hp->hsSocketQueue[i].socket) continue;
		if (hp->hsSocketQueue[i].ipAddr.laddr == ip.laddr) {
			connThisIP++;
		}
	}
	return connThisIP;
}

void _mwCloseAllConnections(HttpParam* hp)
{
	int i;

	if (hp->listenSocket) {
		closesocket(hp->listenSocket);
		hp->listenSocket = 0;
	}
	for (i = 0; i < hp->maxClients; i++) {
		if (hp->hsSocketQueue[i].socket) {
			closesocket(hp->hsSocketQueue[i].socket);
			hp->hsSocketQueue[i].socket = 0;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// _mwHttpThread
// Webserver independant processing thread. Handles all connections
////////////////////////////////////////////////////////////////////////////
void* mwHttpLoop(void* _hp)
{
	HttpParam *hp = (HttpParam*)_hp;
	HttpSocket *phsSocketCur;
	SOCKET socket;
	struct sockaddr_in sinaddr;
    int iRc;
	int i;

  // main processing loop
	while (!hp->bKillWebserver) {
		time_t tmCurrentTime;
		SOCKET iSelectMaxFds;
		fd_set fdsSelectRead;
		fd_set fdsSelectWrite;

		// clear descriptor sets
		FD_ZERO(&fdsSelectRead);
		FD_ZERO(&fdsSelectWrite);
		FD_SET(hp->listenSocket,&fdsSelectRead);
		iSelectMaxFds=hp->listenSocket;

		// get current time
#ifndef WINCE
		tmCurrentTime=time(NULL);
#else
		tmCurrentTime=GetTickCount() >> 10;
#endif
		// build descriptor sets and close timed out sockets
		for (i = 0; i < hp->maxClients; i++) {
			phsSocketCur = hp->hsSocketQueue + i;

			// get socket fd
			socket = phsSocketCur->socket;
			if (!socket) continue;

			{
				int iError=0;
				int iOptSize=sizeof(int);
				if (getsockopt(socket,SOL_SOCKET,SO_ERROR,(char*)&iError,&iOptSize)) {
					// if a socket contains a error, close it
					SYSLOG(LOG_INFO,"[%d] Socket no longer vaild.\n",socket);
					phsSocketCur->flags=FLAG_CONN_CLOSE;
					_mwCloseSocket(hp, phsSocketCur);
					continue;
				}
			}
			// check expiration timer (for non-listening, in-use sockets)
			if (tmCurrentTime > phsSocketCur->tmExpirationTime) {
				SYSLOG(LOG_INFO,"[%d] Http socket expired\n",phsSocketCur->socket);
				hp->stats.timeOutCount++;
				// close connection
				phsSocketCur->flags=FLAG_CONN_CLOSE;
				_mwCloseSocket(hp, phsSocketCur);
			} else {
				if (phsSocketCur->dwResumeTick) {
					// suspended
					if (phsSocketCur->dwResumeTick > GetTickCount())
						continue;
					else
						phsSocketCur->dwResumeTick = 0;
				}
				if (ISFLAGSET(phsSocketCur,FLAG_RECEIVING)) {
					// add to read descriptor set
					FD_SET(socket,&fdsSelectRead);
				}
				if (ISFLAGSET(phsSocketCur,FLAG_SENDING)) {
					// add to write descriptor set
					FD_SET(socket,&fdsSelectWrite);
				}
				// check if new max socket
				if (socket>iSelectMaxFds) {
					iSelectMaxFds=socket;
				}
			}
		}

		{
			struct timeval tvSelectWait;
			// initialize select delay
			tvSelectWait.tv_sec = 1;
			tvSelectWait.tv_usec = 0; // note: using timeval here -> usec not nsec

			// and check sockets (may take a while!)
			iRc=select(iSelectMaxFds+1, &fdsSelectRead, &fdsSelectWrite,
					NULL, &tvSelectWait);
		}
		if (iRc<0) {
			if (hp->bKillWebserver) break;
			DBG("Select error\n");
			msleep(1000);
			continue;
		}
		if (iRc>0) {
			// check which sockets are read/write able
			for (i = 0; i < hp->maxClients; i++) {
				BOOL bRead;
				BOOL bWrite;

				phsSocketCur = hp->hsSocketQueue + i;

				// get socket fd
				socket = phsSocketCur->socket;
				if (!socket) continue;

				// get read/write status for socket
				bRead = FD_ISSET(socket, &fdsSelectRead);
				bWrite = FD_ISSET(socket, &fdsSelectWrite);

				if ((bRead|bWrite)!=0) {
					//DBG("socket %d bWrite=%d, bRead=%d\n",phsSocketCur->socket,bWrite,bRead);
					// if readable or writeable then process
					if (bWrite && ISFLAGSET(phsSocketCur,FLAG_SENDING)) {
						iRc=_mwProcessWriteSocket(hp, phsSocketCur);
					} else if (bRead && ISFLAGSET(phsSocketCur,FLAG_RECEIVING)) {
						iRc=_mwProcessReadSocket(hp,phsSocketCur);
					} else {
						iRc=-1;
						DBG("Invalid socket state (flag: %08x)\n",phsSocketCur->flags);
					}
					if (!iRc) {
						// and reset expiration timer
#ifndef WINCE
						phsSocketCur->tmExpirationTime=time(NULL)+hp->tmSocketExpireTime;
#else
						phsSocketCur->tmExpirationTime=(GetTickCount()>>10)+hp->tmSocketExpireTime;
#endif
					} else {
						SETFLAG(phsSocketCur,FLAG_CONN_CLOSE);
						_mwCloseSocket(hp, phsSocketCur);
					}
				}
			}

			// check if any socket to accept and accept the socket
			if (FD_ISSET(hp->listenSocket, &fdsSelectRead)) {
				// find empty slot
				phsSocketCur = 0;
				for (i = 0; i < hp->maxClients; i++) {
					if (hp->hsSocketQueue[i].socket == 0) {
						phsSocketCur = hp->hsSocketQueue + i;
						break;
					}
				}

				if (!phsSocketCur) {
					DBG("WARNING: clientCount:%d > maxClients:%d\n", hp->stats.clientCount, hp->maxClients);
					_mwDenySocket(hp,&sinaddr);
					continue;
				}

				phsSocketCur->socket = _mwAcceptSocket(hp,&sinaddr);
				if (phsSocketCur->socket == 0) continue;
				phsSocketCur->ipAddr.laddr=ntohl(sinaddr.sin_addr.s_addr);
				SYSLOG(LOG_INFO,"[%d] IP: %d.%d.%d.%d\n",
					phsSocketCur->socket,
					phsSocketCur->ipAddr.caddr[3],
					phsSocketCur->ipAddr.caddr[2],
					phsSocketCur->ipAddr.caddr[1],
					phsSocketCur->ipAddr.caddr[0]);

				hp->stats.clientCount++;

				//fill structure with data
				_mwInitSocketData(phsSocketCur);
				phsSocketCur->request.pucPayload = 0;
#ifndef WINCE
				phsSocketCur->tmAcceptTime=time(NULL);
#else
				phsSocketCur->tmAcceptTime = GetTickCount()>>10;
#endif
				phsSocketCur->tmExpirationTime=phsSocketCur->tmAcceptTime+hp->tmSocketExpireTime;
				phsSocketCur->iRequestCount=0;
				DBG("Connected clients: %d\n",hp->stats.clientCount);

				//update max client count
				if (hp->stats.clientCount>hp->stats.clientCountMax) hp->stats.clientCountMax=hp->stats.clientCount;
			}
		} else {
			//DBG("Select Timeout\n");
			// select timeout
			// call idle event
			if (hp->pfnIdleCallback) {
				(*hp->pfnIdleCallback)(hp);
			}
		}
	}

	// cleanup
	_mwCloseAllConnections(hp);
	SYSLOG(LOG_INFO, "Cleaning up...\n");
	for (i = 0; i < hp->maxClients; i++) {
		if (hp->hsSocketQueue[i].buffer) free(hp->hsSocketQueue[i].buffer);
	}
	for (i=0; hp->pxUrlHandler[i].pchUrlPrefix; i++) {
		if (hp->pxUrlHandler[i].pfnUrlHandler && hp->pxUrlHandler[i].pfnEventHandler)
			hp->pxUrlHandler[i].pfnEventHandler(MW_UNINIT, &hp->pxUrlHandler[i] , hp);
	}
	free(hp->hsSocketQueue);
	hp->hsSocketQueue = 0;

	// clear state vars
	hp->bKillWebserver=FALSE;
	hp->bWebserverRunning=FALSE;


	return NULL;
} // end of _mwHttpThread

////////////////////////////////////////////////////////////////////////////
// _mwAcceptSocket
// Accept an incoming connection
////////////////////////////////////////////////////////////////////////////
SOCKET _mwAcceptSocket(HttpParam* hp,struct sockaddr_in *sinaddr)
{
    SOCKET socket;
	socklen_t namelen=sizeof(struct sockaddr);

	socket=accept(hp->listenSocket, (struct sockaddr*)sinaddr,&namelen);

    DBG("[%d] connection accepted\n",socket);

	if ((int)socket<=0) {
		DBG("Error accepting socket\n");
		return 0;
    }

#ifndef WIN32
    // set to non-blocking to stop sends from locking up thread
	{
        int iRc;
        int iSockFlags;
        iSockFlags = fcntl(socket, F_GETFL, 0);
        iSockFlags |= O_NONBLOCK;
        iRc = fcntl(socket, F_SETFL, iSockFlags);
	}
#endif

	if (hp->socketRcvBufSize) {
		int iSocketBufSize=hp->socketRcvBufSize<<10;
		setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (const char*)&iSocketBufSize, sizeof(int));
	}

	return socket;
} // end of _mwAcceptSocket

void _mwDenySocket(HttpParam* hp,struct sockaddr_in *sinaddr)
{
    SOCKET socket;
	socklen_t namelen=sizeof(struct sockaddr);
	socket=accept(hp->listenSocket, (struct sockaddr*)sinaddr,&namelen);
    SYSLOG(LOG_INFO,"[%d] connection denied\n",socket);
	_mwSendErrorPage(socket, HTTP403_HEADER, HTTP403_BODY);
	closesocket(socket);
} // end of _mwDenySocket

int _mwBuildHttpHeader(HttpParam* hp, HttpSocket *phsSocket, time_t contentDateTime, char* buffer)
{
	char *p = buffer;
	char *end = buffer + 512;
	const char *status;
	BOOL keepalive = !ISFLAGSET(phsSocket,FLAG_CONN_CLOSE);

	if (phsSocket->response.statusCode >= 200 && phsSocket->response.statusCode < 200 + sizeof(status200) / sizeof(status200[0])) {
		status = status200[phsSocket->response.statusCode - 200];
	} else 	if (phsSocket->response.statusCode >= 300 && phsSocket->response.statusCode < 300 + sizeof(status300) / sizeof(status300[0])) {
		status = status300[phsSocket->response.statusCode - 300];
	} else 	if (phsSocket->response.statusCode >= 400 && phsSocket->response.statusCode < 400 + sizeof(status400) / sizeof(status400[0])) {
		status = status400[phsSocket->response.statusCode - 400];
	} else 	if (phsSocket->response.statusCode >= 500 && phsSocket->response.statusCode < 500 + sizeof(status500) / sizeof(status500[0])) {
		status = status500[phsSocket->response.statusCode - 500];
	} else {
		status = "";
	}


	if (hp->maxClientsPerIP && _mwGetConnFromIP(hp, phsSocket->ipAddr) >= hp->maxClientsPerIP) {
		keepalive = FALSE;
	}

	p+=snprintf(p, end - p, HTTP200_HEADER,
#ifdef ENABLE_RTSP
		(phsSocket->flags & (FLAG_REQUEST_DESCRIBE | FLAG_REQUEST_SETUP)) ? "RTSP/1.0" : "HTTP/1.1",
#else
		"HTTP/1.1",
#endif
		phsSocket->response.statusCode, status,
		HTTP_SERVER_NAME,
		keepalive ? "keep-alive" : "close");

	if (!(hp->flags & FLAG_DISABLE_RANGE)) {
		p += snprintf(p, end - p, "Accept-Ranges: bytes\r\n");
	}

	if (contentDateTime) {
		p += snprintf(p, end - p, "Last-Modified: ");
		p+=mwGetHttpDateTime(contentDateTime, p, 512);
		strcpy(p, "\r\n");
		p+=2;
	}
	if (phsSocket->request.iCSeq) {
		p += snprintf(p, end - p, "CSeq: %d\r\n", phsSocket->request.iCSeq);
	}
	p+=snprintf(p, end - p, "Content-Type: %s\r\n", phsSocket->mimeType ? phsSocket->mimeType : contentTypeTable[phsSocket->response.fileType]);
	if (phsSocket->response.contentLength > 0 && !(phsSocket->flags & FLAG_CHUNK)) {
		p+=snprintf(p, end - p,"Content-Length: %d\r\n", phsSocket->response.contentLength);
	}
	if (phsSocket->flags & FLAG_CHUNK) {
		p += sprintf(p, "Transfer-Encoding: chunked\r\n");
	}
	if (phsSocket->response.statusCode == 301 || phsSocket->response.statusCode == 307) {
		p += sprintf(p, "Location: %s\r\n", phsSocket->pucData);
	}
	strcpy(p, "\r\n");
	return (int)(p- buffer + 2);
}

int mwParseQueryString(UrlHandlerParam* up)
{
	if (up->iVarCount==-1) {
		//parsing variables from query string
		unsigned char *p,*s;
		// get start of query string
		s = strchr(up->pucRequest, '?');
		if (s) {
			*(s++) = 0;
		} else if (ISFLAGSET(up->hs,FLAG_REQUEST_POST)){
			s = up->hs->request.pucPayload;
			if (s && s[0] == '<') s = 0;
		}
		if (s && *s) {
			int i;
			int n = 1;
			//get number of variables
			for (p = s; *p ; p++) {
				if (*p < 32 || *p > 127)
					return 0;
				if (*p == '&') n++;
			}
			up->pxVars = calloc(n + 1, sizeof(HttpVariables));
			up->iVarCount = n;
			//store variable name and value
			for (i = 0, p = s; i < n; p++) {
				switch (*p) {
				case '=':
					if (!(up->pxVars + i)->name) {
						*p = 0;
						(up->pxVars + i)->name = s;
						s=p+1;
					}
					break;
				case 0:
				case '&':
					*p = 0;
					if ((up->pxVars + i)->name) {
						(up->pxVars + i)->value = s;
						mwDecodeString(s);
					} else {
						(up->pxVars + i)->name = s;
						(up->pxVars + i)->value = p;
					}
					s = p + 1;
					i++;
					break;
				}
			}
			(up->pxVars + n)->name = NULL;
		}
	}
	return up->iVarCount;
}

////////////////////////////////////////////////////////////////////////////
// _mwBase64Encode
// buffer size of out_str is (in_len * 4 / 3 + 1)
////////////////////////////////////////////////////////////////////////////
void _mwBase64Encode(const char *in_str, int in_len, char *out_str)
{
	const char base64[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int curr_out_len = 0;
	int i = 0;
	char a, b, c;

	out_str[0] = '\0';

	if (in_len <= 0) return;

	while (i < in_len) {
		a = in_str[i];
		b = (i + 1 >= in_len) ? 0 : in_str[i + 1];
		c = (i + 2 >= in_len) ? 0 : in_str[i + 2];
		if (i + 2 < in_len) {
			 out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);
			 out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);
			 out_str[curr_out_len++] = (base64[((b << 2) & 0x3c) + ((c >> 6) & 0x3)]);
			 out_str[curr_out_len++] = (base64[c & 0x3F]);
		}
		else if (i + 1 < in_len) {
			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);
			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);
			out_str[curr_out_len++] = (base64[((b << 2) & 0x3c) + ((c >> 6) & 0x3)]);
			out_str[curr_out_len++] = '=';
		}
		else {
			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);
			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);
			out_str[curr_out_len++] = '=';
			out_str[curr_out_len++] = '=';
		}
		i += 3;
	}

	out_str[curr_out_len] = '\0';
}

////////////////////////////////////////////////////////////////////////////
// _mwGetBaisAuthorization
// RETURN VALUE: Authorization string, need to free by caller
////////////////////////////////////////////////////////////////////////////
int _mwGetBaisAuthorization(const char* username, const char* password, char* out /*OUT*/)
{
	const char prefix[] = "Basic ";
	int len = (int)(strlen(username) + 1 + strlen(password));
	int out_len = sizeof(prefix) + (len * 4 / 3 + 1) + 2;
	char *tmp, *p;

	if (out_len >= MAX_AUTH_INFO_LEN) return -1;

	tmp = (char*)malloc(len + 1);
	sprintf(tmp, "%s:%s", username, password);
	strcpy(out, prefix);
	p = out + sizeof(prefix) - 1;
	_mwBase64Encode(tmp, len, p);
	p += strlen(p);
	p[0] = '\r'; p[1] = '\n'; p[2] = '\0';

	free(tmp);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// _mwSend401AuthorizationRequired
////////////////////////////////////////////////////////////////////////////
void _mwSend401AuthorizationRequired(HttpParam* hp, HttpSocket* phsSocket, int reason)
{
	char hdr[128];
	const char *body = NULL;
	int body_len = 0, hdrsize = 0;

	if (reason == AUTH_REQUIRED) {
		body = "Authentication required";
		body_len = 8;
	}
	else {
		body = "Authentication failed";
		body_len = 6;
	}

	hdrsize = snprintf(hdr, sizeof(hdr), HTTP401_HEADER, "Login", body_len);

	SYSLOG(LOG_INFO,"[%d] Authorization Required\n",phsSocket->socket);
	// send Authorization Required
	send(phsSocket->socket, hdr, hdrsize, 0);
	send(phsSocket->socket, body, body_len, 0);
	//Below is the work around
	SETFLAG(phsSocket, FLAG_CONN_CLOSE);
	_mwCloseSocket(hp, phsSocket);
}

////////////////////////////////////////////////////////////////////////////
// _mwBasicAuthorizationHandlers
// Basic Authorization implement
// RETURN VALUE:
//  -1 (failed)
//   0 (no need to authorization)
//   1 (successed)
//   2 (Authorization needed)
////////////////////////////////////////////////////////////////////////////
int _mwBasicAuthorizationHandlers(HttpParam* hp, HttpSocket* phsSocket)
{
	AuthHandler* pah;
	char* path = phsSocket->request.pucPath;
	int ret = AUTH_NO_NEED;

	for (pah = hp->pxAuthHandler; pah && pah->pchUrlPrefix; pah++) {
		//printf("\req path:%s prefix:0x%x, username:0x%x, password:0x%x\n", path, pah->pchUrlPrefix, pah->pchUsername, pah->pchPassword);
		if (strncmp(path, pah->pchUrlPrefix, strlen(pah->pchUrlPrefix)) != 0) continue;

		if (pah->pchUsername == NULL || *pah->pchUsername == '\0' ||
			pah->pchPassword == NULL || *pah->pchPassword == '\0') continue;

		if (*pah->pchAuthString == '\0') {
				if (_mwGetBaisAuthorization(pah->pchUsername, pah->pchPassword, pah->pchAuthString) != 0) continue;
		}
		if (phsSocket->request.pucAuthInfo == NULL) {
			ret = AUTH_REQUIRED; //Need to auth
			break;
		}
		else if (strncmp(phsSocket->request.pucAuthInfo, pah->pchAuthString, strlen(pah->pchAuthString)) == 0) { //FIXME:
			phsSocket->request.pucAuthInfo = pah->pchOtherInfo ? pah->pchOtherInfo : "group=admin";
			ret = AUTH_SUCCESSED; //successed
			break;
		}
		else {
			ret = AUTH_FAILED; //Failed, try next
		}
	}

	return ret;
}

int _mwCheckUrlHandlers(HttpParam* hp, HttpSocket* phsSocket)
{
	UrlHandler* puh;
	UrlHandlerParam up;
	int ret=0;
	char* path = phsSocket->request.pucPath;
	char* p = strstr(path, "rtsp://");
	if (p) {
		// remove RTSP protocol and host name from URL
		p = strchr(p + 7, '/');
		if (p) path = p + 1;
	} else {
		while (*path == '/') path++;
	}
	up.pxVars=NULL;
	for (puh=hp->pxUrlHandler; puh && puh->pchUrlPrefix; puh++) {
		size_t prefixLen=strlen(puh->pchUrlPrefix);
		if (puh->pfnUrlHandler && !strncmp(path,puh->pchUrlPrefix,prefixLen)) {
			//URL prefix matches
			memset(&up, 0, sizeof(up));
			up.hp=hp;
			up.p_sys = puh->p_sys;
			up.hs = phsSocket;
			up.dataBytes=(int)phsSocket->bufferSize;
			up.pucRequest=path+prefixLen;
			up.pucHeader=phsSocket->buffer;
			up.pucBuffer=phsSocket->pucData;
			up.pucBuffer[0]=0;
			up.pucPayload = phsSocket->request.pucPayload;
			up.iVarCount=-1;
			phsSocket->handler = puh;
			if (!ISFLAGSET(phsSocket,FLAG_REQUEST_POST)) mwParseQueryString(&up);
			ret=(*puh->pfnUrlHandler)(&up);
			if (!ret) continue;
			phsSocket->flags|=ret;
			phsSocket->response.fileType=up.fileType;
			hp->stats.urlProcessCount++;
			if (ret & FLAG_DATA_RAW) {
				SETFLAG(phsSocket, FLAG_DATA_RAW);
				phsSocket->pucData=up.pucBuffer;
				phsSocket->dataLength=up.dataBytes;
				phsSocket->response.contentLength=up.contentBytes>0?up.contentBytes:up.dataBytes;
				if (ret & FLAG_TO_FREE) {
					phsSocket->ptr=up.pucBuffer;	//keep the pointer which will be used to free memory later
				}
				DBG("URL handler: raw data\n");
			} else if (ret & FLAG_DATA_STREAM) {
				SETFLAG(phsSocket, FLAG_DATA_STREAM);
				phsSocket->pucData = up.pucBuffer;
				phsSocket->dataLength = up.dataBytes;
				phsSocket->response.contentLength = 0;
				DBG("URL handler: stream\n");
			} else if (ret & FLAG_DATA_FILE) {
				SETFLAG(phsSocket, FLAG_DATA_FILE);
				if (up.pucBuffer[0]) {
					free(phsSocket->request.pucPath);
					phsSocket->request.pucPath=strdup(up.pucBuffer);
				}
				DBG("URL handler: file\n");
			} else if (ret & FLAG_DATA_REDIRECT) {
				phsSocket->pucData = up.pucBuffer;
				DBG("URL handler: redirect\n");
			} else if (ret & FLAG_DATA_FD) {
				SETFLAG(phsSocket, FLAG_DATA_FILE);
				DBG("URL handler: file descriptor\n");
			}
			break;
		}
	}
	if (up.pxVars) free(up.pxVars);
	return ret;
}

////////////////////////////////////////////////////////////////////////////
// _mwProcessReadSocket
// Process a socket (read)
////////////////////////////////////////////////////////////////////////////
int _mwProcessReadSocket(HttpParam* hp, HttpSocket* phsSocket)
{
    if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST)) {
		if (phsSocket->pxMP && phsSocket->pxMP->pchBoundaryValue[0]) {
			// multipart and has valid boundary string
			int ret = _mwProcessMultipartPost(hp, phsSocket, FALSE);
			if (!ret) return 0;
			if (ret < 0) {
				SETFLAG(phsSocket, FLAG_CONN_CLOSE);
				return -1;
			}
			goto done;
		}
    }

	// read next chunk of data
	{
		int iLength = recv(phsSocket->socket,
						phsSocket->pucData+phsSocket->dataLength,
						(int)(phsSocket->bufferSize - phsSocket->dataLength - 1), 0);
		if (iLength <= 0) {
			SYSLOG(LOG_INFO,"[%d] socket closed by client\n",phsSocket->socket);
			SETFLAG(phsSocket, FLAG_CONN_CLOSE);

			if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST) && phsSocket->pxMP && hp->pfnFileUpload) {
				HttpMultipart* pxMP = phsSocket->pxMP;
				if (!pxMP->pchBoundaryValue[0]) {
					// no boundary, commit remaining data
					pxMP->oFileuploadStatus = HTTPUPLOAD_LASTCHUNK;
					(hp->pfnFileUpload)(phsSocket->pxMP, phsSocket->pucData, (int)phsSocket->dataLength);
				}
				_mwCloseSocket(hp, phsSocket);
				return 0;
			}
			_mwCloseSocket(hp, phsSocket);
			return -1;
		}
		// add in new data received
		phsSocket->dataLength += iLength;
		phsSocket->pucData[phsSocket->dataLength] = 0;
	}

	// check if end of request
	if (phsSocket->request.headerSize==0) { //FIXME
		int i=0;
		char *path = 0;
		char *headerEnd = strstr(phsSocket->buffer, HTTP_HEADER_END);

		if (!headerEnd)
			return 0;

		// reach the end of the header
		//check request type
		if (!memcmp(phsSocket->buffer, "GET", 3)) {
			SETFLAG(phsSocket,FLAG_REQUEST_GET);
			path = phsSocket->pucData + 5;
		} else if (!memcmp(phsSocket->buffer, "POST", 4)) {
			SETFLAG(phsSocket,FLAG_REQUEST_POST);
			path = phsSocket->pucData + 6;
#ifdef ENABLE_RTSP
		} else if (!memcmp(phsSocket->buffer, "OPTIONS", 7)) {
			SETFLAG(phsSocket,FLAG_REQUEST_OPTIONS);
			path = phsSocket->pucData + 8;
		} else if (!memcmp(phsSocket->buffer, "DESCRIBE", 8)) {
			SETFLAG(phsSocket,FLAG_REQUEST_DESCRIBE);
			path = phsSocket->pucData + 9;
		} else if (!memcmp(phsSocket->buffer, "SETUP", 5)) {
			SETFLAG(phsSocket,FLAG_REQUEST_SETUP);
			path = phsSocket->pucData + 6;
		} else if (!memcmp(phsSocket->buffer, "PLAY", 4)) {
			SETFLAG(phsSocket,FLAG_REQUEST_PLAY);
			path = phsSocket->pucData + 5;
		} else if (!memcmp(phsSocket->buffer, "TEARDOWN", 8)) {
			SETFLAG(phsSocket,FLAG_REQUEST_TEARDOWN);
			path = phsSocket->pucData + 9;
#endif
		} else {
			SYSLOG(LOG_INFO,"[%d] Unsupported method\n",phsSocket->socket);
			phsSocket->request.pucPath = 0;
			return -1;
		}

		phsSocket->request.headerSize = (int)(headerEnd - phsSocket->buffer + 4);
		DBG("[%d] HTTP Header (%d bytes):\n%s", phsSocket->socket, phsSocket->request.headerSize, phsSocket->buffer);
		if (_mwParseHttpHeader(phsSocket)) {
			SYSLOG(LOG_INFO,"Error parsing request\n");
			SETFLAG(phsSocket, FLAG_CONN_CLOSE);
			return -1;
		} else {
			int pathLen;

			if ((hp->flags & FLAG_DISABLE_RANGE) && phsSocket->request.startByte > 0) {
				_mwSendErrorPage(phsSocket->socket, HTTP403_HEADER, HTTP403_BODY);
				return -1;
			}

			DBG("Header parsed\n");

			// count connections from this IP and duplicated connection
			DBG("Checking connections from IP\n");
			if (hp->maxClientsPerIP && _mwGetConnFromIP(hp, phsSocket->ipAddr) > hp->maxClientsPerIP) {
				// too many connection from the same IP
				SYSLOG(LOG_INFO,"[%d] Too many connections from this IP\n", phsSocket->socket);
				_mwSendErrorPage(phsSocket->socket, HTTP403_HEADER, HTTP403_BODY);
				return -1;
			}

			// keep request path
			for (i = 0; i < MAX_REQUEST_PATH_LEN; i++) {
				if ((path[i] == ' ' && (!strncmp(path + i + 1, "HTTP/", 5) || !strncmp(path + i + 1, "RTSP/", 5)))
					|| path[i] == '\r') {
					break;
				}
			}
			pathLen = i;
			if (pathLen >= MAX_REQUEST_PATH_LEN) {
				return -1;
			}

			phsSocket->request.pucPath = malloc(pathLen + 1);
			memcpy(phsSocket->request.pucPath, path, pathLen);
			phsSocket->request.pucPath[pathLen] = 0;
			if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST)) {
				if (phsSocket->pxMP) {
					// is multipart request
					int ret;
					HttpMultipart *pxMP = phsSocket->pxMP;
					pxMP->pp.pchPath = phsSocket->request.pucPath;

					if (ISFLAGSET(phsSocket, FLAG_MULTIPART)) {
						// multipart POST
						if (hp->pfnFileUpload) {
							phsSocket->dataLength -= (phsSocket->request.headerSize);
							memmove(phsSocket->buffer, phsSocket->buffer + phsSocket->request.headerSize, phsSocket->dataLength);
							phsSocket->pucData = phsSocket->buffer;
							phsSocket->request.headerSize = 0;
							pxMP->pp.httpParam = hp;
							pxMP->writeLocation = phsSocket->dataLength;
							ret = _mwProcessMultipartPost(hp, phsSocket, phsSocket->dataLength != 0);
						} else {
							ret = -1;
						}
						if (ret < 0) {
							SETFLAG(phsSocket, FLAG_CONN_CLOSE);
							return -1;
						} else if (ret > 0) {
							goto done;
						}
						return 0;
					} else {
						// direct POST with filename in Content-Type
						phsSocket->dataLength -= phsSocket->request.headerSize;
						pxMP->oFileuploadStatus = HTTPUPLOAD_FIRSTCHUNK;
						if (hp->pfnFileUpload) (hp->pfnFileUpload)(pxMP, phsSocket->buffer + phsSocket->request.headerSize, phsSocket->dataLength);
						pxMP->oFileuploadStatus = HTTPUPLOAD_MORECHUNKS;
						phsSocket->pucData = phsSocket->buffer;
						phsSocket->bufferSize = HTTP_BUFFER_SIZE;
						phsSocket->dataLength = 0;
					}
				} else if (!phsSocket->request.pucPayload) {
					// first receive of payload, prepare for next receive
					if (phsSocket->request.payloadSize > MAX_POST_PAYLOAD_SIZE) phsSocket->request.payloadSize = MAX_POST_PAYLOAD_SIZE;
					phsSocket->bufferSize = phsSocket->request.payloadSize + 1;
					phsSocket->request.pucPayload = malloc(phsSocket->bufferSize);
					phsSocket->pucData = phsSocket->request.pucPayload;
					// payload length already received
					phsSocket->dataLength -= phsSocket->request.headerSize;
					// copy already received payload to payload buffer
					memcpy(phsSocket->request.pucPayload, phsSocket->buffer + phsSocket->request.headerSize, phsSocket->dataLength);
					phsSocket->request.pucPayload[phsSocket->dataLength]=0;
				}
			}
		}
	}
    if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST) && phsSocket->pxMP) {
		if (!phsSocket->pxMP->pchBoundaryValue[0]) {
			// no boundary, simply receive raw data
			if (phsSocket->dataLength == phsSocket->bufferSize && hp->pfnFileUpload) {
				(hp->pfnFileUpload)(phsSocket->pxMP, phsSocket->pucData, phsSocket->dataLength);
				phsSocket->dataLength = 0;
			}
		}
		return 0;
	}

	DBG("[%d] Payload: %d Data Length: %d\n", phsSocket->socket, phsSocket->request.payloadSize, phsSocket->dataLength);
	if (phsSocket->request.payloadSize > 0 && phsSocket->dataLength < (int)phsSocket->request.payloadSize) {
		// there is more data
		return 0;
	}
done:

	// add header zero terminator
	phsSocket->buffer[phsSocket->request.headerSize]=0;
	DBG("%s",phsSocket->buffer);

	if (phsSocket->request.headerSize) {
		phsSocket->pucData = phsSocket->buffer + phsSocket->request.headerSize + 4;
		phsSocket->bufferSize = HTTP_BUFFER_SIZE - phsSocket->request.headerSize - 4;
	} else {
		phsSocket->pucData = phsSocket->buffer;
		phsSocket->bufferSize = HTTP_BUFFER_SIZE;
	}

	SYSLOG(LOG_INFO,"[%d] request path: %s\n",phsSocket->socket,phsSocket->request.pucPath);
	hp->stats.reqCount++;

	if (hp->pxAuthHandler != NULL) {
		int ret = _mwBasicAuthorizationHandlers(hp, phsSocket);
		switch (ret) {
		case AUTH_NO_NEED: //No need to auth
		case AUTH_SUCCESSED: //Successed
			break;
		case AUTH_REQUIRED: //Authorization needed
		case AUTH_FAILED:
		default://Failed
			_mwSend401AuthorizationRequired(hp, phsSocket, ret);
			return 0;
		}
	}

	phsSocket->iRequestCount++;

	if (hp->pxUrlHandler) {
		if (!_mwCheckUrlHandlers(hp,phsSocket))
			SETFLAG(phsSocket,FLAG_DATA_FILE);
	} else {
			SETFLAG(phsSocket,FLAG_DATA_FILE);
	}
	// set state to SENDING (actual sending will occur on next select)
	CLRFLAG(phsSocket,FLAG_RECEIVING)
	SETFLAG(phsSocket,FLAG_SENDING);
	if (phsSocket->request.iHttpVer == 0) {
		CLRFLAG(phsSocket, FLAG_CHUNK);
	}
	if (ISFLAGSET(phsSocket,FLAG_DATA_RAW | FLAG_DATA_STREAM)) {
		return _mwStartSendRawData(hp, phsSocket);
	} else if (ISFLAGSET(phsSocket,FLAG_DATA_FILE)) {
		// send requested page
		return _mwStartSendFile(hp,phsSocket);
	} else if (ISFLAGSET(phsSocket,FLAG_DATA_REDIRECT)) {
		_mwRedirect(phsSocket, phsSocket->pucData);
		return 1;
	}
	SYSLOG(LOG_INFO,"Invalid data flag specified\n");
	return -1;
} // end of _mwProcessReadSocket

////////////////////////////////////////////////////////////////////////////
// _mwProcessWriteSocket
// Process a socket (write)
////////////////////////////////////////////////////////////////////////////
int _mwProcessWriteSocket(HttpParam *hp, HttpSocket* phsSocket)
{
	if (phsSocket->dataLength<=0 && !ISFLAGSET(phsSocket,FLAG_DATA_STREAM)) {
		DBG("[%d] Data sending completed (%d/%d)\n",phsSocket->socket,phsSocket->response.sentBytes,phsSocket->response.contentLength);
		return 1;
	}
	//SYSLOG(LOG_INFO,"[%d] sending data\n",phsSocket->socket);
	if (ISFLAGSET(phsSocket,FLAG_DATA_RAW|FLAG_DATA_STREAM)) {
		return _mwSendRawDataChunk(hp, phsSocket);
	} else if (ISFLAGSET(phsSocket,FLAG_DATA_FILE)) {
		return _mwSendFileChunk(hp, phsSocket);
	} else {
		SYSLOG(LOG_INFO,"Invalid content source\n");
		return -1;
	}
} // end of _mwProcessWriteSocket

////////////////////////////////////////////////////////////////////////////
// _mwCloseSocket
// Close an open connection
////////////////////////////////////////////////////////////////////////////
void _mwCloseSocket(HttpParam* hp, HttpSocket* phsSocket)
{
    if (phsSocket->socket == 0) return;
	if (phsSocket->fd > 0) {
#ifndef WINCE
		close(phsSocket->fd);
#else
		CloseHandle(phsSocket->fd);
#endif
	}
	phsSocket->fd = 0;
	if (phsSocket->pxMP && hp->pfnFileUpload) {
		int i;
		HttpMultipart *pxMP = phsSocket->pxMP;
		(hp->pfnFileUpload)(pxMP , 0, 0);
		// clear multipart structure
		for (i=0; i<pxMP->pp.iNumParams; i++) {
			free(pxMP->pp.stParams[i].pchParamName);
			free(pxMP->pp.stParams[i].pchParamValue);
		}
		free(pxMP);
		phsSocket->pxMP = 0;
	}
	if (phsSocket->request.pucPayload) {
		free(phsSocket->request.pucPayload);
		phsSocket->request.pucPayload = 0;
	}
	if (phsSocket->handler && (ISFLAGSET(phsSocket,FLAG_DATA_STREAM) || ISFLAGSET(phsSocket,FLAG_CLOSE_CALLBACK | FLAG_CONN_CLOSE) == (FLAG_CLOSE_CALLBACK | FLAG_CONN_CLOSE))) {
		UrlHandlerParam up;
		UrlHandler* pfnHandler = (UrlHandler*)phsSocket->handler;
		up.hs = phsSocket;
		up.hp = hp;
		up.pucBuffer = 0;	// indicate connection closed
		up.dataBytes = -1;
		(pfnHandler->pfnUrlHandler)(&up);
		phsSocket->handler = 0;
	}
	if (ISFLAGSET(phsSocket,FLAG_TO_FREE) && phsSocket->ptr) {
		free(phsSocket->ptr);
		phsSocket->ptr=NULL;
		CLRFLAG(phsSocket, FLAG_TO_FREE);
	}
	if (phsSocket->request.pucPath) {
		free(phsSocket->request.pucPath);
		phsSocket->request.pucPath = 0;
	}
	if (!ISFLAGSET(phsSocket,FLAG_CONN_CLOSE)) {
		_mwInitSocketData(phsSocket);
		//reset flag bits
#ifndef WINCE
		phsSocket->tmExpirationTime=time(NULL)+HTTP_KEEPALIVE_TIME;
#else
		phsSocket->tmExpirationTime=(GetTickCount()>>10)+HTTP_KEEPALIVE_TIME;
#endif
		return;
	}
	DBG("[%d] socket closed after responded for %d requests\n",phsSocket->socket,phsSocket->iRequestCount);
	DBG("Connected clients: %d\n",hp->stats.clientCount);
	closesocket(phsSocket->socket);
	phsSocket->socket = 0;
	hp->stats.clientCount--;
	phsSocket->iRequestCount=0;
} // end of _mwCloseSocket

int _mwStrCopy(char *dest, const char *src)
{
	int i;
	for (i=0; src[i]; i++) {
		dest[i]=src[i];
	}
	dest[i]=0;
	return i;
}

int _mwStrHeadMatch(char** pbuf1, const char* buf2) {
	unsigned int i;
	char* buf1 = *pbuf1;
	int x;
	for(i=0;buf2[i];i++) {
		if ((x=tolower(buf1[i])-tolower(buf2[i]))) return 0;
	}
	*pbuf1 = buf1 + i;
	return i;
}

int _mwListDirectory(HttpSocket* phsSocket, char* dir)
{
	char cFileName[128];
	char cFilePath[MAX_PATH];
	char *p=phsSocket->pucData;
	int ret;
	char *pagebuf=phsSocket->pucData;
	size_t bufsize=phsSocket->bufferSize;

	p+=snprintf(p, 256, "<html><head><title>/%s</title></head><body><table border=0 cellpadding=0 cellspacing=0 width=100%%><h2>Directory of /%s</h2><hr>",
		phsSocket->request.pucPath,phsSocket->request.pucPath);
	if (!*dir) strcpy(dir, ".");
	DBG("Listing directory: %s\n",dir);
	for (ret=ReadDir(dir,cFileName); !ret; ret=ReadDir(NULL,cFileName)) {
		struct stat st;
		char *s;
		size_t bytes;
		if (!strcmp(cFileName, ".")) continue;
		DBG("Checking %s ...\n",cFileName);
		bytes=(int)(p-pagebuf);
		if (bytes+384>bufsize) {
			//need to expand buffer
			bufsize+=2048;
			if (!ISFLAGSET(phsSocket,FLAG_TO_FREE)) {
				//first time expanding
				SETFLAG(phsSocket,FLAG_TO_FREE);
				pagebuf=malloc(bufsize);
				memcpy(pagebuf,phsSocket->pucData,bytes);
			} else {
				pagebuf=realloc(pagebuf,bufsize);
			}
			p=pagebuf+bytes;
			DBG("Buffer expanded to %d bytes\n",bufsize);
		}
		snprintf(cFilePath, sizeof(cFilePath), "%s/%s",dir,cFileName);
		if (stat(cFilePath,&st)) continue;
		if (st.st_mode & S_IFDIR) {
			p+=snprintf(p, 256, "<tr><td width=35%%><a href='%s/'>%s</a></td><td width=15%%>&lt;dir&gt;</td><td width=15%%>",
				cFileName,cFileName);
		} else {
			p+=snprintf(p, 256, "<tr><td width=35%%><a href='%s'>%s</a></td><td width=15%%>%u KB</td><td width=15%%>",
				cFileName,cFileName,(unsigned int)(st.st_size >> 10));
			s=strrchr(cFileName,'.');
			if (s) {
				int filetype=mwGetContentType(++s);
				if (filetype!=HTTPFILETYPE_OCTET)
					p+=_mwStrCopy(p,contentTypeTable[filetype]);
				else
					p+=snprintf(p, 256, "%s file",s);
			}
		}
		p+=_mwStrCopy(p,"</td><td>");
		p+=mwGetHttpDateTime(st.st_mtime, p, 512);
		p+=_mwStrCopy(p,"</td></tr>");
	}
	p+=snprintf(p, 256, "</table><hr><i>Directory content generated by %s</i></body></html>", HTTP_SERVER_NAME);
	ReadDir(NULL,NULL);
	phsSocket->response.contentLength=(phsSocket->dataLength=(int)(p-pagebuf));
	phsSocket->response.fileType=HTTPFILETYPE_HTML;
	if (ISFLAGSET(phsSocket,FLAG_TO_FREE)) {
		phsSocket->pucData=pagebuf;
		phsSocket->ptr=pagebuf;
	}
	return 0;
}

void _mwSendErrorPage(SOCKET socket, const char* header, const char* body)
{
	char hdr[128];
	int len = (int)strlen(body);
	int hdrsize = snprintf(hdr, sizeof(hdr), header, HTTP_SERVER_NAME, len);
	send(socket, hdr, hdrsize, 0);
	send(socket, body, len, 0);
}

#ifdef WIN32
#define OPEN_FLAG O_RDONLY|0x8000
#else
#define OPEN_FLAG O_RDONLY
#endif

////////////////////////////////////////////////////////////////////////////
// _mwStartSendFile
// Setup for sending of a file
////////////////////////////////////////////////////////////////////////////
int _mwStartSendFile2(HttpParam* hp, HttpSocket* phsSocket, const char* rootPath, const char* filePath)
{
#ifndef WINCE
	struct stat st;
#endif
	HttpFilePath hfp;

	if (rootPath == NULL || filePath == NULL)
		return -1;

	hfp.pchRootPath=rootPath; //hp->pchWebPath;
	// check type of file requested
	if (!ISFLAGSET(phsSocket, FLAG_DATA_FD)) {
		hfp.pchHttpPath=filePath; //phsSocket->request.pucPath;
		mwGetLocalFileName(&hfp);
#ifndef WINCE
		if (stat(hfp.cFilePath,&st) < 0) {
#ifdef _7Z
			char* szfile = 0;
			char *p = strstr(hfp.cFilePath, ".7z/");
			if (p) {
				p += 3;
				*p = 0;
				szfile = p + 1;
			}
			if (szfile) {
				char* data;
				int len = SzExtractContent(hp->szctx, hfp.cFilePath, szfile, &data);
				if (len > 0) {
					p = strrchr(szfile, '.');
					SETFLAG(phsSocket,FLAG_DATA_RAW);
					phsSocket->pucData = (char*)malloc(len);
					memcpy(phsSocket->pucData, data, len);
					phsSocket->ptr = phsSocket->pucData;
					phsSocket->response.contentLength = len;
					phsSocket->response.fileType = p ? mwGetContentType(p + 1) : HTTPFILETYPE_OCTET;
					phsSocket->dataLength = len;
					return _mwStartSendRawData(hp, phsSocket);
				}
			}
			if (p) *p = SLASH;
#endif

			// file/dir not found
			return -1;
		}
		// open file
		if (!(st.st_mode & S_IFDIR))
			phsSocket->fd=open(hfp.cFilePath,OPEN_FLAG);
		else
			phsSocket->fd = -1;
#else
		{
			TCHAR filePath[MAX_PATH];
			MultiByteToWideChar(CP_ACP, MB_COMPOSITE, hfp.cFilePath, strlen(hfp.cFilePath) + 1, filePath, sizeof(filePath) / sizeof(filePath[0]));
			phsSocket->fd = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		}
#endif
#ifndef WINCE
	} else if ((phsSocket->flags & FLAG_DATA_FD) && phsSocket->fd > 0) {
		strcpy(hfp.cFilePath, phsSocket->request.pucPath);
		hfp.pchExt = strrchr(hfp.cFilePath, '.');
		if (hfp.pchExt) hfp.pchExt++;
		st.st_mtime = 0;
		st.st_size = 0;
		fstat(phsSocket->fd, &st);
#endif
	} else {
		return -1;
	}

#ifndef WINCE
	if (phsSocket->fd < 0) {
#else
	if (phsSocket->fd == INVALID_HANDLE_VALUE) {
#endif
		char *p;
		int i;

#ifndef WINCE
		if (!(st.st_mode & S_IFDIR)) {
			// file/dir not found
			return -1;
		}
#endif

		DBG("Process Directory...\n");
		//requesting for directory, first try opening default pages
		for (p = hfp.cFilePath; *p; p++);
		*(p++)=SLASH;
		for (i=0; i<sizeof(defaultPages)/sizeof(defaultPages[0]); i++) {
			strcpy(p,defaultPages[i]);
#ifndef WINCE
			phsSocket->fd=open(hfp.cFilePath,OPEN_FLAG);
			if (phsSocket->fd > 0) {
				fstat(phsSocket->fd, &st);
				hfp.pchExt = strchr(defaultPages[i], '.') + 1;
				break;
			}
#else
			{
				WCHAR filePath[MAX_PATH];
				MultiByteToWideChar(CP_ACP, MB_COMPOSITE, hfp.cFilePath, strlen(hfp.cFilePath) + 1, filePath, sizeof(filePath) / sizeof(filePath[0]));
				phsSocket->fd = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
			}
			if (phsSocket->fd != INVALID_HANDLE_VALUE) {
				hfp.pchExt = strchr(defaultPages[i], '.') + 1;
				break;
			}
#endif
		}

		if (phsSocket->fd <= 0 && (hp->flags & FLAG_DIR_LISTING)) {
			SETFLAG(phsSocket,FLAG_DATA_RAW);
			if (!hfp.fTailSlash) {
				p=phsSocket->request.pucPath;
				while(*p) p++;				//seek to the end of the string
				strcpy(p, "/");				//add a tailing slash
				while(--p>(char*)phsSocket->request.pucPath) {
					if (*p=='/') {
						p++;
						break;
					}
				}
				_mwRedirect(phsSocket,p);
			} else {
				*(p-1)=0;
				_mwListDirectory(phsSocket,hfp.cFilePath);
			}
			return _mwStartSendRawData(hp, phsSocket);
		}
	}
#ifndef WINCE
	if (phsSocket->fd > 0) {
		DWORD fileSize = st.st_size;

#else
	if (phsSocket->fd != INVALID_HANDLE_VALUE) {
		DWORD fileSize = GetFileSize(phsSocket->fd, 0);
#endif
		phsSocket->response.contentLength = fileSize - phsSocket->request.startByte;
		if (phsSocket->response.contentLength <= 0) {
			phsSocket->request.startByte = 0;
			phsSocket->response.contentLength = fileSize;
		}
		if (phsSocket->request.startByte) {
#ifndef WINCE
			lseek(phsSocket->fd, (long)phsSocket->request.startByte, SEEK_SET);
#else
			SetFilePointer(phsSocket->fd, phsSocket->request.startByte, 0, FILE_BEGIN);
#endif
			phsSocket->response.statusCode = 206;
		}
		if (!phsSocket->response.fileType && hfp.pchExt) {
			phsSocket->response.fileType=mwGetContentType(hfp.pchExt);
		}
		// mark if substitution needed
		if (hp->pfnSubst && (phsSocket->response.fileType==HTTPFILETYPE_HTML ||phsSocket->response.fileType==HTTPFILETYPE_JS)) {
			SETFLAG(phsSocket,FLAG_SUBST);
		}
	} else {
		return -1;
	}

	//SYSLOG(LOG_INFO,"File/requested size: %d/%d\n",st.st_size,phsSocket->response.contentLength);

	// build http header
	phsSocket->dataLength=_mwBuildHttpHeader(
		hp,
		phsSocket,
#ifndef WINCE
		st.st_mtime,
#else
		0,
#endif
		phsSocket->pucData);

	phsSocket->response.headerBytes = phsSocket->dataLength;
	phsSocket->response.sentBytes = 0;
	hp->stats.fileSentCount++;
	return 0;
} // end of _mwStartSendFile2

////////////////////////////////////////////////////////////////////////////
// _mwStartSendFile
// Setup for sending of a file
////////////////////////////////////////////////////////////////////////////
int _mwStartSendFile(HttpParam* hp, HttpSocket* phsSocket)
{
	int ret = -1;

#ifndef DISABLE_VIRTUAL_PATH
	VirtPathHandler* pvph;

	for (pvph=hp->pxVirtPathHandler; pvph && pvph->pchUrlPrefix; pvph++) {
		int prefixLen = (int)strlen(pvph->pchUrlPrefix);
		if (prefixLen == 0) continue;
		if (strncmp(pvph->pchUrlPrefix, phsSocket->request.pucPath, prefixLen) == 0) {
			ret = _mwStartSendFile2(hp, phsSocket, pvph->pchLocalRealPath, phsSocket->request.pucPath + prefixLen);
			if (ret == 0) return 0;
			break;
		}
	}
#endif

	ret = _mwStartSendFile2(hp, phsSocket, hp->pchWebPath, phsSocket->request.pucPath);

	if (ret != 0) {
		SYSLOG(LOG_INFO,"[%d] Http file not found\n",phsSocket->socket);
		_mwSendErrorPage(phsSocket->socket, HTTP404_HEADER, HTTP404_BODY);
	}

	return ret;
} //end of _mwStartSendFile

////////////////////////////////////////////////////////////////////////////
// _mwSendFileChunk
// Send a chunk of a file
////////////////////////////////////////////////////////////////////////////
int _mwSendFileChunk(HttpParam *hp, HttpSocket* phsSocket)
{
	int iBytesWritten;
	int iBytesRead;

	if (hp->maxDownloadSpeed) {
		int speed = (unsigned int)(phsSocket->response.sentBytes / (((time(NULL) - phsSocket->tmAcceptTime) << 10) + 1));
		if (speed && speed > hp->maxDownloadSpeed) {
			DBG("[%d] speed limit applied\n", phsSocket->socket);
			// set resume time, before that, this socket will not be processed
			phsSocket->dwResumeTick = GetTickCount() + 1000;
			return 0;
		}
	}
	if (phsSocket->dataLength > 0) {
		if ((phsSocket->flags & FLAG_CHUNK) && ISFLAGSET(phsSocket, FLAG_HEADER_SENT)) {
			char buf[16];
			iBytesRead = snprintf(buf, sizeof(buf), "%X\r\n", phsSocket->dataLength);
			iBytesWritten = send(phsSocket->socket, buf, iBytesRead, 0);
		}
		// send a chunk of data
		iBytesWritten=send(phsSocket->socket, phsSocket->pucData,(int)phsSocket->dataLength, 0);
		if (iBytesWritten<=0) {
			// close connection
			DBG("[%d] error sending data\n", phsSocket->socket);
			SETFLAG(phsSocket,FLAG_CONN_CLOSE);
#ifndef WINCE
			close(phsSocket->fd);
#else
			CloseHandle(phsSocket->fd);
#endif
			phsSocket->fd = 0;
			return -1;
		}
		SETFLAG(phsSocket, FLAG_HEADER_SENT);
		hp->stats.fileSentBytes+=iBytesWritten;
		phsSocket->response.sentBytes+=iBytesWritten;
		phsSocket->pucData+=iBytesWritten;
		phsSocket->dataLength-=iBytesWritten;
		DBG("[%d] %d bytes sent (%u KB/s)\n", phsSocket->socket, phsSocket->response.sentBytes,
			(unsigned int)(phsSocket->response.sentBytes / (((time(NULL) - phsSocket->tmAcceptTime) << 10) + 1)));
		// if only partial data sent just return wait the remaining data to be sent next time
		if (phsSocket->dataLength>0) return 0;
	}

	// used all buffered data - load next chunk of file
	phsSocket->pucData=phsSocket->buffer;
#ifndef WINCE
	iBytesRead=read(phsSocket->fd,phsSocket->buffer,HTTP_BUFFER_SIZE);
	if (iBytesRead == -1 && errno == 8)
		return 0; // try reading again next time
#else
	iBytesRead = -1;
	ReadFile(phsSocket->fd, phsSocket->buffer,HTTP_BUFFER_SIZE, &iBytesRead, 0);
#endif
	if (iBytesRead<=0) {
		// finished with a file
		int remainBytes = (int)(phsSocket->response.contentLength + phsSocket->response.headerBytes - phsSocket->response.sentBytes);
		DBG("[%d] EOF reached\n",phsSocket->socket);
		if (remainBytes > 0) {
			if (remainBytes>HTTP_BUFFER_SIZE) remainBytes=HTTP_BUFFER_SIZE;
			DBG("Sending %d padding bytes\n",remainBytes);
			memset(phsSocket->buffer,0,remainBytes);
			phsSocket->dataLength=remainBytes;
			return 0;
		} else {
			if (phsSocket->flags & FLAG_CHUNK) {
				send(phsSocket->socket, "0\r\n\r\n", 5, 0);
			}
			DBG("Closing file (fd=%d)\n",phsSocket->fd);
#ifndef WINCE
			if (phsSocket->fd > 0) close(phsSocket->fd);
#else
			if (phsSocket->fd != INVALID_HANDLE_VALUE) CloseHandle(phsSocket->fd);
#endif

			phsSocket->fd = 0;
			return 1;
		}
	}
	if (ISFLAGSET(phsSocket,FLAG_SUBST)) {
		int iBytesUsed;
		// substituted file - read smaller chunk
		phsSocket->dataLength=_mwSubstVariables(hp, phsSocket->buffer,iBytesRead,&iBytesUsed);
		if (iBytesUsed<iBytesRead) {
#ifndef WINCE
			lseek(phsSocket->fd,iBytesUsed-iBytesRead,SEEK_CUR);
#else
			SetFilePointer(phsSocket->fd, iBytesUsed-iBytesRead, 0, FILE_BEGIN);
#endif
		}
	} else {
		phsSocket->dataLength=iBytesRead;
	}
	return 0;
} // end of _mwSendFileChunk

////////////////////////////////////////////////////////////////////////////
// _mwStartSendRawData
// Start sending raw data blocks
////////////////////////////////////////////////////////////////////////////
int _mwStartSendRawData(HttpParam *hp, HttpSocket* phsSocket)
{
	if (ISFLAGSET(phsSocket, FLAG_CUSTOM_HEADER)) {
		return _mwSendRawDataChunk(hp, phsSocket);
	} else {
		char header[HTTP200_HDR_EST_SIZE];
		int offset=0,hdrsize,bytes;
#ifndef WINCE
		hdrsize=_mwBuildHttpHeader(hp, phsSocket,time(NULL),header);
#else
		hdrsize=_mwBuildHttpHeader(hp, phsSocket,0,header);
#endif
		// send http header
		do {
			bytes=send(phsSocket->socket, header+offset,hdrsize-offset,0);
			if (bytes<=0) break;
			offset+=bytes;
		} while (offset<hdrsize);
		if (bytes<=0) {
			// failed to send header (socket may have been closed by peer)
			SYSLOG(LOG_INFO,"Failed to send header\n");
			return -1;
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
// _mwSendRawDataChunk
// Send a chunk of a raw data block
////////////////////////////////////////////////////////////////////////////
int _mwSendRawDataChunk(HttpParam *hp, HttpSocket* phsSocket)
{
	int  iBytesWritten;

	if (phsSocket->flags & FLAG_CHUNK) {
		char buf[16];
		int bytes = snprintf(buf, sizeof(buf), "%x\r\n", phsSocket->dataLength);
		iBytesWritten = send(phsSocket->socket, buf, bytes, 0);
	}
    // send a chunk of data
	if (phsSocket->dataLength > 0) {
		iBytesWritten=(int)send(phsSocket->socket, phsSocket->pucData, phsSocket->dataLength, 0);
		if (iBytesWritten<=0) {
			// failure - close connection
			DBG("Connection closed\n");
			SETFLAG(phsSocket,FLAG_CONN_CLOSE);
			_mwCloseSocket(hp, phsSocket);
			return -1;
		} else {
			DBG("[%d] sent %d bytes of raw data\n",phsSocket->socket,iBytesWritten);
			hp->stats.fileSentBytes+=iBytesWritten;
			phsSocket->response.sentBytes+=iBytesWritten;
			phsSocket->pucData+=iBytesWritten;
			phsSocket->dataLength-=iBytesWritten;
		}
	} else {
		if (ISFLAGSET(phsSocket,FLAG_DATA_STREAM) && phsSocket->handler) {
			//load next chuck of raw data
			UrlHandlerParam up;
			UrlHandler* pfnHandler = (UrlHandler*)phsSocket->handler;
			up.hs = phsSocket;
			up.hp = hp;
			up.pucBuffer=phsSocket->buffer;
			up.dataBytes=HTTP_BUFFER_SIZE;
			if ((pfnHandler->pfnUrlHandler)(&up) == 0) {
				if (phsSocket->flags & FLAG_CHUNK) {
					send(phsSocket->socket, "0\r\n\r\n", 5, 0);
				}
				SETFLAG(phsSocket, FLAG_CONN_CLOSE);
				return 1;	// EOF
			}
			phsSocket->dataLength=up.dataBytes;
			phsSocket->pucData = up.pucBuffer;
		} else {
			if (phsSocket->flags & FLAG_CHUNK) {
				send(phsSocket->socket, "0\r\n\r\n", 5, 0);
			}
			return 1;
		}
	}
	return 0;
} // end of _mwSendRawDataChunk

////////////////////////////////////////////////////////////////////////////
// _mwRedirect
// Setup for redirect to another file
////////////////////////////////////////////////////////////////////////////
void _mwRedirect(HttpSocket* phsSocket, char* pchPath)
{
	/*
	char* path;
	// raw (not file) data send mode
	SETFLAG(phsSocket,FLAG_DATA_RAW);
	// messages is HTML
	phsSocket->response.fileType=HTTPFILETYPE_HTML;

	// build redirect message
	SYSLOG(LOG_INFO,"[%d] Http redirection to %s\n",phsSocket->socket,pchPath);
	path = (pchPath == (char*)phsSocket->pucData) ? strdup(pchPath) : (char*)pchPath;
	phsSocket->dataLength=snprintf(phsSocket->pucData, 512, HTTPBODY_REDIRECT,path);
	phsSocket->response.contentLength=phsSocket->dataLength;
	if (path != pchPath) free(path);
	*/
	char* url = strdup(pchPath);
	int n = snprintf(phsSocket->pucData, phsSocket->dataLength, HTTP301_HEADER, HTTP_SERVER_NAME, url);
	free(url);
	send(phsSocket->socket, phsSocket->pucData, n, 0);
	SETFLAG(phsSocket, FLAG_CONN_CLOSE);
} // end of _mwRedirect

////////////////////////////////////////////////////////////////////////////
// _mwSubstVariables
// Perform substitution of variables in a buffer
// returns new length
////////////////////////////////////////////////////////////////////////////
int _mwSubstVariables(HttpParam* hp, char* pchData, int iLength, int* piBytesUsed)
{
	int lastpos,pos=0,used=0;
	SubstParam sp;
	int iValueBytes;
	if (!hp->pfnSubst) return -1;
    DBG("_SubstVariables input len %d\n",iLength);
	iLength--;
	for(;;) {
		lastpos=pos;
		for (; pos<iLength && *(WORD*)(pchData+pos)!=HTTP_SUBST_PATTERN; pos++);
		used+=(pos-lastpos);
		if (pos==iLength) {
			*piBytesUsed=used+1;
			return iLength+1;
		}
		lastpos=pos;
		for (pos=pos+2; pos<iLength && *(WORD*)(pchData+pos)!=HTTP_SUBST_PATTERN; pos++);
		if (pos==iLength) {
			*piBytesUsed=used;
			return lastpos;
		}
		pos+=2;
		used+=pos-lastpos;
		pchData[pos-2]=0;
		sp.pchParamName=pchData+lastpos+2;
		sp.iMaxValueBytes=pos-lastpos;
		sp.pchParamValue=pchData+lastpos;
		iValueBytes=hp->pfnSubst(&sp);
		if (iValueBytes>=0) {
			if (iValueBytes>sp.iMaxValueBytes) iValueBytes=sp.iMaxValueBytes;
			memmove(pchData+lastpos+iValueBytes,pchData+pos,iLength-pos);
			iLength=iLength+iValueBytes-(pos-lastpos);
			pos=lastpos+iValueBytes;
		} else {
			DBG("No matched variable for %s\n",sp.pchParamName);
			pchData[pos-2]='$';
		}
	}
} // end of _mwSubstVariables

////////////////////////////////////////////////////////////////////////////
// _mwStrStrNoCase
// Case insensitive version of ststr
////////////////////////////////////////////////////////////////////////////
char* _mwStrStrNoCase(char* pchHaystack, char* pchNeedle)
{
  char* pchReturn=NULL;

  while(*pchHaystack!='\0' && pchReturn==NULL) {
    if (toupper(*pchHaystack)==toupper(pchNeedle[0])) {
      char* pchTempHay=pchHaystack;
      char* pchTempNeedle=pchNeedle;
      // start of match
      while(*pchTempHay!='\0') {
        if(toupper(*pchTempHay)!=toupper(*pchTempNeedle)) {
          // failed to match
          break;
        }
        pchTempNeedle++;
        pchTempHay++;
        if (*pchTempNeedle=='\0') {
          // completed match
          pchReturn=pchHaystack;
          break;
        }
      }
    }
    pchHaystack++;
  }

  return pchReturn;
} // end of _mwStrStrNoCase

////////////////////////////////////////////////////////////////////////////
// _mwDecodeCharacter
// Convert and individual character
////////////////////////////////////////////////////////////////////////////
__inline char _mwDecodeCharacter(char* s)
{
  	register unsigned char v;
	if (!*s) return 0;
	if (*s>='a' && *s<='f')
		v = *s-('a'-'A'+7);
	else if (*s>='A' && *s<='F')
		v = *s-7;
	else
		v = *s;
	if (*(++s)==0) return v;
	v <<= 4;
	if (*s>='a' && *s<='f')
		v |= (*s-('a'-'A'+7)) & 0xf;
	else if (*s>='A' && *s<='F')
		v |= (*s-7) & 0xf;
	else
		v |= *s & 0xf;
	return v;
} // end of _mwDecodeCharacter

////////////////////////////////////////////////////////////////////////////
// _mwDecodeString
// This function converts URLd characters back to ascii. For example
// %3A is '.'
////////////////////////////////////////////////////////////////////////////
void mwDecodeString(char* pchString)
{
  int bEnd=FALSE;
  char* pchInput=pchString;
  char* pchOutput=pchString;

  do {
    switch (*pchInput) {
    case '%':
      if (*(pchInput+1)=='\0' || *(pchInput+2)=='\0') {
        // something not right - terminate the string and abort
        *pchOutput='\0';
        bEnd=TRUE;
      } else {
        *pchOutput=_mwDecodeCharacter(pchInput+1);
        pchInput+=3;
      }
      break;
    case '+':
      *pchOutput=' ';
      pchInput++;
      break;
    case '\0':
      bEnd=TRUE;
      // drop through
    default:
      // copy character
      *pchOutput=*pchInput;
      pchInput++;
    }
    pchOutput++;
  } while (!bEnd);
} // end of mwDecodeString

int mwGetContentType(const char *pchExtname)
{
	DWORD dwExt = 0;
	// check type of file requested
	if (pchExtname[1]=='\0') {
		return HTTPFILETYPE_OCTET;
	} else if (pchExtname[2]=='\0') {
		memcpy(&dwExt, pchExtname, 2);
		switch (GETDWORD(pchExtname) & 0xffdfdf) {
		case FILEEXT_JS: return HTTPFILETYPE_JS;
		case FILEEXT_TS: return HTTPFILETYPE_TS;
		}
	} else if (pchExtname[3]=='\0' || pchExtname[3]=='?') {
		//identify 3-char file extensions
		memcpy(&dwExt, pchExtname, sizeof(dwExt));
		switch (dwExt & 0xffdfdfdf) {
		case FILEEXT_HTM:	return HTTPFILETYPE_HTML;
		case FILEEXT_XML:	return HTTPFILETYPE_XML;
		case FILEEXT_XSL:	return HTTPFILETYPE_XML;
		case FILEEXT_TEXT:	return HTTPFILETYPE_TEXT;
		case FILEEXT_XUL:	return HTTPFILETYPE_XUL;
		case FILEEXT_CSS:	return HTTPFILETYPE_CSS;
		case FILEEXT_PNG:	return HTTPFILETYPE_PNG;
		case FILEEXT_JPG:	return HTTPFILETYPE_JPEG;
		case FILEEXT_GIF:	return HTTPFILETYPE_GIF;
		case FILEEXT_SWF:	return HTTPFILETYPE_SWF;
		case FILEEXT_MPA:	return HTTPFILETYPE_MPA;
		case FILEEXT_MPG:	return HTTPFILETYPE_MPEG;
		case FILEEXT_AVI:	return HTTPFILETYPE_AVI;
		case FILEEXT_MP4:	return HTTPFILETYPE_MP4;
		case FILEEXT_MOV:	return HTTPFILETYPE_MOV;
		case FILEEXT_264:	return HTTPFILETYPE_264;
		case FILEEXT_FLV:	return HTTPFILETYPE_FLV;
		case FILEEXT_3GP:	return HTTPFILETYPE_3GP;
		case FILEEXT_ASF:	return HTTPFILETYPE_ASF;
		case FILEEXT_SDP:	return HTTPFILETYPE_SDP;
		}
	} else if (pchExtname[4]=='\0' || pchExtname[4]=='?') {
		memcpy(&dwExt, pchExtname, sizeof(dwExt));
		//logic-and with 0xdfdfdfdf gets the uppercase of 4 chars
		switch (dwExt & 0xdfdfdfdf) {
		case FILEEXT_HTML:	return HTTPFILETYPE_HTML;
		case FILEEXT_MPEG:	return HTTPFILETYPE_MPEG;
		case FILEEXT_M3U8:	return HTTPFILETYPE_M3U8;
		}
	}
	return HTTPFILETYPE_OCTET;
}

int _mwGrabToken(char *pchToken, char chDelimiter, char *pchBuffer, int iMaxTokenSize)
{
	char *p=pchToken;
	int iCharCopied=0;

	while (*p && *p!=chDelimiter && iCharCopied < iMaxTokenSize - 1) {
		*(pchBuffer++)=*(p++);
		iCharCopied++;
	}
	*pchBuffer='\0';
	return (*p==chDelimiter)?iCharCopied:0;
}

int _mwParseHttpHeader(HttpSocket* phsSocket)
{
	int iLen;
	char buf[256];
	char *p=phsSocket->buffer;		//pointer to header data
	HttpRequest *req=&phsSocket->request;

	CLRFLAG(phsSocket, FLAG_MULTIPART);

	p = strstr(phsSocket->buffer, "HTTP/1.");
	do {
		if (p) continue;
		p = strstr(phsSocket->buffer, "RTSP/1.");
		if (!p) return -1;
	} while(0);
	p += 7;
	req->iHttpVer = (*p - '0');
	//analyze rest of the header
	for(;;) {
		//look for a valid field name
		while (*p && *p!='\r') p++;		//travel to '\r'
		if (!*p || !memcmp(p, HTTP_HEADER_END, sizeof(HTTP_HEADER_END)))
			break;
		p+=2;							//skip "\r\n"

		if (_mwStrHeadMatch(&p,"Connection: ")) {
			if (_mwStrHeadMatch(&p,"close")) {
				SETFLAG(phsSocket,FLAG_CONN_CLOSE);
			} else if (_mwStrHeadMatch(&p,"Keep-Alive")) {
				CLRFLAG(phsSocket,FLAG_CONN_CLOSE); //FIXME!!
			}
		} else if (_mwStrHeadMatch(&p, "Content-Length: ")) {
			p+=_mwGrabToken(p,'\r',buf,sizeof(buf));
			phsSocket->request.payloadSize = atoi(buf);
		} else if (_mwStrHeadMatch(&p, "Content-Type: ")) {
			if (_mwStrHeadMatch(&p, "multipart/form-data; boundary=")) {
				// request is a multipart POST
				SETFLAG(phsSocket, FLAG_MULTIPART);
				buf[0] = '-';
				buf[1] = '-';
				p += _mwGrabToken(p ,'\r', buf + 2, sizeof(buf) - 2);
				phsSocket->pxMP = calloc(1, sizeof(HttpMultipart));
				strcpy(phsSocket->pxMP->pchBoundaryValue, buf);
			} else {
				for (; *p != '\r'; p++) {
					if (_mwStrHeadMatch(&p, "; filename=")) {
						p += _mwGrabToken(p ,'\r', buf, sizeof(buf));
						phsSocket->pxMP = calloc(1, sizeof(HttpMultipart));
						strncpy(phsSocket->pxMP->pchFilename, buf, sizeof(phsSocket->pxMP->pchFilename) - 1);
						break;
					}
				}
			}
		} else if (_mwStrHeadMatch(&p, "CSeq: ")) {
			phsSocket->request.iCSeq = atoi(p);
		} else if (_mwStrHeadMatch(&p,"Referer: ")) {
			phsSocket->request.pucReferer= p;
		} else if (_mwStrHeadMatch(&p,"Range: bytes=")) {
			int iEndByte;
			iLen=_mwGrabToken(p,'-',buf,sizeof(buf));
			if (iLen==0) continue;
			p+=iLen;
			phsSocket->request.startByte=atoi(buf);
			iLen=_mwGrabToken(p,'/',buf,sizeof(buf));
			if (iLen==0) continue;
			p+=iLen;
			iEndByte = atoi(buf);
			if (iEndByte > 0)
				phsSocket->response.contentLength = (int)(iEndByte-phsSocket->request.startByte+1);
		} else if (_mwStrHeadMatch(&p,"Host: ")) {
			phsSocket->request.pucHost = p;
		} else if (_mwStrHeadMatch(&p,"Transport: ")) {
			phsSocket->request.pucTransport = p;
		} else if (_mwStrHeadMatch(&p,"Authorization: ")) {
			phsSocket->request.pucAuthInfo = p;
		} else if (_mwStrHeadMatch(&p,"X-Forwarded-For: ")) {
			int i;
			for (i = 3; i >= 0 && *p; i--) {
				phsSocket->ipAddr.caddr[i] = atoi(p);
				while (*p && *p != '\r' && *(p++) != '.');
			}
			DBG("[%d] Forwarded-For: %d.%d.%d.%d\n", phsSocket->socket, phsSocket->ipAddr.caddr[3], phsSocket->ipAddr.caddr[2], phsSocket->ipAddr.caddr[1], phsSocket->ipAddr.caddr[0]);
		}
	}
	return 0;					//end of header
}
//////////////////////////// END OF FILE ///////////////////////////////////
