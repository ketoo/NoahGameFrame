/////////////////////////////////////////////////////////////////////////////
//
// miniweb.c
//
// MiniWeb start-up code
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "httppil.h"
#include "httpapi.h"
#include "revision.h"
#ifdef MEDIA_SERVER
#include "mediaserver.h"
#endif
#ifdef WIN32
#pragma comment( lib, "WSock32.Lib" )
#endif

int uhMpd(UrlHandlerParam* param);
int ehMpd(MW_EVENT msg, int argi, void* argp);
int uhStats(UrlHandlerParam* param);
int uhVod(UrlHandlerParam* param);
int uhLib(UrlHandlerParam* param);
int uhVodStream(UrlHandlerParam* param);
int uhStream(UrlHandlerParam* param);
int ehVod(MW_EVENT msg, int argi, void* argp);
int uhTest(UrlHandlerParam* param);
int uh7Zip(UrlHandlerParam* param);
int uhFileStream(UrlHandlerParam* param);
int uhAsyncDataTest(UrlHandlerParam* param);
int uhRTSP(UrlHandlerParam* param);
int uhSerial(UrlHandlerParam* param);

UrlHandler urlHandlerList[]={
	{"stats", uhStats, NULL},
#ifdef ENABLE_SERIAL
	{"serial", uhSerial, NULL},
#endif
#ifdef HAVE_THREAD
	{"async", uhAsyncDataTest, NULL},
#endif
#ifdef MEDIA_SERVER
	{"test.sdp", uhRTSP, NULL},
	{"MediaServer/VideoItems/", uhMediaItemsTranscode, ehMediaItemsEvent},
#endif
#ifdef _7Z
	{"7z", uh7Zip, NULL},
#endif
#ifdef _MPD
	{"mpd", uhMpd, ehMpd},
#endif
#ifdef _VOD
	{"vodstream", uhVodStream,NULL},
	{"vodlib", uhLib,0},
	{"vodplay", uhVod,ehVod},
	{"stream", uhStream,NULL},
#endif
	{NULL},
};

#ifndef DISABLE_BASIC_WWWAUTH
AuthHandler authHandlerList[]={
	{"stats", "user", "pass", "group=admin", ""},
	{NULL}
};
#endif

HttpParam httpParam;

extern FILE *fpLog;


