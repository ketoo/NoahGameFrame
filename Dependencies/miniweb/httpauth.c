/////////////////////////////////////////////////////////////////////////////
//
// httpauth.c
//
// MiniWeb HTTP authentication implementation
// Copyright (c) 2005-2012 Stanley Huang <stanleyhuangyc@gmail.com>
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
// _mwCheckAuthentication
// Check if a connected peer is authenticated
////////////////////////////////////////////////////////////////////////////
BOOL _mwCheckAuthentication(HttpParam *hp, HttpSocket* phsSocket)
{
	if (!ISFLAGSET(phsSocket,FLAG_AUTHENTICATION))
		return TRUE;
	if (hp->dwAuthenticatedNode != phsSocket->ipAddr.laddr) {
		// Not authenticated
		hp->stats.authFailCount++;
		return FALSE;
	} 
    // Extend authentication period
    hp->tmAuthExpireTime = time(NULL) + HTTPAUTHTIMEOUT;
  return TRUE;
}
