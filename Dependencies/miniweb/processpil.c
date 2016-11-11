 /*******************************************************************
* Process Platform Independent Layer
* Copyright (c) 2005-06 Stanley Huang <stanleyhuangyc@gmail.com>
* All rights reserved.
*******************************************************************/

#ifdef WIN32
#include <windows.h>
#include <io.h>
//#define snprintf _snprintf
#else
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "processpil.h"

#define SF_BUFFER_SIZE 256

int ShellRead(SHELL_PARAM* param, int timeout)
{
#ifndef WIN32
	int ret;
	fd_set fds;
	struct timeval tv;
#endif

	if (!param->buffer || param->iBufferSize <= 0) {
		if (param->flags & SF_ALLOC) {
			param->buffer=malloc(SF_BUFFER_SIZE);
			param->iBufferSize=SF_BUFFER_SIZE;
		} else {
			return -1;
		}
	}

#ifndef WIN32
	FD_ZERO(&fds);
	FD_SET(param->fdRead,&fds);
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = (timeout - tv.tv_sec * 1000) * 1000;
#endif

	if (!(param->flags & SF_READ_STDOUT_ALL)) {
#ifdef WIN32
		DWORD bytes;
		BOOL success;
		DWORD start = GetTickCount();
		do {
			if (!PeekNamedPipe(param->fdRead,0,0,0,&bytes,0)) return -1;
		} while (bytes == 0 && WaitForSingleObject(param->piProcInfo.hProcess, 50) == WAIT_TIMEOUT && GetTickCount() - start < (DWORD)timeout);
		if (bytes == 0) return 0;

		param->locked++;
		success =ReadFile(param->fdRead, param->buffer, param->iBufferSize - 1, &bytes, NULL);
		param->locked--;
		if (!success)
			return -1;
		param->buffer[bytes]=0;
		return bytes;
#else
		if (select(param->fdRead+1,&fds,NULL,NULL,&tv) < 1) return -1;
		ret=read(param->fdRead,param->buffer, param->iBufferSize - 1);
		if (ret<1) return -1;
		param->buffer[ret] = 0;
		return ret;
#endif
	} else {
#ifdef WIN32
		size_t offset = 0;
		int fSuccess;
		for(;;) {
			DWORD bytes;
			DWORD start = GetTickCount();
			do {
				if (!PeekNamedPipe(param->fdRead,0,0,0,&bytes,0)) return -1;
			} while (bytes == 0 && WaitForSingleObject(param->piProcInfo.hProcess, 50) == WAIT_TIMEOUT && GetTickCount() - start < (DWORD)timeout);
			if (bytes == 0) return 0;

			if (offset + bytes + 1 >= (size_t)param->iBufferSize) {
				if (param->flags & SF_ALLOC) {
					param->iBufferSize = max(param->iBufferSize * 2, (int)(offset + bytes + 1));
					param->buffer = realloc(param->buffer, param->iBufferSize);
				} else {
					break;
				}
			}
			param->locked++;
			fSuccess = ReadFile(param->fdRead, param->buffer + offset, param->iBufferSize - 1 - offset, &bytes, NULL);
			param->locked--;
			if (!fSuccess) break;
			offset += bytes;
		}
		param->buffer[offset]=0;
		return offset;
#endif
	}
}

int ShellWrite(SHELL_PARAM* param, void* data, int bytes)
{
#ifdef WIN32
	DWORD written;
	return WriteFile(param->fdWrite, data, bytes, &written, 0) ? written : -1;
#else
	return write(param->fdWrite, data, bytes);
#endif
}

int ShellTerminate(SHELL_PARAM* param)
{
	int ret;
#ifdef WIN32
	if (!param->piProcInfo.hProcess) return -1;
	ret=TerminateProcess(param->piProcInfo.hProcess,0)?0:-1;
#else
	if (!param->pid) return 0;
	ret=kill(param->pid,SIGKILL);
	param->pid=0;
#endif
	return ret;
}

void ShellClean(SHELL_PARAM* param)
{
#ifdef WIN32
	while (param->locked) Sleep(10);
	if (param->fdRead) CloseHandle((HANDLE)param->fdRead);
	if (param->fdWrite) CloseHandle((HANDLE)param->fdWrite);
	if (param->piProcInfo.hProcess) CloseHandle(param->piProcInfo.hProcess);
	if (param->piProcInfo.hThread) CloseHandle(param->piProcInfo.hThread);
	memset(&param->piProcInfo, 0, sizeof(PROCESS_INFORMATION));
#else
	if (param->fdRead) close(param->fdRead);
	if (param->fdWrite) close(param->fdWrite);
	param->pid=0;
#endif
	param->fdRead=0;
	param->fdWrite=0;
	if (param->flags & SF_ALLOC) {
		free(param->buffer);
		param->buffer=NULL;
		param->iBufferSize=0;
	}
}

