/* iowin32.c -- IO base function header for compress/uncompress .zip
   Version 1.1, February 14h, 2010
   part of the MiniZip project

   Copyright (C) 1998-2010 Gilles Vollant
     http://www.winimage.com/zLibDll/minizip.html
   Modifications for Zip64 support
     Copyright (C) 2009-2010 Mathias Svensson
     http://result42.com

   This program is distributed under the terms of the same license as zlib.
   See the accompanying LICENSE file for the full text of the license.
*/

#include <stdlib.h>
#include <tchar.h>

#include "zlib.h"
#include "ioapi.h"
#include "iowin32.h"

#ifndef INVALID_HANDLE_VALUE
#  define INVALID_HANDLE_VALUE (0xFFFFFFFF)
#endif

#ifndef INVALID_SET_FILE_POINTER
#  define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif

#if defined(WINAPI_FAMILY_PARTITION) && (!(defined(IOWIN32_USING_WINRT_API)))
#  if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#    define IOWIN32_USING_WINRT_API 1
#  endif
#endif

voidpf  ZCALLBACK win32_open_file_func  OF((voidpf opaque, const char* filename, int mode));
uLong   ZCALLBACK win32_read_file_func  OF((voidpf opaque, voidpf stream, void* buf, uLong size));
uLong   ZCALLBACK win32_write_file_func OF((voidpf opaque, voidpf stream, const void* buf, uLong size));
ZPOS64_T ZCALLBACK win32_tell64_file_func  OF((voidpf opaque, voidpf stream));
long    ZCALLBACK win32_seek64_file_func  OF((voidpf opaque, voidpf stream, ZPOS64_T offset, int origin));
int     ZCALLBACK win32_close_file_func OF((voidpf opaque, voidpf stream));
int     ZCALLBACK win32_error_file_func OF((voidpf opaque, voidpf stream));

typedef struct
{
    HANDLE hf;
    int error;
    void *filename;
    int filenameLength;
} WIN32FILE_IOWIN;


static void win32_translate_open_mode(int mode,
                                      DWORD* lpdwDesiredAccess,
                                      DWORD* lpdwCreationDisposition,
                                      DWORD* lpdwShareMode,
                                      DWORD* lpdwFlagsAndAttributes)
{
    *lpdwDesiredAccess = *lpdwShareMode = *lpdwFlagsAndAttributes = *lpdwCreationDisposition = 0;

    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) == ZLIB_FILEFUNC_MODE_READ)
    {
        *lpdwDesiredAccess = GENERIC_READ;
        *lpdwCreationDisposition = OPEN_EXISTING;
        *lpdwShareMode = FILE_SHARE_READ;
    }
    else if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
    {
        *lpdwDesiredAccess = GENERIC_WRITE | GENERIC_READ;
        *lpdwCreationDisposition = OPEN_EXISTING;
    }
    else if (mode & ZLIB_FILEFUNC_MODE_CREATE)
    {
        *lpdwDesiredAccess = GENERIC_WRITE | GENERIC_READ;
        *lpdwCreationDisposition = CREATE_ALWAYS;
    }
}

static voidpf win32_build_iowin(HANDLE hFile)
{
    WIN32FILE_IOWIN *iowin = NULL;

    if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
    {
        iowin = (WIN32FILE_IOWIN *)malloc(sizeof(WIN32FILE_IOWIN));
        if (iowin == NULL)
        {
            CloseHandle(hFile);
            return NULL;
        }
        memset(iowin, 0, sizeof(WIN32FILE_IOWIN));
        iowin->hf = hFile;
    }
    return (voidpf)iowin;
}