//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it needs to substitute variables
//////////////////////////////////////////////////////////////////////////
int DefaultWebSubstCallback(SubstParam* sp)
{
	// the maximum length of variable value should never exceed the number
	// given by sp->iMaxValueBytes
	if (!strcmp(sp->pchParamName,"mykeyword")) {
		return sprintf(sp->pchParamValue, "%d", 1234);
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it recevies posted data
//////////////////////////////////////////////////////////////////////////
int DefaultWebPostCallback(PostParam* pp)
{
  int iReturn=WEBPOST_OK;

  // by default redirect to config page
  //strcpy(pp->chFilename,"index.htm");

  return iReturn;
}

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it receives a multipart
// upload file chunk
//////////////////////////////////////////////////////////////////////////
int DefaultWebFileUploadCallback(HttpMultipart *pxMP, OCTET *poData, size_t dwDataChunkSize)
{
  // Do nothing with the data
	int fd = (int)pxMP->pxCallBackData;
	if (!poData) {
		// to cleanup
		if (fd > 0) {
			close(fd);
			pxMP->pxCallBackData = NULL;
		}
		return 0;
	}
	if (!fd) {
		char filename[256];
		snprintf(filename, sizeof(filename), "%s/%s", httpParam.pchWebPath, pxMP->pchFilename);
		fd = open(filename, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, 0);
		pxMP->pxCallBackData = (void*)fd;
	}
	if (fd <= 0) return -1;
	write(fd, poData, dwDataChunkSize);
	if (pxMP->oFileuploadStatus & HTTPUPLOAD_LASTCHUNK) {
		close(fd);
		pxMP->pxCallBackData = NULL;
	}
	printf("Received %u bytes for multipart upload file %s\n", dwDataChunkSize, pxMP->pchFilename);
	return 0;
}

void Shutdown()
{
	//shutdown server
	mwServerShutdown(&httpParam);
	fclose(fpLog);
	UninitSocket();
}

char* GetLocalAddrString()
{
	// get local ip address
	struct sockaddr_in sock;
	char hostname[128];
	struct hostent * lpHost;
	gethostname(hostname, 128);
	lpHost = gethostbyname(hostname);
	memcpy(&(sock.sin_addr), (void*)lpHost->h_addr_list[0], lpHost->h_length);
	return inet_ntoa(sock.sin_addr);
}

int MiniWebQuit(int arg) {
	static int quitting = 0;
	if (quitting) return 0;
	quitting = 1;
	if (arg) printf("\nCaught signal (%d). MiniWeb shutting down...\n",arg);
	Shutdown();
	return 0;
}

void GetFullPath(char* buffer, char* argv0, char* path)
{
	char* p = strrchr(argv0, '/');
	if (!p) p = strrchr(argv0, '\\');
	if (!p) {
		strcpy(buffer, path);
	} else {
		int l = p - argv0 + 1;
		memcpy(buffer, argv0, l);
		strcpy(buffer + l, path);
	}
}

int main(int argc,char* argv[])
{
	fprintf(stderr,"MiniWeb (built on %s)\n(C)2005-2013 Written by Stanley Huang <stanleyhuangyc@gmail.com>\n\n", __DATE__);

#ifdef WIN32
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) MiniWebQuit, TRUE );
#else
	signal(SIGINT, (void *) MiniWebQuit);
	signal(SIGTERM, (void *) MiniWebQuit);
	signal(SIGPIPE, SIG_IGN);
#endif

	//fill in default settings
	mwInitParam(&httpParam);
	httpParam.maxClients=32;
	httpParam.httpPort = 80;
	GetFullPath(httpParam.pchWebPath, argv[0], "NF_Web_Monitor");
#ifndef DISABLE_BASIC_WWWAUTH
	httpParam.pxAuthHandler = authHandlerList;
#endif
	httpParam.pxUrlHandler=urlHandlerList;
	httpParam.flags=FLAG_DIR_LISTING;
	httpParam.tmSocketExpireTime = 15;
	httpParam.pfnPost = DefaultWebPostCallback;
#ifdef MEDIA_SERVER
	httpParam.pfnFileUpload = TranscodeUploadCallback;
#else
	httpParam.pfnFileUpload = DefaultWebFileUploadCallback;
#endif
#ifndef WIN32
	int isDaemon = 0;
#endif // WIN32
	//parsing command line arguments
	{
		int i;
		for (i=1;i<argc;i++) {
			if (argv[i][0]=='-') {
				switch (argv[i][1]) {
				case 'h':
					fprintf(stderr,"Usage: miniweb	-h	: display this help screen\n"
						       "		-v	: log status/error info\n"
						       "		-p	: specifiy http port [default 80]\n"
						       "		-r	: specify http document directory [default htdocs]\n"
						       "		-l	: specify log file\n"
						       "		-m	: specifiy max clients [default 32]\n"
						       "		-M	: specifiy max clients per IP\n"
							   "		-s	: specifiy download speed limit in KB/s [default: none]\n"
							   "		-n	: disallow multi-part download [default: allow]\n"
						       "		-d	: disallow directory listing [default ON]\n\n");
					fflush(stderr);
                                        exit(1);

				case 'p':
					if ((++i)<argc) httpParam.httpPort=atoi(argv[i]);
					break;
				case 'r':
					if ((++i)<argc) strncpy(httpParam.pchWebPath, argv[i], sizeof(httpParam.pchWebPath) - 1);
					break;
				case 'l':
					if ((++i)<argc) fpLog=freopen(argv[i],"w",stderr);
					break;
				case 'm':
					if ((++i)<argc) httpParam.maxClients=atoi(argv[i]);
					break;
				case 'M':
					if ((++i)<argc) httpParam.maxClientsPerIP=atoi(argv[i]);
					break;
				case 's':
					if ((++i)<argc) httpParam.maxDownloadSpeed=atoi(argv[i]);
					break;
				case 'n':
					httpParam.flags |= FLAG_DISABLE_RANGE;
					break;
				case 'd':
					httpParam.flags &= ~FLAG_DIR_LISTING;
					break;
#ifndef WIN32
				case 't':
					isDaemon = 1;
					break;
#endif // WIN32
				}
			}
		}
	}
	{
		int i;
		int error = 0;
		for (i = 0; urlHandlerList[i].pchUrlPrefix; i++) {
			if (urlHandlerList[i].pfnEventHandler) {
				if (urlHandlerList[i].pfnEventHandler(MW_PARSE_ARGS, urlHandlerList[i].pfnEventHandler, &httpParam))
					error++;
			}
		}
		if (error > 0) {
			printf("Error parsing command line options\n");
			return -1;
		}
	}

	InitSocket();

	{
		int n;
		printf("Host: %s:%d\n", GetLocalAddrString(), httpParam.httpPort);
		printf("Web root: %s\n",httpParam.pchWebPath);
		printf("Max clients (per IP): %d (%d)\n",httpParam.maxClients, httpParam.maxClientsPerIP);
		for (n=0;urlHandlerList[n].pchUrlPrefix;n++);
		printf("URL handlers: %d\n",n);
		if (httpParam.flags & FLAG_DIR_LISTING) printf("Dir listing enabled\n");
		if (httpParam.flags & FLAG_DISABLE_RANGE) printf("Byte-range disabled\n");

		//register page variable substitution callback
		//httpParam[i].pfnSubst=DefaultWebSubstCallback;
#ifndef WIN32
		if (isDaemon == 1)
		{
			daemon(1, 0);
		}
#endif // WIN32
		//start server
		if (mwServerStart(&httpParam)) {
			printf("Error starting HTTP server\n");
		} else {
			mwHttpLoop(&httpParam);
		}
	}

	Shutdown();
	return 0;
}
////////////////////////////// END OF FILE //////////////////////////////
