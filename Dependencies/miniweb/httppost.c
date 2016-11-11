/////////////////////////////////////////////////////////////////////////////
//
// http.c
//
// MiniWeb HTTP POST implementation for 
// Copyright (c) 2005-2011 Stanley Huang <stanleyhuangyc@gmail.com>
//
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "httppil.h"
#include "httpapi.h"
#include "httpint.h"

////////////////////////////////////////////////////////////////////////////
// _mwFindMultipartBoundary
// Searches a memory buffer for a multi-part boundary string
////////////////////////////////////////////////////////////////////////////
char* _mwFindMultipartBoundary(char *poHaystack, int iHaystackSize, char *poNeedle)
{
  int i;
  int iNeedleLength = (int)strlen(poNeedle);
  
//  ASSERT(iNeedleLength > 0);
  for (i=0; i <= (iHaystackSize-iNeedleLength-2); i++){
    if (*(poHaystack+i)==0x0d && *(poHaystack+i+1)==0x0a &&
        memcmp(poHaystack+i+2, poNeedle, iNeedleLength) == 0) {
      return (poHaystack+i);
    }
  }
  
  return NULL;
}

////////////////////////////////////////////////////////////////////////////
// mwFileUploadRegister
// Register a MULTIPART FILE UPLOAD callback
////////////////////////////////////////////////////////////////////////////
PFNFILEUPLOADCALLBACK mwFileUploadRegister(HttpParam *httpParam, PFNFILEUPLOADCALLBACK pfnUploadCb) 
{
  PFNFILEUPLOADCALLBACK pfnUploadPrevCb=httpParam->pfnFileUpload;
  
  // save new CB
  if (pfnUploadCb==NULL) return NULL;
  httpParam->pfnFileUpload=pfnUploadCb;
  
  // return previous CB (so app can chain onto it)
  return pfnUploadPrevCb;
}

////////////////////////////////////////////////////////////////////////////
// mwPostRegister
// Register a POST callback
////////////////////////////////////////////////////////////////////////////
PFNPOSTCALLBACK mwPostRegister(HttpParam *httpParam, PFNPOSTCALLBACK pfnPostCb) 
{
  PFNPOSTCALLBACK pfnPostPrevCb=httpParam->pfnPost;

  // save new CB
  if (pfnPostCb==NULL) return NULL;
  httpParam->pfnPost=pfnPostCb;

  // return previous CB (so app can chain onto it)
  return pfnPostPrevCb;
}

////////////////////////////////////////////////////////////////////////////
// _mwNotifyPostVars
// Process posted variables and do callback with parameter list
////////////////////////////////////////////////////////////////////////////
void _mwNotifyPostVars(HttpParam *hp, HttpSocket* phsSocket, PostParam *pp)
{
  // if found any vars
  if (pp->iNumParams>0 && hp->pfnPost) {
    int iReturn;
    
    // call app callback to process post vars
	iReturn=(hp->pfnPost)(pp);
    
    switch(iReturn) {
    case WEBPOST_AUTHENTICATIONON:
      DBG("Http authentication on\n");
	  SETFLAG(phsSocket,FLAG_AUTHENTICATION)
      break;
    case WEBPOST_AUTHENTICATIONOFF:
      DBG("Http authentication off\n");
	  CLRFLAG(phsSocket,FLAG_AUTHENTICATION)
      break;
    case WEBPOST_AUTHENTICATED:
      {
		/*
        struct sockaddr_in sinAddress;
        socklen_t sLength=sizeof(struct sockaddr_in);
        getpeername(phsSocket->socket,
                    (struct sockaddr*)&sinAddress,&sLength);
        
        hp->dwAuthenticatedNode=ntohl(sinAddress.sin_addr.s_addr);
		*/
		hp->dwAuthenticatedNode = phsSocket->ipAddr.laddr;
		SYSLOG(LOG_INFO,"[%d] Http authenticated\n", phsSocket->socket);
        
        // Set authentication period
        hp->tmAuthExpireTime = time(NULL) + HTTPAUTHTIMEOUT;
      }
      break;
    case WEBPOST_NOTAUTHENTICATED:
	  SYSLOG(LOG_INFO,"[%d] Http authentication failed\n", phsSocket->socket);
      hp->dwAuthenticatedNode=0;
      break;
    }
  }
  

  // was a redirect filename returned
  if (strlen(pp->pchPath) == 0) {
    // redirect to index page
    _mwRedirect(phsSocket, "/");
  }
}