int ShellWait(SHELL_PARAM* param, int iTimeout)
{
#ifdef WIN32
	switch (WaitForSingleObject(param->piProcInfo.hProcess,(iTimeout==-1)?INFINITE:iTimeout)) {
	case WAIT_OBJECT_0:
		GetExitCodeProcess(param->piProcInfo.hProcess, &param->iRetCode);
		return 1;
	case WAIT_TIMEOUT:
		return 0;
	default:
		return -1;
	}
#else
	int ret=-1;
	return waitpid(param->pid,&ret,iTimeout==0?WNOHANG:0);
#endif
}

#ifdef WIN32
static char* GetAppName(const char* commandLine)
{
	char* appname;
	char* p;

	p = strrchr(commandLine, '.');
	if (p && !_stricmp(p + 1, "bat")) {
		return _strdup("cmd.exe");
	} else if (*commandLine == '\"') {
		appname = _strdup(commandLine + 1);
		p = strchr(appname, '\"');
		*p = 0;
	} else {
		p = strchr(commandLine, ' ');
		if (p) {
			int l = p - commandLine;
			appname = malloc(l + 5);
			strncpy(appname, commandLine, l);
			appname[l] = 0;
			if (!strchr(appname, '.')) strcat(appname, ".exe");
		} else {
			appname = _strdup(commandLine);
		}
	}
	return appname;
}

#endif

char** Tokenize(char* str, char delimiter)
{
	char** tokens;
	int n = 1;
	int i;
	char *p;

	// find out number of tokens
	p = str;
	for (;;) {
		while (*p && *p != delimiter) p++;
		if (!*p) break;
		n++;
		while (*(++p) == delimiter);
	}
	// allocate buffer for array
	tokens = (char**)calloc(n + 1, sizeof(char*));
	// store pointers to tokens
	p = str;
	for (i = 0; i < n; i++) {
		if (*p == '\"') {
			tokens[i] = ++p;
			while (*p && *p != '\"') p++;
			if (!*p) {
				i++;
				break;
			}
		} else {
			tokens[i] = p;
			while (*p && *p != delimiter) p++;
			if (!*p) {
				i++;
				break;
			}
		}
		*p = 0;
		while (*(++p) == delimiter);
	}
	//tokens[i] = "";
	return tokens;
}

int ShellExec(SHELL_PARAM* param, const char* cmdline)
{
#ifdef WIN32
	SECURITY_ATTRIBUTES saAttr;
	STARTUPINFO siStartInfo;
	BOOL fSuccess;
	char newPath[256],prevPath[256];
	HANDLE hChildStdinRd, hChildStdinWr, hChildStdoutRd, hChildStdoutWr;
#else
	int fdin[2], fdout[2],pid;
	int fdStdinChild;
	int fdStdoutChild;
	char *filePath;
	char *prevPath;
 	char *env[2];

	char *_cmdline = strdup(cmdline);
	char **argv = Tokenize(_cmdline, ' ');
#endif

#if 0
    for ( i = 0; argv[i] != NULL; i++) {
        printf("	argv[%d]=%s\n", i, argv[i]);
    }
#endif

#ifdef WIN32
	if (param->piProcInfo.hProcess || !cmdline) return -1;

	_setmode( _fileno( stdin ), _O_BINARY );
	_setmode( _fileno( stdout ), _O_BINARY );

	// modify path variable
	if (param->pchPath) {
		GetEnvironmentVariable("PATH",prevPath,sizeof(prevPath));
		snprintf(newPath, sizeof(newPath), "%s;s", param->pchPath, prevPath);
		SetEnvironmentVariable("PATH",newPath);
	}

	memset( &param->piProcInfo, 0, sizeof(PROCESS_INFORMATION) );
	memset( &siStartInfo, 0, sizeof(STARTUPINFO) );

///////////////////////////////////////////////////////////////////////
//	create redirecting pipes
///////////////////////////////////////////////////////////////////////

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	if (param->flags & (SF_REDIRECT_STDOUT | SF_REDIRECT_STDERR)) {
		// Create a pipe for the child process's STDOUT.
		if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
			return -1;
		}
		// Create noninheritable read handle and close the inheritable read
		// handle.
		fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
			GetCurrentProcess(), (LPHANDLE)&param->fdRead, 0,
				FALSE,
				DUPLICATE_SAME_ACCESS);
		if( !fSuccess ) return 0;
		CloseHandle(hChildStdoutRd);
		if (param->flags & SF_REDIRECT_STDOUT) {
			siStartInfo.hStdOutput = hChildStdoutWr;
		}
		if (param->flags & SF_REDIRECT_STDERR) {
			siStartInfo.hStdError = hChildStdoutWr;
		}
	}
	siStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	if (param->flags & SF_REDIRECT_STDIN) {
		// Create a pipe for the child process's STDIN.
		if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) return 0;

		// Duplicate the write handle to the pipe so it is not inherited.
		fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr,
			GetCurrentProcess(), (LPHANDLE)&param->fdWrite, 0,
				FALSE,                  // not inherited
				DUPLICATE_SAME_ACCESS);
		if (! fSuccess) return -1;
		CloseHandle(hChildStdinWr);
		siStartInfo.hStdInput = hChildStdinRd;
	}

	siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
	siStartInfo.wShowWindow = (param->flags & SF_SHOW_WINDOW) ? SW_SHOW : SW_HIDE;
	if (param->flags & (SF_REDIRECT_STDIN | SF_REDIRECT_STDOUT | SF_REDIRECT_STDERR)) siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

