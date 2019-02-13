/* miniunz.c
   Version 1.1, February 14h, 2010
   sample part of the MiniZip project

   Copyright (C) 1998-2010 Gilles Vollant
     http://www.winimage.com/zLibDll/minizip.html
   Modifications of Unzip for Zip64
     Copyright (C) 2007-2008 Even Rouault
   Modifications for Zip64 support
     Copyright (C) 2009-2010 Mathias Svensson
     http://result42.com

   This program is distributed under the terms of the same license as zlib.
   See the accompanying LICENSE file for the full text of the license.
*/

#if (!defined(_WIN32)) && (!defined(WIN32)) && (!defined(__APPLE__))
#  ifndef __USE_FILE_OFFSET64
#    define __USE_FILE_OFFSET64
#  endif
#  ifndef __USE_LARGEFILE64
#    define __USE_LARGEFILE64
#  endif
#  ifndef _LARGEFILE64_SOURCE
#    define _LARGEFILE64_SOURCE
#  endif
#  ifndef _FILE_OFFSET_BIT
#    define _FILE_OFFSET_BIT 64
#  endif
#endif

#ifdef __APPLE__
/* In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions */
#  define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#  define FTELLO_FUNC(stream) ftello(stream)
#  define FSEEKO_FUNC(stream, offset, origin) fseeko(stream, offset, origin)
#else
#  define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#  define FTELLO_FUNC(stream) ftello64(stream)
#  define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
#  include <direct.h>
#  include <io.h>
#else
#  include <sys/stat.h>
#  include <unistd.h>
#  include <utime.h>
#endif

#ifdef _WIN32
#  define MKDIR(d) _mkdir(d)
#  define CHDIR(d) _chdir(d)
#else
#  define MKDIR(d) mkdir(d, 0775)
#  define CHDIR(d) chdir(d)
#endif

#include "unzip.h"

#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME     (256)

#ifdef _WIN32
#  define USEWIN32IOAPI
#  include "iowin32.h"
#endif

void change_file_date(const char *filename, uLong dosdate, tm_unz tmu_date)
{
#ifdef _WIN32
    HANDLE hFile;
    FILETIME ftm, ftLocal, ftCreate, ftLastAcc, ftLastWrite;

    hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        GetFileTime(hFile, &ftCreate, &ftLastAcc, &ftLastWrite);
        DosDateTimeToFileTime((WORD)(dosdate>>16),(WORD)dosdate, &ftLocal);
        LocalFileTimeToFileTime(&ftLocal, &ftm);
        SetFileTime(hFile, &ftm, &ftLastAcc, &ftm);
        CloseHandle(hFile);
    }
#else
#if defined unix || defined __APPLE__
    struct utimbuf ut;
    struct tm newdate;

    newdate.tm_sec = tmu_date.tm_sec;
    newdate.tm_min = tmu_date.tm_min;
    newdate.tm_hour = tmu_date.tm_hour;
    newdate.tm_mday = tmu_date.tm_mday;
    newdate.tm_mon = tmu_date.tm_mon;
    if (tmu_date.tm_year > 1900)
        newdate.tm_year = tmu_date.tm_year - 1900;
    else
        newdate.tm_year = tmu_date.tm_year ;
    newdate.tm_isdst = -1;

    ut.actime = ut.modtime = mktime(&newdate);
    utime(filename,&ut);
#endif
#endif
}

int check_file_exists(const char* filename)
{
    FILE* ftestexist = FOPEN_FUNC(filename,"rb");
    if (ftestexist == NULL)
        return 0;
    fclose(ftestexist);
    return 1;
}

int makedir(const char *newdir)
{
    char *buffer = NULL;
    char *p = NULL;
    int len = (int)strlen(newdir);

    if (len <= 0)
        return 0;

    buffer = (char*)malloc(len+1);
    if (buffer == NULL)
    {
        printf("Error allocating memory\n");
        return UNZ_INTERNALERROR;
    }

    strcpy(buffer, newdir);

    if (buffer[len-1] == '/')
        buffer[len-1] = 0;

    if (MKDIR(buffer) == 0)
    {
        free(buffer);
        return 1;
    }

    p = buffer + 1;
    while (1)
    {
        char hold;
        while(*p && *p != '\\' && *p != '/')
            p++;
        hold = *p;
        *p = 0;

        if ((MKDIR(buffer) == -1) && (errno == ENOENT))
        {
            printf("couldn't create directory %s (%d)\n", buffer, errno);
            free(buffer);
            return 0;
        }

        if (hold == 0)
            break;

        *p++ = hold;
    }

    free(buffer);
    return 1;
}