////////////////////////////////////////////////////////////////////////////
// _mwProcessMultipartPost
// Process a multipart POST request
////////////////////////////////////////////////////////////////////////////
int _mwProcessMultipartPost(HttpParam *httpParam, HttpSocket* phsSocket, BOOL fNoRecv)
{
  int sLength = 0;
  char *pchBoundarySearch = NULL;
  HttpMultipart *pxMP = phsSocket->pxMP;
  
  if (pxMP == NULL) {
    return -1;
  }

  // Grab more POST data from the socket
  if (!fNoRecv) {
	  sLength = recv(phsSocket->socket, 
                 phsSocket->buffer + pxMP->writeLocation,
                 (int)(HTTPMAXRECVBUFFER - pxMP->writeLocation), 
                 0);
	  if (sLength < 0) {
		DBG("Socket closed by peer\n");
		return -1;
	  }  else if (sLength > 0) {
		// reset expiration timer
#ifndef WINCE
		phsSocket->tmExpirationTime=time(NULL)+httpParam->tmSocketExpireTime;
#else
		phsSocket->tmExpirationTime=GetTickCount()+httpParam->tmSocketExpireTime;
#endif
		pxMP->writeLocation += sLength;
	  } else {
		return 1;
	  }
  }
  
  
  
  //ASSERT(pxMP->writeLocation <= HTTPMAXRECVBUFFER);
  
  // Search new data for boundary indicator
  pchBoundarySearch = _mwFindMultipartBoundary(phsSocket->buffer, 
                                                 HTTPMAXRECVBUFFER, 
                                                 pxMP->pchBoundaryValue);
  
  for (; pchBoundarySearch != NULL; pchBoundarySearch = _mwFindMultipartBoundary(phsSocket->buffer, 
                                                   HTTPMAXRECVBUFFER, 
												   pxMP->pchBoundaryValue)) {
    if (pxMP->pchFilename[0] && pxMP->oFileuploadStatus != HTTPUPLOAD_LASTCHUNK) {
      // It's the last chunk of the posted file
      // Warning: MAY BE BIGGER THAN HTTPUPLOAD_CHUNKSIZE
      pxMP->oFileuploadStatus = HTTPUPLOAD_LASTCHUNK;
      (httpParam->pfnFileUpload)(pxMP, phsSocket->buffer, (DWORD)pchBoundarySearch - (DWORD)phsSocket->buffer);
      
      DBG("Multipart file POST on socket %d complete\n",  phsSocket->socket);
    }
    
    else {
      char *pchStart = _mwStrStrNoCase(phsSocket->buffer, HTTP_MULTIPARTCONTENT);
      char *pchEnd = strstr(phsSocket->buffer, HTTP_HEADER_END);
      char *pchFilename = _mwStrStrNoCase(phsSocket->buffer, HTTP_FILENAME);

      if (pchStart == NULL || pchEnd == NULL) {
		if (strncmp(phsSocket->buffer + strlen(pxMP->pchBoundaryValue) + 2, "--\r\n",4) == 0) {
			// yes, we're all done
			_mwNotifyPostVars(httpParam, phsSocket, &(pxMP->pp));
			DBG("Multipart POST on socket %d complete!\n", phsSocket->socket);
			return 1;
		} else {
			DBG("Waiting for multipart header description on socket %d\n", phsSocket->socket);
			break;
		}
      }
      
      if (pchFilename == NULL || 
          pchFilename > pchEnd) {    // check filename belongs to this variable
        // It's a normal (non-file) var
        // check we have the entire section (up to start of next boundary)
        pchFilename = NULL;
        if (strstr(pchEnd+4, "\r\n") == NULL) {
          DBG("Waiting for multipart variable value on socket %d\n",
                       phsSocket->socket);
          break;
        }
      }
      
      pchStart+=sizeof(HTTP_MULTIPARTCONTENT); // move past first quote
      pchEnd=strchr(pchStart,0x22);              // find end quote
      
      // Is peer authenticated to post this var?
	  if (_mwCheckAuthentication(httpParam, phsSocket)) {
        
        pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamName = (char*)calloc(pchEnd-pchStart+1, sizeof(char));
        memcpy(pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamName, pchStart, pchEnd-pchStart);
        
        if (pchFilename!=NULL) {
          // use filename as var value
          pchStart=pchFilename+strlen(HTTP_FILENAME)+1;  // move past first quote
          pchEnd=strchr(pchStart,0x22);                  // find end quote
        } else {
          // use data as var value
          pchStart=strstr(pchEnd, HTTP_HEADER_END);
		  if (pchStart) pchStart += 4;
          pchEnd=strstr(pchStart,"\r\n");
        }
        
        pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamValue = calloc(pchEnd-pchStart+1, sizeof(char));  
        memcpy(pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamValue, pchStart, 
               pchEnd-pchStart);
        
        DBG("Http multipart POST var %d [%s]=[%s]\n",
               pxMP->pp.iNumParams,
               pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamName,
               pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamValue);
        
        pxMP->pp.iNumParams++;
        
        if (pchFilename!=NULL) {
          strncpy(pxMP->pchFilename, pxMP->pp.stParams[pxMP->pp.iNumParams-1].pchParamValue, sizeof(pxMP->pchFilename) - 1);
          
          // shift to start of file data
          pxMP->oFileuploadStatus = HTTPUPLOAD_FIRSTCHUNK;
          pchEnd=strstr(pchFilename, HTTP_HEADER_END);
		  if (pchEnd) pchEnd += 4;  //move past "\r\n\r\n"
          pxMP->writeLocation -= (DWORD)pchEnd - (DWORD)phsSocket->buffer;
          memmove(phsSocket->buffer, pchEnd, pxMP->writeLocation);
		  if (pxMP->writeLocation == 0) break;
		  /*
          memset(phsSocket->buffer + pxMP->writeLocation, 0,
                HTTPMAXRECVBUFFER - pxMP->writeLocation);
				*/
          continue;
        } 
        else {
          // move to start of next boundary indicator
          pchBoundarySearch = pchEnd;
        }
      }
    }
    
    // Shift to start of next boundary section
    pxMP->writeLocation -= (DWORD)pchBoundarySearch - (DWORD)phsSocket->buffer;
    memmove(phsSocket->buffer, pchBoundarySearch, pxMP->writeLocation);
    memset(phsSocket->buffer + pxMP->writeLocation, 0, HTTPMAXRECVBUFFER - pxMP->writeLocation);
    
    if (strncmp(phsSocket->buffer + strlen(pxMP->pchBoundaryValue) + 2, "--\r\n",4) == 0) {
		// yes, we're all done
		_mwNotifyPostVars(httpParam, phsSocket, &(pxMP->pp));
		DBG("Multipart POST on socket %d complete!\n", phsSocket->socket);
		return 1;
    }
    
  }
  
  // check if receive buffer is full, if so, flush half of the buffer
  if (pxMP->writeLocation == HTTPMAXRECVBUFFER) {
    if (pxMP->oFileuploadStatus != HTTPUPLOAD_LASTCHUNK) {
      // callback with next chunk of posted file
		if ((httpParam->pfnFileUpload)(pxMP, phsSocket->buffer, HTTPUPLOAD_CHUNKSIZE)) {
			return 1;
		}
      pxMP->oFileuploadStatus = HTTPUPLOAD_MORECHUNKS;
      pxMP->writeLocation -= HTTPUPLOAD_CHUNKSIZE;
      memmove(phsSocket->buffer, phsSocket->buffer + HTTPUPLOAD_CHUNKSIZE, 
              HTTPMAXRECVBUFFER - HTTPUPLOAD_CHUNKSIZE);
      //memset(phsSocket->buffer + HTTPUPLOAD_CHUNKSIZE, 0, HTTPMAXRECVBUFFER - HTTPUPLOAD_CHUNKSIZE);
    } 
    else {
      DBG("Error, posted variable too large?\n");
	  DBG("%s\n", phsSocket->buffer);
      return -1;
    }
  }
  
  return 0;
} // end of _mwProcessMultipartPost