///////////////////////////////////////////////////////////////////////
//	create child process
///////////////////////////////////////////////////////////////////////
// Create the child process.

	{
		char* appname = GetAppName(cmdline);
		fSuccess = CreateProcess(0,
			(char*)cmdline,	// command line
			NULL,					// process security attributes
			NULL,					// primary thread security attributes
			TRUE,					// handles are inherited
			0,						// creation flags
			NULL,					// process' environment
			param->pchCurDir,		// current directory
			&siStartInfo,			// STARTUPINFO pointer
			&param->piProcInfo);	// receives PROCESS_INFORMATION

		free(appname);
	}

	if (param->pchPath) SetEnvironmentVariable("PATH",prevPath);
	if (!fSuccess) return -1;
	WaitForInputIdle(param->piProcInfo.hProcess,INFINITE);

	if (param->flags & SF_REDIRECT_STDIN)
		CloseHandle(hChildStdinRd);
	if (param->flags & SF_REDIRECT_STDOUT)
		CloseHandle(hChildStdoutWr);
#else

	if (param->pid) return -1;

	if (param->flags & SF_REDIRECT_STDIN) {
		pipe(fdin);
		param->fdWrite = fdin[1];
		fdStdinChild=fdin[0];
	}
	if (param->flags & SF_REDIRECT_STDOUT) {
		pipe(fdout);
		param->fdRead = fdout[0];
		fdStdoutChild=fdout[1];
	}

	pid = fork();
	if (pid == -1) return -1;
	if (pid == 0) { /* chid process */
	    int i;
		//generate argument list
#ifdef _DEBUG
		printf("cmdline:%s\n", cmdline);
#endif
		//TODO: Tokenize() is not good
		filePath=argv[0];
//		p=strrchr(args[0],'/');
//		if (p) {
//			filePath=args[0];
//			args[0]=p+1;
//		} else {
//			filePath=args[0];
//		}

		//set PATH
		env[0]=NULL;
		if (param->pchPath) {
			prevPath=getenv("PATH");
			env[0]=malloc(strlen(prevPath)+strlen(param->pchPath)+2+5);
			sprintf(env[0],"PATH=%s:%s",prevPath,param->pchPath);
			env[1]=NULL;
		}
		if (param->flags & SF_REDIRECT_STDIN) {
			close(fdin[1]);
			dup2(fdStdinChild, 0);
		}
		if (param->flags & SF_REDIRECT_STDOUT) {
			close(fdout[0]);
			dup2(fdStdoutChild, 1);
		}
		for ( i = 0; env[i] != NULL; i++) {
			printf("env[%d]=%s\n", i, env[i]);
		}
		if (execve(filePath, argv, env)<0) {
			printf("Error starting specified program\n");
		}
		return 0;
	}
	if (param->flags & SF_REDIRECT_STDIN) {
		close(fdin[0]);
	}
	if (param->flags & SF_REDIRECT_STDOUT) {
		close(fdout[1]);
	}
	param->pid=pid;
    free(_cmdline);
    free(argv);
#endif
	return 0;
}

int ShellRun(SHELL_PARAM* param, const char* cmdline)
{
	int ret;
	SHELL_PARAM* proc = param;
	if (!param) proc = calloc(1, sizeof(SHELL_PARAM));

#ifdef _DEBUG
	printf("# %s\n", cmdline);
#endif
	ret = ShellExec(proc, cmdline);
	if (ret == 0) {
		if (proc->flags & SF_READ_STDOUT_ALL)
			ShellRead(proc, -1);
		else
			ShellWait(proc, -1);
	}
	ShellClean(proc);

	if (!param) free(proc);
	return ret;
}