void display_zpos64(ZPOS64_T n, int size_char)
{
    /* To avoid compatibility problem we do here the conversion */
    char number[21] = {0};
    int offset = 19;
    int pos_string = 19;
    int size_display_string = 19;

    while (1)
    {
        number[offset] = (char)((n%10) + '0');
        if (number[offset] != '0')
            pos_string = offset;
        n /= 10;
        if (offset == 0)
            break;
        offset--;
    }

    size_display_string -= pos_string;
    while (size_char-- > size_display_string)
        printf(" ");
    printf("%s",&number[pos_string]);
}

void do_banner()
{
    printf("MiniUnz 1.01b, demo of zLib + Unz package written by Gilles Vollant\n");
    printf("more info at http://www.winimage.com/zLibDll/minizip.html\n\n");
}

void do_help()
{
    printf("Usage : miniunz [-e] [-x] [-v] [-l] [-o] [-p password] file.zip [file_to_extr.] [-d extractdir]\n\n" \
           "  -e  Extract without pathname (junk paths)\n" \
           "  -x  Extract with pathname\n" \
           "  -v  list files\n" \
           "  -l  list files\n" \
           "  -d  directory to extract into\n" \
           "  -o  overwrite files without prompting\n" \
           "  -p  extract crypted file using password\n\n");
}

int do_list(unzFile uf)
{
    int err = unzGoToFirstFile(uf);
    if (err != UNZ_OK)
    {
        printf("error %d with zipfile in unzGoToFirstFile\n", err);
        return 1;
    }

    printf("  Length  Method     Size Ratio   Date    Time   CRC-32     Name\n");
    printf("  ------  ------     ---- -----   ----    ----   ------     ----\n");

    do
    {
        char filename_inzip[256] = {0};
        unz_file_info64 file_info = {0};
        uLong ratio = 0;
        const char *string_method = NULL;
        char charCrypt = ' ';

        err = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
        if (err != UNZ_OK)
        {
            printf("error %d with zipfile in unzGetCurrentFileInfo\n", err);
            break;
        }

        if (file_info.uncompressed_size > 0)
            ratio = (uLong)((file_info.compressed_size*100) / file_info.uncompressed_size);

        /* Display a '*' if the file is encrypted */
        if ((file_info.flag & 1) != 0)
            charCrypt = '*';

        if (file_info.compression_method == 0)
            string_method = "Stored";
        else if (file_info.compression_method == Z_DEFLATED)
        {
            uInt iLevel = (uInt)((file_info.flag & 0x6) / 2);
            if (iLevel == 0)
              string_method = "Defl:N";
            else if (iLevel == 1)
              string_method = "Defl:X";
            else if ((iLevel == 2) || (iLevel == 3))
              string_method = "Defl:F"; /* 2:fast , 3 : extra fast*/
        }
        else if (file_info.compression_method == Z_BZIP2ED)
        {
            string_method = "BZip2 ";
        }
        else
            string_method = "Unkn. ";

        display_zpos64(file_info.uncompressed_size, 7);
        printf("  %6s%c", string_method, charCrypt);
        display_zpos64(file_info.compressed_size, 7);
        printf(" %3lu%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %8.8lx   %s\n", ratio,
                (uLong)file_info.tmu_date.tm_mon + 1, (uLong)file_info.tmu_date.tm_mday,
                (uLong)file_info.tmu_date.tm_year % 100,
                (uLong)file_info.tmu_date.tm_hour, (uLong)file_info.tmu_date.tm_min,
                (uLong)file_info.crc, filename_inzip);

        err = unzGoToNextFile(uf);
    }
    while (err == UNZ_OK);

    if (err != UNZ_END_OF_LIST_OF_FILE && err != UNZ_OK) {
        printf("error %d with zipfile in unzGoToNextFile\n", err);
        return err;
    }

    return 0;
}