////////////////////////////////////////////////////////////////////////////
// _mwProcessPostVars
// Extract and process POST variables
// NOTE: the function damages the recvd data
////////////////////////////////////////////////////////////////////////////
void _mwProcessPostVars(HttpParam *httpParam, HttpSocket* phsSocket,
                          int iContentOffset,
                          int contentLength)
{
  BOOL bAuthenticated;
  
  bAuthenticated=_mwCheckAuthentication(httpParam, phsSocket);
  //ASSERT(iContentOffset+contentLength<=phsSocket->dataLength);

  // extract the posted vars
  if (httpParam->pfnPost!=NULL) {
    int i;
    char* pchPos;
    char* pchVar=phsSocket->buffer+iContentOffset;
    PostParam pp;
    
    // init number of param block
    memset(&pp, 0, sizeof(PostParam));
	pp.httpParam = httpParam;
    
    // null terminate content data
    *(pchVar+contentLength)='\0';
    
    // process each param
    for (i=0;i<MAXPOSTPARAMS;i++) {
      // find =
      pchPos=strchr(pchVar,'=');
      if (pchPos==NULL) {
        break;
      }
      // terminate var name and add to parm list
      *pchPos='\0'; 
      pp.stParams[pp.iNumParams].pchParamName=pchVar;
      
      // terminate var value and add to parm list
      pp.stParams[pp.iNumParams].pchParamValue=pchPos+1;
      pchPos=strchr(pchPos+1,'&');
      if (pchPos!=NULL) {
        *pchPos='\0'; // null term current value
      }
      
      // if not authenticated then only process vars starting with .
      if (bAuthenticated || 
          (*pp.stParams[pp.iNumParams].pchParamName=='.')) {
        // convert any encoded characters
        mwDecodeString(pp.stParams[pp.iNumParams].pchParamValue);
        
        DBG("Http POST var %d [%s]=[%s]\n",
               pp.iNumParams,
               pp.stParams[pp.iNumParams].pchParamName,
               pp.stParams[pp.iNumParams].pchParamValue);
        
        pp.iNumParams++;
      } else {
        DBG("Http POST var [%s]=[%s] skipped - not authenticated\n",
               pp.stParams[pp.iNumParams].pchParamName,
               pp.stParams[pp.iNumParams].pchParamValue);
      }
      
      // if last var then quit
      if (pchPos==NULL) {
        break;
      }
      
      // move to next var
      pchVar=pchPos+1;
    }

    // process and callback with list of vars
    _mwNotifyPostVars(httpParam, phsSocket, &pp);

  } else {
    // redirect to index page
    _mwRedirect(phsSocket, "/");
  }
} // end of _mwProcessPostVars

