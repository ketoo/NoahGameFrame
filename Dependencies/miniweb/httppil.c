/////////////////////////////////////////////////////////////////////////////
//
// httppil.c
//
// MiniWeb Platform Independent Layer
// Copyright (c) 2005-2011 Stanley Huang <stanleyhuangyc@gmail.com>
//
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifndef WINCE
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#endif
#include "httppil.h"

#ifndef WIN32
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif

int InitSocket()
{
#ifdef WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) &&
		WSAStartup(MAKEWORD(1, 1), &wsaData )) {
		return 0;
	}
#endif
	return 1;
}

void UninitSocket()
{
#ifdef WIN32
	WSACleanup();
#endif
}

#ifndef WINCE
char *GetTimeString()
{
	static char buf[16];
	time_t tm=time(NULL);
	memcpy(buf,ctime(&tm)+4,15);
	buf[15]=0;
	return buf;
}
#endif

#ifdef ENABLE_THREAD
int ThreadCreate(pthread_t *pth, void* (*start_routine)(void*), void* arg)
{
#ifndef HAVE_PTHREAD
	DWORD dwid;
	*pth=CreateThread(0,0,(LPTHREAD_START_ROUTINE)start_routine,arg,0,&dwid);
	return *pth!=NULL?0:1;
#else
      pthread_attr_t attr;
      pthread_attr_init(&attr);
	  // Change policy
	  pthread_attr_setschedpolicy(&attr,   SCHED_RR);
	  int ret = pthread_create(pth, &attr, start_routine, arg);
      pthread_attr_destroy(&attr);
      return ret;
#endif
}

int ThreadKill(pthread_t pth)
{
#ifndef HAVE_PTHREAD
	return TerminateThread(pth,0)?0:1;
#else
	return pthread_cancel(pth);
#endif
}

int ThreadWait(pthread_t pth, int timeout, void** ret)
{
#ifndef HAVE_PTHREAD
	if (WaitForSingleObject(pth, timeout)==WAIT_TIMEOUT)
		return 1;
	if (ret) GetExitCodeThread(pth,(LPDWORD)ret);
	CloseHandle(pth);
	return 0;
#else
	return pthread_join(pth,ret);
#endif
}

void MutexCreate(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	*mutex = CreateMutex(0,FALSE,NULL);
#else
	pthread_mutex_init(mutex,NULL);
#endif
}

void MutexDestroy(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	CloseHandle(*mutex);
#else
	pthread_mutex_destroy(mutex);
#endif
}

void MutexLock(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	WaitForSingleObject(*mutex,INFINITE);
#else
	pthread_mutex_lock(mutex);
#endif
}

void MutexUnlock(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	ReleaseMutex(*mutex);
#else
	pthread_mutex_unlock(mutex);
#endif
}

#endif

int IsDir(const char* pchName)
{
	struct stat stDirInfo;
	if (stat( pchName, &stDirInfo) < 0) return 0;
	return (stDirInfo.st_mode & S_IFDIR)?1:0;
}

int ReadDir(const char* pchDir, char* pchFileNameBuf)
{
#if defined(WIN32) || defined(WINCE)
	static HANDLE hFind=NULL;
	WIN32_FIND_DATA finddata;

	if (!pchFileNameBuf) {
		if (hFind) {
			FindClose(hFind);
			hFind=NULL;
		}
		return 0;
	}
	if (pchDir) {
		char *p;
		int len;
		if (!IsDir(pchDir)) return -1;
		if (hFind) FindClose(hFind);
		len = strlen(pchDir);
		p = malloc(len + 5);
		snprintf(p, len + 5, "%s\\*.*", pchDir);
		hFind=FindFirstFile(p,&finddata);
		free(p);
		if (hFind==INVALID_HANDLE_VALUE) {
			hFind=NULL;
			return -1;
		}
		strcpy(pchFileNameBuf,finddata.cFileName);
		return 0;
	}
	if (!hFind) return -1;
	if (!FindNextFile(hFind,&finddata)) {
		FindClose(hFind);
		hFind=NULL;
		return -1;
	}
	strcpy(pchFileNameBuf,finddata.cFileName);
#else
	static DIR *stDirIn=NULL;
	struct dirent *stFiles;

	if (!pchFileNameBuf) {
		if (stDirIn) {
			closedir(stDirIn);
			stDirIn=NULL;
		}
		return 0;
	}
	if (pchDir) {
		if (!IsDir(pchDir)) return -1;
		if (stDirIn) closedir(stDirIn);
		stDirIn = opendir( pchDir);
	}
	if (!stDirIn) return -1;
	stFiles = readdir(stDirIn);
	if (!stFiles) {
		closedir(stDirIn);
		stDirIn=NULL;
		return -1;
	}
	strcpy(pchFileNameBuf, stFiles->d_name);
#endif
	return 0;
}

int IsFileExist(const char* filename)
{
#ifdef WINCE
	WIN32_FIND_DATA f;
	HANDLE hFind = FindFirstFile(filename, &f);
	if (hFind == INVALID_HANDLE_VALUE)
		return 0;
	FindClose(hFind);
	return 1;
#else
	struct stat stat_ret;
	if (stat(filename, &stat_ret) != 0) return 0;

	return (stat_ret.st_mode & S_IFREG) != 0;
#endif
}

#ifndef WIN32
unsigned int GetTickCount()
{
	struct timeval ts;
	gettimeofday(&ts,0);
	return ts.tv_sec * 1000 + ts.tv_usec / 1000;
}
#endif