int do_extract_currentfile(unzFile uf, int opt_extract_without_path, int* popt_overwrite, const char *password)
{
    unz_file_info64 file_info = {0};
    FILE* fout = NULL;
    void* buf = NULL;
    uInt size_buf = WRITEBUFFERSIZE;
    int err = UNZ_OK;
    int errclose = UNZ_OK;
    int skip = 0;
    char filename_inzip[256] = {0};
    char* filename_withoutpath = NULL;
    const char* write_filename = NULL;
    char* p = NULL;

    err = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
    if (err != UNZ_OK)
    {
        printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
        return err;
    }

    p = filename_withoutpath = filename_inzip;
    while (*p != 0)
    {
        if ((*p == '/') || (*p == '\\'))
            filename_withoutpath = p+1;
        p++;
    }

    /* If zip entry is a directory then create it on disk */
    if (*filename_withoutpath == 0)
    {
        if (opt_extract_without_path == 0)
        {
            printf("creating directory: %s\n", filename_inzip);
            MKDIR(filename_inzip);
        }
        return err;
    }

    buf = (void*)malloc(size_buf);
    if (buf == NULL)
    {
        printf("Error allocating memory\n");
        return UNZ_INTERNALERROR;
    }

    err = unzOpenCurrentFilePassword(uf, password);
    if (err != UNZ_OK)
        printf("error %d with zipfile in unzOpenCurrentFilePassword\n", err);

    if (opt_extract_without_path)
        write_filename = filename_withoutpath;
    else
        write_filename = filename_inzip;

    /* Determine if the file should be overwritten or not and ask the user if needed */
    if ((err == UNZ_OK) && (*popt_overwrite == 0) && (check_file_exists(write_filename)))
    {
        char rep = 0;
        do
        {
            char answer[128];
            printf("The file %s exists. Overwrite ? [y]es, [n]o, [A]ll: ", write_filename);
            if (scanf("%1s", answer) != 1)
                exit(EXIT_FAILURE);
            rep = answer[0];
            if ((rep >= 'a') && (rep <= 'z'))
                rep -= 0x20;
        }
        while ((rep != 'Y') && (rep != 'N') && (rep != 'A'));

        if (rep == 'N')
            skip = 1;
        if (rep == 'A')
            *popt_overwrite = 1;
    }

    /* Create the file on disk so we can unzip to it */
    if ((skip == 0) && (err == UNZ_OK))
    {
        fout = FOPEN_FUNC(write_filename, "wb");
        /* Some zips don't contain directory alone before file */
        if ((fout == NULL) && (opt_extract_without_path == 0) &&
            (filename_withoutpath != (char*)filename_inzip))
        {
            char c = *(filename_withoutpath-1);
            *(filename_withoutpath-1) = 0;
            makedir(write_filename);
            *(filename_withoutpath-1) = c;
            fout = FOPEN_FUNC(write_filename, "wb");
        }
        if (fout == NULL)
            printf("error opening %s\n", write_filename);
    }

    /* Read from the zip, unzip to buffer, and write to disk */
    if (fout != NULL)
    {
        printf(" extracting: %s\n", write_filename);

        do
        {
            err = unzReadCurrentFile(uf, buf, size_buf);
            if (err < 0)
            {
                printf("error %d with zipfile in unzReadCurrentFile\n", err);
                break;
            }
            if (err == 0)
                break;
            if (fwrite(buf, err, 1, fout) != 1)
            {
                printf("error %d in writing extracted file\n", errno);
                err = UNZ_ERRNO;
                break;
            }
        }
        while (err > 0);

        if (fout)
            fclose(fout);

        /* Set the time of the file that has been unzipped */
        if (err == 0)
            change_file_date(write_filename,file_info.dosDate, file_info.tmu_date);
    }

    errclose = unzCloseCurrentFile(uf);
    if (errclose != UNZ_OK)
        printf("error %d with zipfile in unzCloseCurrentFile\n", errclose);

    free(buf);
    return err;
}