voidpf ZCALLBACK win32_open64_file_func (voidpf opaque, const void* filename, int mode)
{
    DWORD dwDesiredAccess,dwCreationDisposition,dwShareMode,dwFlagsAndAttributes ;
    HANDLE hFile = NULL;
    WIN32FILE_IOWIN *iowin = NULL;

    win32_translate_open_mode(mode, &dwDesiredAccess, &dwCreationDisposition, &dwShareMode, &dwFlagsAndAttributes);

    if ((filename != NULL) && (dwDesiredAccess != 0))
    {
#ifdef IOWIN32_USING_WINRT_API
#ifdef UNICODE
        hFile = CreateFile2((LPCTSTR)filename, dwDesiredAccess, dwShareMode, dwCreationDisposition, NULL);
#else
        WCHAR filenameW[FILENAME_MAX + 0x200 + 1];
        MultiByteToWideChar(CP_ACP, 0, (const char*)filename, -1, filenameW, FILENAME_MAX + 0x200);
        hFile = CreateFile2(filenameW, dwDesiredAccess, dwShareMode, dwCreationDisposition, NULL);
#endif
#else
        hFile = CreateFile((LPCTSTR)filename, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
#endif
    }

    iowin = win32_build_iowin(hFile);
    if (iowin == NULL)
        return NULL;
    iowin->filenameLength = _tcslen(filename) + 1;
    iowin->filename = (void*)malloc(iowin->filenameLength * sizeof(TCHAR));
    _tcsncpy(iowin->filename, filename, iowin->filenameLength);
    return iowin; 
}


voidpf ZCALLBACK win32_open64_file_funcA (voidpf opaque, const void* filename, int mode)
{
    DWORD dwDesiredAccess, dwCreationDisposition, dwShareMode, dwFlagsAndAttributes ;
    HANDLE hFile = NULL;
    WIN32FILE_IOWIN *iowin = NULL;

    win32_translate_open_mode(mode, &dwDesiredAccess, &dwCreationDisposition, &dwShareMode, &dwFlagsAndAttributes);

    if ((filename != NULL) && (dwDesiredAccess != 0))
    {
#ifdef IOWIN32_USING_WINRT_API
        WCHAR filenameW[FILENAME_MAX + 0x200 + 1];
        MultiByteToWideChar(CP_ACP, 0, (const char*)filename, -1, filenameW, FILENAME_MAX + 0x200);
        hFile = CreateFile2(filenameW, dwDesiredAccess, dwShareMode, dwCreationDisposition, NULL);
#else
        hFile = CreateFileA((LPCSTR)filename, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
#endif
    }

    iowin = win32_build_iowin(hFile);
    if (iowin == NULL)
        return NULL;
    iowin->filenameLength = strlen(filename) + 1;
    iowin->filename = (void*)malloc(iowin->filenameLength * sizeof(char));
    strncpy(iowin->filename, filename, iowin->filenameLength);
    return iowin;
}


voidpf ZCALLBACK win32_open64_file_funcW (voidpf opaque,const void* filename,int mode)
{
    DWORD dwDesiredAccess, dwCreationDisposition, dwShareMode, dwFlagsAndAttributes;
    HANDLE hFile = NULL;
    WIN32FILE_IOWIN *iowin = NULL;

    win32_translate_open_mode(mode, &dwDesiredAccess, &dwCreationDisposition, &dwShareMode, &dwFlagsAndAttributes);

    if ((filename != NULL) && (dwDesiredAccess != 0))
    {
#ifdef IOWIN32_USING_WINRT_API
        hFile = CreateFile2((LPCWSTR)filename, dwDesiredAccess, dwShareMode, dwCreationDisposition, NULL);
#else
        hFile = CreateFileW((LPCWSTR)filename, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
#endif
    }

    iowin = win32_build_iowin(hFile);
    if (iowin == NULL)
        return NULL;
    if (iowin->filename == NULL)
    {
        iowin->filenameLength = wcslen(filename) + 1;
        iowin->filename = (void*)malloc(iowin->filenameLength * sizeof(WCHAR));
        wcsncpy(iowin->filename, filename, iowin->filenameLength);
    }
    return iowin;
}

voidpf ZCALLBACK win32_open_file_func (voidpf opaque,const char* filename,int mode)
{
    DWORD dwDesiredAccess, dwCreationDisposition, dwShareMode, dwFlagsAndAttributes ;
    HANDLE hFile = NULL;
    WIN32FILE_IOWIN *iowin = NULL;

    win32_translate_open_mode(mode, &dwDesiredAccess, &dwCreationDisposition, &dwShareMode, &dwFlagsAndAttributes);

    if ((filename != NULL) && (dwDesiredAccess != 0))
    {
#ifdef IOWIN32_USING_WINRT_API
#ifdef UNICODE
        hFile = CreateFile2((LPCTSTR)filename, dwDesiredAccess, dwShareMode, dwCreationDisposition, NULL);
#else
        WCHAR filenameW[FILENAME_MAX + 0x200 + 1];
        MultiByteToWideChar(CP_ACP, 0, (const char*)filename, -1, filenameW, FILENAME_MAX + 0x200);
        hFile = CreateFile2(filenameW, dwDesiredAccess, dwShareMode, dwCreationDisposition, NULL);
#endif
#else
        hFile = CreateFile((LPCTSTR)filename, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
#endif
    }

    iowin = win32_build_iowin(hFile);
    if (iowin == NULL)
        return NULL;
    iowin->filenameLength = _tcslen((TCHAR*)filename) + 1;
    iowin->filename = (void*)malloc(iowin->filenameLength * sizeof(TCHAR));
    _tcsncpy(iowin->filename, (TCHAR*)filename, iowin->filenameLength);
    return iowin; 
}

voidpf ZCALLBACK win32_opendisk64_file_func (voidpf opaque, voidpf stream, int number_disk, int mode)
{
    WIN32FILE_IOWIN *iowin = NULL;
    TCHAR *diskFilename = NULL;
    voidpf ret = NULL;
    int i = 0;

    if (stream == NULL)
        return NULL;
    iowin = (WIN32FILE_IOWIN*)stream;
    diskFilename = (TCHAR*)malloc(iowin->filenameLength * sizeof(TCHAR));
    _tcsncpy(diskFilename, iowin->filename, iowin->filenameLength);
    for (i = iowin->filenameLength - 1; i >= 0; i -= 1)
    {
        if (diskFilename[i] != _T('.')) 
            continue;
        _sntprintf(&diskFilename[i], iowin->filenameLength - i, _T(".z%02d"), number_disk + 1);
        break;
    }
    if (i >= 0)
        ret = win32_open64_file_func(opaque, (char*)diskFilename, mode);
    free(diskFilename);
    return ret;
}

voidpf ZCALLBACK win32_opendisk64_file_funcW (voidpf opaque, voidpf stream, int number_disk, int mode)
{
    WIN32FILE_IOWIN *iowin = NULL;
    WCHAR *diskFilename = NULL;
    voidpf ret = NULL;
    int i = 0;

    if (stream == NULL)
        return NULL;
    iowin = (WIN32FILE_IOWIN*)stream;
    diskFilename = (WCHAR*)malloc((iowin->filenameLength + 10) * sizeof(WCHAR));
    wcsncpy(diskFilename, iowin->filename, iowin->filenameLength);
    for (i = iowin->filenameLength - 1; i >= 0; i -= 1)
    {
        if (diskFilename[i] != L'.') 
            continue;
        _snwprintf(&diskFilename[i], (iowin->filenameLength + 10) - i, L".z%02d", number_disk + 1);
        break;
    }
    if (i >= 0)
        ret = win32_open64_file_funcW(opaque, diskFilename, mode);
    free(diskFilename);
    return ret;
}

voidpf ZCALLBACK win32_opendisk64_file_funcA (voidpf opaque, voidpf stream, int number_disk, int mode)
{
    WIN32FILE_IOWIN *iowin = NULL;
    char *diskFilename = NULL;
    voidpf ret = NULL;
    int i = 0;

    if (stream == NULL)
        return NULL;
    iowin = (WIN32FILE_IOWIN*)stream;
    diskFilename = (char*)malloc(iowin->filenameLength * sizeof(char));
    strncpy(diskFilename, iowin->filename, iowin->filenameLength);
    for (i = iowin->filenameLength - 1; i >= 0; i -= 1)
    {
        if (diskFilename[i] != '.') 
            continue;
        _snprintf(&diskFilename[i], iowin->filenameLength - i, ".z%02d", number_disk + 1);
        break;
    }
    if (i >= 0)
        ret = win32_open64_file_funcA(opaque, diskFilename, mode);
    free(diskFilename);
    return ret;
}

voidpf ZCALLBACK win32_opendisk_file_func (voidpf opaque, voidpf stream, int number_disk, int mode)
{
    WIN32FILE_IOWIN *iowin = NULL;
    TCHAR *diskFilename = NULL;
    voidpf ret = NULL;
    int i = 0;

    if (stream == NULL)
        return NULL;
    iowin = (WIN32FILE_IOWIN*)stream;
    diskFilename = (TCHAR*)malloc(iowin->filenameLength * sizeof(TCHAR));
    _tcsncpy(diskFilename, iowin->filename, iowin->filenameLength);
    for (i = iowin->filenameLength - 1; i >= 0; i -= 1)
    {
        if (diskFilename[i] != _T('.')) 
            continue;
        _sntprintf(&diskFilename[i], iowin->filenameLength - i, _T(".z%02d"), number_disk + 1);
        break;
    }
    if (i >= 0)
        ret = win32_open_file_func(opaque, (char*)diskFilename, mode);
    free(diskFilename);
    return ret;
}

uLong ZCALLBACK win32_read_file_func (voidpf opaque, voidpf stream, void* buf,uLong size)
{
    uLong ret = 0;
    HANDLE hFile = NULL;
    if (stream != NULL)
        hFile = ((WIN32FILE_IOWIN*)stream)->hf;

    if (hFile != NULL)
    {
        if (!ReadFile(hFile, buf, size, &ret, NULL))
        {
            DWORD dwErr = GetLastError();
            if (dwErr == ERROR_HANDLE_EOF)
                dwErr = 0;
            ((WIN32FILE_IOWIN*)stream)->error = (int)dwErr;
        }
    }

    return ret;
}

uLong ZCALLBACK win32_write_file_func (voidpf opaque,voidpf stream,const void* buf,uLong size)
{
    uLong ret = 0;
    HANDLE hFile = NULL;
    if (stream != NULL)
        hFile = ((WIN32FILE_IOWIN*)stream)->hf;

    if (hFile != NULL)
    {
        if (!WriteFile(hFile, buf, size, &ret, NULL))
        {
            DWORD dwErr = GetLastError();
            if (dwErr == ERROR_HANDLE_EOF)
                dwErr = 0;
            ((WIN32FILE_IOWIN*)stream)->error = (int)dwErr;
        }
    }

    return ret;
}

static BOOL win32_setfilepointer_internal(HANDLE hFile, LARGE_INTEGER pos, LARGE_INTEGER *newPos, DWORD dwMoveMethod)
{
#ifdef IOWIN32_USING_WINRT_API
    return SetFilePointerEx(hFile, pos, newPos, dwMoveMethod);
#else
    LONG lHigh = pos.HighPart;
    BOOL ret = TRUE;
    DWORD dwNewPos = SetFilePointer(hFile, pos.LowPart, &lHigh, dwMoveMethod);
    if ((dwNewPos == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))
        ret = FALSE;
    if ((newPos != NULL) && (ret))
    {
        newPos->LowPart = dwNewPos;
        newPos->HighPart = lHigh;
    }
    return ret;
#endif
}

long ZCALLBACK win32_tell_file_func (voidpf opaque,voidpf stream)
{
    long ret = -1;
    HANDLE hFile = NULL;
    if (stream != NULL)
        hFile = ((WIN32FILE_IOWIN*)stream)->hf;
    if (hFile != NULL)
    {
        LARGE_INTEGER pos;
        pos.QuadPart = 0;
        if (!win32_setfilepointer_internal(hFile, pos, &pos, FILE_CURRENT))
        {
            DWORD dwErr = GetLastError();
            ((WIN32FILE_IOWIN*)stream)->error = (int)dwErr;
            ret = -1;
        }
        else
            ret = (long)pos.LowPart;
    }
    return ret;
}

ZPOS64_T ZCALLBACK win32_tell64_file_func (voidpf opaque, voidpf stream)
{
    ZPOS64_T ret = (ZPOS64_T)-1;
    HANDLE hFile = NULL;
    if (stream != NULL)
        hFile = ((WIN32FILE_IOWIN*)stream)->hf;

    if (hFile)
    {
        LARGE_INTEGER pos;
        pos.QuadPart = 0;
        if (!win32_setfilepointer_internal(hFile, pos, &pos, FILE_CURRENT))
        {
            DWORD dwErr = GetLastError();
            ((WIN32FILE_IOWIN*)stream)->error = (int)dwErr;
            ret = (ZPOS64_T)-1;
        }
        else
            ret = pos.QuadPart;
    }
    return ret;
}


long ZCALLBACK win32_seek_file_func (voidpf opaque,voidpf stream,uLong offset,int origin)
{
    DWORD dwMoveMethod = 0xFFFFFFFF;
    HANDLE hFile = NULL;
    long ret = -1;

    if (stream != NULL)
        hFile = ((WIN32FILE_IOWIN*)stream)->hf;

    switch (origin)
    {
        case ZLIB_FILEFUNC_SEEK_CUR:
            dwMoveMethod = FILE_CURRENT;
            break;
        case ZLIB_FILEFUNC_SEEK_END:
            dwMoveMethod = FILE_END;
            break;
        case ZLIB_FILEFUNC_SEEK_SET:
            dwMoveMethod = FILE_BEGIN;
            break;
        default:
            return -1;
    }

    if (hFile != NULL)
    {
        LARGE_INTEGER pos;
        pos.QuadPart = offset;
        if (!win32_setfilepointer_internal(hFile, pos, NULL, dwMoveMethod))
        {
            DWORD dwErr = GetLastError();
            ((WIN32FILE_IOWIN*)stream)->error = (int)dwErr;
            ret = -1;
        }
        else
            ret = 0;
    }
    return ret;
}

long ZCALLBACK win32_seek64_file_func (voidpf opaque, voidpf stream,ZPOS64_T offset,int origin)
{
    DWORD dwMoveMethod = 0xFFFFFFFF;
    HANDLE hFile = NULL;
    long ret = -1;

    if (stream != NULL)
        hFile = ((WIN32FILE_IOWIN*)stream)->hf;

    switch (origin)
    {
        case ZLIB_FILEFUNC_SEEK_CUR:
            dwMoveMethod = FILE_CURRENT;
            break;
        case ZLIB_FILEFUNC_SEEK_END:
            dwMoveMethod = FILE_END;
            break;
        case ZLIB_FILEFUNC_SEEK_SET:
            dwMoveMethod = FILE_BEGIN;
            break;
        default:
            return -1;
    }

    if (hFile)
    {
        LARGE_INTEGER pos;
        pos.QuadPart = offset;
        if (!win32_setfilepointer_internal(hFile, pos, NULL, dwMoveMethod))
        {
            DWORD dwErr = GetLastError();
            ((WIN32FILE_IOWIN*)stream)->error = (int)dwErr;
            ret = -1;
        }
        else
            ret = 0;
    }
    return ret;
}

int ZCALLBACK win32_close_file_func (voidpf opaque, voidpf stream)
{
    WIN32FILE_IOWIN* iowin = NULL;
    int ret = -1;

    if (stream == NULL)
        return ret;
    iowin = ((WIN32FILE_IOWIN*)stream);
    if (iowin->filename != NULL)
        free(iowin->filename);
    if (iowin->hf != NULL)
    {
        CloseHandle(iowin->hf);
        ret=0;
    }
    free(stream);
    return ret;
}

int ZCALLBACK win32_error_file_func (voidpf opaque, voidpf stream)
{
    int ret = -1;
    if (stream == NULL)
        return ret;
    ret = ((WIN32FILE_IOWIN*)stream)->error;
    return ret;
}

void fill_win32_filefunc (zlib_filefunc_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen_file = win32_open_file_func;
    pzlib_filefunc_def->zopendisk_file = win32_opendisk_file_func;
    pzlib_filefunc_def->zread_file = win32_read_file_func;
    pzlib_filefunc_def->zwrite_file = win32_write_file_func;
    pzlib_filefunc_def->ztell_file = win32_tell_file_func;
    pzlib_filefunc_def->zseek_file = win32_seek_file_func;
    pzlib_filefunc_def->zclose_file = win32_close_file_func;
    pzlib_filefunc_def->zerror_file = win32_error_file_func;
    pzlib_filefunc_def->opaque = NULL;
}

void fill_win32_filefunc64(zlib_filefunc64_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen64_file = win32_open64_file_func;
    pzlib_filefunc_def->zopendisk64_file = win32_opendisk64_file_func;
    pzlib_filefunc_def->zread_file = win32_read_file_func;
    pzlib_filefunc_def->zwrite_file = win32_write_file_func;
    pzlib_filefunc_def->ztell64_file = win32_tell64_file_func;
    pzlib_filefunc_def->zseek64_file = win32_seek64_file_func;
    pzlib_filefunc_def->zclose_file = win32_close_file_func;
    pzlib_filefunc_def->zerror_file = win32_error_file_func;
    pzlib_filefunc_def->opaque = NULL;
}

void fill_win32_filefunc64A(zlib_filefunc64_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen64_file = win32_open64_file_funcA;
    pzlib_filefunc_def->zopendisk64_file = win32_opendisk64_file_funcA;
    pzlib_filefunc_def->zread_file = win32_read_file_func;
    pzlib_filefunc_def->zwrite_file = win32_write_file_func;
    pzlib_filefunc_def->ztell64_file = win32_tell64_file_func;
    pzlib_filefunc_def->zseek64_file = win32_seek64_file_func;
    pzlib_filefunc_def->zclose_file = win32_close_file_func;
    pzlib_filefunc_def->zerror_file = win32_error_file_func;
    pzlib_filefunc_def->opaque = NULL;
}

void fill_win32_filefunc64W(zlib_filefunc64_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen64_file = win32_open64_file_funcW;
    pzlib_filefunc_def->zopendisk64_file = win32_opendisk64_file_funcW;
    pzlib_filefunc_def->zread_file = win32_read_file_func;
    pzlib_filefunc_def->zwrite_file = win32_write_file_func;
    pzlib_filefunc_def->ztell64_file = win32_tell64_file_func;
    pzlib_filefunc_def->zseek64_file = win32_seek64_file_func;
    pzlib_filefunc_def->zclose_file = win32_close_file_func;
    pzlib_filefunc_def->zerror_file = win32_error_file_func;
    pzlib_filefunc_def->opaque = NULL;
}