////////////////////////////////////////////////////////////////////////////
// _mwProcessPost
// Process a POST request 
////////////////////////////////////////////////////////////////////////////
void _mwProcessPost(HttpParam* httpParam, HttpSocket* phsSocket)
{
  int contentLength=-1;
  int iHeaderLength=0;
  
  //ASSERT(phsSocket->buffer!=NULL);
  
  // null terminate the buffer
  *(phsSocket->buffer+phsSocket->dataLength)=0;
  
  // find content length
  {
    char* pchContentLength;
    
    pchContentLength=strstr(phsSocket->buffer,
                                       HTTP_CONTENTLENGTH);
    if (pchContentLength!=NULL) {
      pchContentLength+=strlen(HTTP_CONTENTLENGTH);
      contentLength=atoi(pchContentLength);
    }
  }
  
  // check if content length found
  if (contentLength>0) {
    
    // check if this is a multipart POST
    if ((HttpMultipart*)phsSocket->ptr == NULL) {
      char *pchMultiPart = _mwStrStrNoCase(phsSocket->buffer, 
                                             HTTP_MULTIPARTHEADER);
      
      if (pchMultiPart != NULL) {
        // We need the full HTTP header before processing (ends in '\r\n\r\n')
        char *pchHttpHeaderEnd = strstr(phsSocket->buffer, HTTP_HEADER_END);
        
        if (pchHttpHeaderEnd != NULL) {
          char *pchBoundarySearch = NULL;
          int iHttpHeaderLength = (DWORD)pchHttpHeaderEnd + 2 - (DWORD)phsSocket->buffer;
          
          DBG("Http multipart POST received on socket %d\n",
                 phsSocket->socket);
          
          // Allocate multipart structure information for socket
          phsSocket->ptr = calloc(1,sizeof(HttpMultipart));
          //ASSERT((HttpMultipart*)phsSocket->ptr != NULL);
          
          // What is the 'boundary' value
          strcpy(((HttpMultipart*)phsSocket->ptr)->pchBoundaryValue,"--");
          pchBoundarySearch = _mwStrStrNoCase(phsSocket->buffer, 
                                                HTTP_MULTIPARTBOUNDARY);
          if (pchBoundarySearch != NULL) {
            sscanf(pchBoundarySearch+9,"%s",
                   ((HttpMultipart*)phsSocket->ptr)->pchBoundaryValue+2);
          } else {
            DBG("Error! Http multipart POST header recvd on socket %d does not contain a boundary value\n",
                   phsSocket->socket);
            SETFLAG(phsSocket, FLAG_CONN_CLOSE);
            return;
          }
          
          //ASSERT(phsSocket->buffer != NULL);
          
          // Shift window to start at first boundary indicator
          ((HttpMultipart*)phsSocket->ptr)->writeLocation = 
            phsSocket->dataLength - iHttpHeaderLength;
          //ASSERT(((HttpMultipart*)phsSocket->ptr)->writeLocation >= 0);
          memmove(phsSocket->buffer, pchHttpHeaderEnd + 2, 
                  ((HttpMultipart*)phsSocket->ptr)->writeLocation);
          memset(phsSocket->buffer + ((HttpMultipart*)phsSocket->ptr)->writeLocation, 0,
                HTTPMAXRECVBUFFER - ((HttpMultipart*)phsSocket->ptr)->writeLocation);
        } 
        else {
          DBG("Http multipart POST on socket %d waiting for additional header info\n",
                       phsSocket->socket);
        }
        
        return;
      }
    }
    
    // it's a normal POST. find body of message
    {
      int iLineLength;
      
      do {
        iLineLength=(int)strcspn(phsSocket->buffer+iHeaderLength,"\r\n");
        iHeaderLength+=(iLineLength+2); // move to next line
      } while (iLineLength>0 && iHeaderLength<=phsSocket->dataLength);
    }
    
    // check if we have the whole message
    if (iHeaderLength+contentLength <= phsSocket->dataLength) {
      // process the variables
      _mwProcessPostVars(httpParam, phsSocket,iHeaderLength,contentLength);
    } else {
      // not enough content received yet
      DBG("Http POST on socket %d waiting for additional data (%d of %d recvd)\n",
                   phsSocket->socket,phsSocket->dataLength-iHeaderLength,
                   contentLength);
    }
  } else {
    #if 0
    // header does not contain content length
    SYSLOG(LOG_ERR,"Error! Http POST header recvd on socket %d does not contain content length\n",
           phsSocket->socket);
    #endif
   
  }
} // end of _mwProcessPost