int do_extract_all(unzFile uf, int opt_extract_without_path, int opt_overwrite, const char *password)
{
    int err = unzGoToFirstFile(uf);
    if (err != UNZ_OK)
    {
        printf("error %d with zipfile in unzGoToFirstFile\n", err);
        return 1;
    }

    do
    {
        err = do_extract_currentfile(uf, opt_extract_without_path, &opt_overwrite, password);
        if (err != UNZ_OK)
            break;
        err = unzGoToNextFile(uf);
    }
    while (err == UNZ_OK);

    if (err != UNZ_END_OF_LIST_OF_FILE)
    {
        printf("error %d with zipfile in unzGoToNextFile\n", err);
        return 1;
    }
    return 0;
}

int do_extract_onefile(unzFile uf, const char* filename, int opt_extract_without_path, int opt_overwrite,
    const char* password)
{
    if (unzLocateFile(uf, filename, NULL) != UNZ_OK)
    {
        printf("file %s not found in the zipfile\n", filename);
        return 2;
    }
    if (do_extract_currentfile(uf, opt_extract_without_path, &opt_overwrite, password) == UNZ_OK)
        return 0;
    return 1;
}

int main(int argc, const char *argv[])
{
    const char *zipfilename = NULL;
    const char *filename_to_extract = NULL;
    const char *password = NULL;
    int i = 0;
    int ret = 0;
    int opt_do_list = 0;
    int opt_do_extract = 1;
    int opt_do_extract_withoutpath = 0;
    int opt_overwrite = 0;
    int opt_extractdir = 0;
    const char *dirname = NULL;
    unzFile uf = NULL;

    do_banner();
    if (argc == 1)
    {
        do_help();
        return 0;
    }

    /* Parse command line options */
    for (i = 1; i < argc; i++)
    {
        if ((*argv[i]) == '-')
        {
            const char *p = argv[i]+1;

            while (*p != 0)
            {
                char c = *(p++);
                if ((c == 'l') || (c == 'L'))
                    opt_do_list = 1;
                if ((c == 'v') || (c == 'V'))
                    opt_do_list = 1;
                if ((c == 'x') || (c == 'X'))
                    opt_do_extract = 1;
                if ((c == 'e') || (c == 'E'))
                    opt_do_extract = opt_do_extract_withoutpath = 1;
                if ((c == 'o') || (c == 'O'))
                    opt_overwrite=1;
                if ((c == 'd') || (c == 'D'))
                {
                    opt_extractdir = 1;
                    dirname = argv[i+1];
                }

                if (((c == 'p') || (c == 'P')) && (i+1 < argc))
                {
                    password = argv[i+1];
                    i++;
                }
            }
        }
        else
        {
            if (zipfilename == NULL)
                zipfilename = argv[i];
            else if ((filename_to_extract == NULL) && (!opt_extractdir))
                filename_to_extract = argv[i];
        }
    }

    /* Open zip file */
    if (zipfilename != NULL)
    {
#ifdef USEWIN32IOAPI
        zlib_filefunc64_def ffunc;
        fill_win32_filefunc64A(&ffunc);
        uf = unzOpen2_64(zipfilename, &ffunc);
#else
        uf = unzOpen64(zipfilename);
#endif
    }

    if (uf == NULL)
    {
        printf("Cannot open %s\n", zipfilename);
        return 1;
    }

    printf("%s opened\n", zipfilename);

    /* Process command line options */
    if (opt_do_list == 1)
    {
        ret = do_list(uf);
    }
    else if (opt_do_extract == 1)
    {
        if (opt_extractdir && CHDIR(dirname))
        {
            printf("Error changing into %s, aborting\n", dirname);
            exit(-1);
        }

        if (filename_to_extract == NULL)
            ret = do_extract_all(uf, opt_do_extract_withoutpath, opt_overwrite, password);
        else
            ret = do_extract_onefile(uf, filename_to_extract, opt_do_extract_withoutpath, opt_overwrite, password);
    }

    unzClose(uf);
    return ret;
}
