#pragma once
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "tinyxml2.h"
#include "MiniExcelReader.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <iostream>
#include <vector>


#if NF_PLATFORM == NF_PLATFORM_WIN
#include <io.h>
#include <windows.h>
#include <conio.h>
#else
#include <iconv.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>

int getch() {
	struct termios oldt,
		newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#endif



class FileProcess
{
public:
	FileProcess();
	~FileProcess();

	void CreateProtoFile();
	void CreateNameFile();
	void OnCreateXMLFile();
	void CreateStructThreadFunc();
	void CreateIniThreadFunc();
	bool CreateStructXML(std::string strFile, std::string strFileName);
	bool CreateIniXML(std::string strFile);
	void OnCreateMysqlFile();
	bool LoadLogicClass(std::string strFile);
	bool LoadClass(std::string strFile, std::string strTable);

private:
	int nCipher = 0;
	std::string strCipherCfg = "conf";

	std::string strExecutePath = "NFDataCfg/";
	std::string strToolBasePath = "../";
	std::string strRelativePath = "../../";
	std::string strExcelStructPath = "Excel_Struct/";
	std::string strXMLStructPath = "Struct/Class/";

	std::string strExcelIniPath = "Excel_Ini/";
	std::string strXMLIniPath = "Ini/NPC/";

	std::string strLogicClassFile = "";
	std::string strMySQLFile = "../mysql/NFrame.sql";
	std::string strMySQLClassFile = "../mysql/NFClass.sql";

	std::string strProtoFile = "../proto/NFRecordDefine.proto";

	std::string strHPPFile = "../proto/NFProtocolDefine.hpp";
	std::string strJavaFile = "../proto/NFProtocolDefine.java";
	std::string strCSFile = "../proto/NFProtocolDefine.cs";

	FILE* mysqlWriter = nullptr;
	FILE* mysqlClassWriter = nullptr;
	FILE* protoWriter = nullptr;
	FILE* hppWriter = nullptr;
	FILE* javaWriter = nullptr;
	FILE* csWriter = nullptr;

	std::string strHppIObjectInfo;
	std::string strJavaIObjectInfo;
	std::string strCSIObjectInfo;

	int nRecordStart = 11;

	std::vector<std::string> GetFileListInFolder(std::string folderPath, int depth)
	{
		std::vector<std::string> result;
#if NF_PLATFORM == NF_PLATFORM_WIN
		_finddata_t FileInfo;
		std::string strfind = folderPath + "\\*";
		long long Handle = _findfirst(strfind.c_str(), &FileInfo);


		if (Handle == -1L)
		{
			std::cerr << "can not match the folder path" << std::endl;
			exit(-1);
		}
		do {
			//判断是否有子目录
			if (FileInfo.attrib & _A_SUBDIR)
			{
				//这个语句很重要
				if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
				{
					std::string newPath = folderPath + "\\" + FileInfo.name;
					//dfsFolder(newPath, depth);
				}
			}
			else
			{

				std::string filename = (folderPath + "\\" + FileInfo.name);
				result.push_back(filename);
			}
		} while (_findnext(Handle, &FileInfo) == 0);


		_findclose(Handle);
#else
	DIR *pDir;
	struct dirent *ent;
	char childpath[512];
	char absolutepath[512];
	pDir = opendir(folderPath.c_str());
	memset(childpath, 0, sizeof(childpath));
	while ((ent = readdir(pDir)) != NULL)
	{
		if (ent->d_type & DT_DIR)
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			{
				continue;
			}
		}
		else
		{
			sprintf(absolutepath, "%s/%s", folderPath.c_str(), ent->d_name);
			result.push_back(absolutepath);
		}
	}

	sort(result.begin(), result.end());//排序
#endif
		return result;
	}

	void StringReplace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
	{
		std::string::size_type pos = 0;
		std::string::size_type srclen = strsrc.size();
		std::string::size_type dstlen = strdst.size();

		while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
		{
			strBig.replace(pos, srclen, strdst);
			pos += dstlen;
		}
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	void UTF8ToGBK(char* src, char* dst, int len)
	{
		int ret = 0;
		WCHAR* strA;
		int i = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		if (i <= 0) {
			printf("ERROR.");
			return;
		}
		strA = (WCHAR*)malloc(i * 2);
		MultiByteToWideChar(CP_UTF8, 0, src, -1, strA, i);
		i = WideCharToMultiByte(CP_ACP, 0, strA, -1, NULL, 0, NULL, NULL);
		if (len >= i) {
			ret = WideCharToMultiByte(CP_ACP, 0, strA, -1, dst, i, NULL, NULL);
			dst[i] = 0;
		}
		if (ret <= 0) {
			free(strA);
			return;
		}

		free(strA);
	}

	void GBKToUTF8(char* src, char* dst, int len)
	{
		int ret = 0;
		WCHAR* strA;
		int i = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		if (i <= 0) {
			printf("ERROR.");
			return;
		}
		strA = (WCHAR*)malloc(i * 2);
		MultiByteToWideChar(CP_ACP, 0, src, -1, strA, i);
		i = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
		if (len >= i) {
			ret = WideCharToMultiByte(CP_UTF8, 0, strA, -1, dst, i, NULL, NULL);
			dst[i] = 0;
		}

		if (ret <= 0) {
			free(strA);
			return;
		}
		free(strA);
	}
#else   //Linux
	// starkwong: In iconv implementations, inlen and outlen should be type of size_t not uint, which is different in length on Mac
	void UTF8ToGBK(char* src, char* dst, int len)
	{
		int ret = 0;
		size_t inlen = strlen(src) + 1;
		size_t outlen = len;
		char* inbuf = src;
		char* outbuf = dst;
		iconv_t cd;
		cd = iconv_open("GBK", "UTF-8");
		if (cd != (iconv_t)-1) {
			ret = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
			if (ret != 0) {
				printf("iconv failed err: %s\n", strerror(errno));
			}

			iconv_close(cd);
		}
	}

	void GBKToUTF8(char* src, char* dst, int len)
	{
		int ret = 0;
		size_t inlen = strlen(src) + 1;
		size_t outlen = len;
		char* inbuf = src;
		char* outbuf2 = NULL;
		char* outbuf = dst;
		iconv_t cd;

		// starkwong: if src==dst, the string will become invalid during conversion since UTF-8 is 3 chars in Chinese but GBK is mostly 2 chars
		if (src == dst) {
			outbuf2 = (char*)malloc(len);
			memset(outbuf2, 0, len);
			outbuf = outbuf2;
		}

		cd = iconv_open("UTF-8", "GBK");
		if (cd != (iconv_t)-1) {
			ret = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
			if (ret != 0)
				printf("iconv failed err: %s\n", strerror(errno));

			if (outbuf2 != NULL) {
				strcpy(dst, outbuf2);
				free(outbuf2);
			}

			iconv_close(cd);
		}
	}
#endif
	bool IsTextUTF8(const char* str, long length)
	{
		int i;
		int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
		unsigned char chr;
		bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
		for (i = 0;i < length;i++)
		{
			chr = *(str + i);
			if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
				bAllAscii = false;
			if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
			{
				if (chr >= 0x80)
				{
					if (chr >= 0xFC && chr <= 0xFD)
						nBytes = 6;
					else if (chr >= 0xF8)
						nBytes = 5;
					else if (chr >= 0xF0)
						nBytes = 4;
					else if (chr >= 0xE0)
						nBytes = 3;
					else if (chr >= 0xC0)
						nBytes = 2;
					else
					{
						return false;
					}
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx
			{
				if ((chr & 0xC0) != 0x80)
				{
					return false;
				}
				nBytes--;
			}
		}

		if (nBytes > 0) //违返规则
		{
			return false;
		}

		if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
		{
			return false;
		}
		return true;
	}
};
