/*******************************************************************
* Process Platform Independent Layer
* Distributed under GPL license
* Copyright (c) 2005-06 Stanley Huang <stanleyhuangyc@yahoo.com.cn>
* All rights reserved.
*******************************************************************/
#ifndef _PROCESSPIL_H
#define _PROCESSPIL_H

#define SF_ALLOC 0x1
#define SF_SHOW_WINDOW 0x2
#define SF_READ_STDOUT_ALL 0x4
#define SF_CONVERT_LF 0x8
#define SF_REDIRECT_STDIN 0x1000
#define SF_REDIRECT_STDOUT 0x2000
#define SF_REDIRECT_STDERR 0x4000
#define SF_REDIRECT_OUTPUT (0x8000 | SF_REDIRECT_STDOUT)

typedef struct {
	unsigned int flags;
	char *pchCurDir;
	char *pchPath;
#ifdef WIN32
	HANDLE fdRead;
	HANDLE fdWrite;
	int fdStdErr;
	PROCESS_INFORMATION piProcInfo;
	HWND hWnd;
#else
	int fdRead;
	int fdWrite;
	int pid;
#endif
	char *pchEnv;
	char *buffer;
	int iRetCode;
	int iBufferSize;
	int locked;
}SHELL_PARAM;

#ifdef __cplusplus
extern "C" {
#endif
int ShellRead(SHELL_PARAM* param, int timeout);
int ShellWrite(SHELL_PARAM* param, void* data, int bytes);
void ShellClean(SHELL_PARAM* param);
int ShellWait(SHELL_PARAM* param, int iTimeout);
int ShellExec(SHELL_PARAM* param, const char* cmdline);
int ShellTerminate(SHELL_PARAM* param);
int ShellRun(SHELL_PARAM* param, const char* cmdline);

#ifdef __cplusplus
}
#endif

#endif
