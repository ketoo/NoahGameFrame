#include <fcntl.h>
#include "httppil.h"
#include "httpapi.h"
#include "httpxml.h"
#include "httpvod.h"
#include "crc32.h"

#define PRE_ALLOC_UNIT 16

static VOD_CTX vodctx;
static int listcount=0;
static int playcount=0;
static char *vodloop = 0;
static char *vodhost = 0;
static char vodbuf[256];
static unsigned long* hashmap = 0;

#define MAX_CHARS 30
static int charsinfo[MAX_CHARS + 1];
static char *vodroot=NULL;
static char** filelist = NULL;
static int filecount = 0;
static CATEGORY_INFO cats;
static int prefixlen = 0;

static int nextarg = 0;

int EnumDir(char* pchDir)
{
	int i;
	char buf[256];
	char* path;
	int dirlen = strlen(pchDir) + 1;
	int pathlen = 0;
	char** dirlist = NULL;
	int dircount = 0;

	for (i = ReadDir(pchDir, buf); !i; i = ReadDir(NULL, buf)) {
		int len;
		if (buf[0] == '.' && (buf[1] == 0 || (buf[1] == '.' && buf[2] == 0))) continue;
		len = dirlen + strlen(buf) + 1;
		if (len > pathlen) {
			if (pathlen) free(path);
			path = malloc(len);
			pathlen = len;
		}
		sprintf(path, "%s/%s", pchDir, buf);
		if (IsDir(path)) {
			if (!(dircount % PRE_ALLOC_UNIT)) {
				dirlist = realloc(dirlist, (dircount + PRE_ALLOC_UNIT) * sizeof(char*));
			}
			dirlist[dircount++] = strdup(buf);
		} else {
			if (!(filecount % PRE_ALLOC_UNIT)) {
				filelist = realloc(filelist, (filecount + PRE_ALLOC_UNIT) * sizeof(char*));
			}
			filelist[filecount++] = strdup(path + prefixlen);
			//printf("%s\n", path);
		}
	}
	for (i = 0; i < dircount; i++) {
		int len = dirlen + strlen(dirlist[i]) + 1;
		if (len > pathlen) {
			if (pathlen) free(path);
			path = malloc(len);
			pathlen = len;
		}
		sprintf(path, "%s/%s", pchDir, dirlist[i]);
		free(dirlist[i]);
		EnumDir(path);
	}
	free(dirlist);
	if (pathlen) free(path);
	return 0;
}

int GetCategoryHash(CATEGORY_INFO* ci)
{
	char *p = ci->name;
	int i;
	unsigned sum = 0;
	for (i = 0; p[i]; i++) sum += p[i];
	return sum % 100;
}

int GetTitleHash(CLIP_INFO* ci)
{
	unsigned long val;
	crc32Init(&val);
	crc32Update(&val, ci->title, strlen(ci->title));
	crc32Finish(&val);
	return val % 10000;
}

char* FilterDup(char* str)
{
	char *newstr = (char*)malloc(strlen(str) + 1);
	char *p, *q;
	for (p = str, q = newstr; *p; p++, q++) {
		switch (*p) {
		case '_':
			*q = ' ';
			break;
		case '&':
			*q = ',';
			break;
		default:
			*q = *p;
		}
	}
	*q = 0;
	return newstr;
}

CATEGORY_INFO* FindCategory(char* name)
{
	int pos = 0;
	CATEGORY_INFO* ptr;
	CATEGORY_INFO* prev = 0;
	if (!name || !*name) {
		cats.count++;
		return &cats;
	}
	{
	char *p = strchr(name, ',');
	if (p) *p = 0;
	}
	ptr = &cats;
	for (;;) {
		int n = strcmp(ptr->name, name);
		if (n == 0) {
			ptr->count++;
			return ptr;
		} else if (n > 0 && prev) {
			prev->next = (CATEGORY_INFO*)calloc(1, sizeof(CATEGORY_INFO));
			prev->next->next = ptr;
			ptr = prev->next;
			break;
		} else if (!ptr->next) {
			ptr->next = (CATEGORY_INFO*)calloc(1, sizeof(CATEGORY_INFO));
			ptr = ptr->next;
			break;
		}
		prev = ptr;
		ptr = ptr->next;		
	}

	ptr->name = FilterDup(name);
	ptr->count = 1;
	ptr->hash = GetCategoryHash(ptr);
	return ptr;
}

int IsHashed(int hash, int autoset)
{
	int dwoff = hash >> 5;
	int bitmask = 1 << (hash & 0x1f);
	if (hashmap[dwoff] & bitmask) return 1;
	if (autoset) hashmap[dwoff] |= bitmask;
	return 0;
}

CLIP_INFO* GetClipByHash(int hash, CATEGORY_INFO** pcat)
{
	int cathash = hash / 10000;
	CATEGORY_INFO* cat;
	for (cat = &cats; cat; cat = cat->next) {
		if (cathash == cat->hash) {
			CLIP_INFO* ptr;
			for (ptr = cat->clips; ptr; ptr = ptr->next) {
				if (ptr->hash == hash) {
					if (pcat) *pcat = cat;
					return ptr;
				}
			}
		}
	}
	return 0;
}

CLIP_INFO* GetClipByName(char* catname, char* title, CATEGORY_INFO** pcat)
{
	CATEGORY_INFO* cat;
	for (cat = &cats; cat; cat = cat->next) {
		if (!catname || !strcmp(catname, cat->name)) {
			CLIP_INFO* ptr;
			for (ptr = cat->clips; ptr; ptr = ptr->next) {
				if (!title || !strcmp(title, ptr->title)) {
					if (pcat) *pcat = cat;
					return ptr;
				}
			}
		}
	}
	return 0;
}

CLIP_INFO* GetClipByFile(char* filename, CATEGORY_INFO** pcat)
{
	CATEGORY_INFO* cat;
	for (cat = &cats; cat; cat = cat->next) {
		CLIP_INFO* ptr;
		for (ptr = cat->clips; ptr; ptr = ptr->next) {
			if (!strcmp(filename, ptr->filename)) {
				if (pcat) *pcat = cat;
				return ptr;
			}
		}
	}
	return 0;
}

FILE* fpdup;

int AddClip(char* filename)
{
	CLIP_INFO* pinfo;
	CATEGORY_INFO* cat;
	char buf[256];
	char *p;
	char *s;
	int i;
	s = strrchr(filename, '/');
	if (!(s++)) s = filename;
	if (s[5] == ' ' && atoi(s) > 0) s += 5;
	while (*s == ' ') s++;
	if (*s == '.') return -1;
	pinfo = (CLIP_INFO*)malloc(sizeof(CLIP_INFO));
	pinfo->next = 0;
	pinfo->filename = filename;
	p = strrchr(s, '[');
	if (p) {
		// get category
		strcpy(buf, p + 1);
		p = strchr(buf, ']');
		if (p) *p = 0;
		cat = FindCategory(buf);

		// get title
		strcpy(buf, s);
		p = strchr(buf, '[');
		while (--p >= buf && *p == ' ');
		*(p + 1) = 0;
		pinfo->title = FilterDup(buf);
	} else {
		strcpy(buf, s);
		s = buf;
		if (p = strstr(s, " - ")) {
			*p = 0;
			cat = FindCategory(s);
			s = p + 3;
			p = strrchr(s, '.');
			if (p) *p = 0;
			pinfo->title = FilterDup(s);
		} else if ((p = strchr(s, '-')) || (p = strchr(s, ' '))) {
			*p = 0;
			cat = FindCategory(s);
			for (s = p + 1; *s == '-' || *s == ' '; s++);
			p = strrchr(s, '.');
			if (p) *p = 0;
			pinfo->title = FilterDup(s);
		} else {
			cat = FindCategory(0);
			p = strrchr(s, '.');
			if (p) *p = 0;
			pinfo->title = FilterDup(s);
		}
	}
	pinfo->hash = cat->hash * 10000 + GetTitleHash(pinfo);
	for (i = 0, p = pinfo->title; *p && *p != '(' && *p != '['; p++) {
		if (*p < 0) {
			p++;
			i++;
		} else if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <='Z') || (*p >= '0' && *p <='9')) {
			i++;
		}
	}
	pinfo->chars = i;
	charsinfo[(i > MAX_CHARS || i < 1) ? 0 : i]++;

	while (IsHashed(pinfo->hash, 1)) {
		CATEGORY_INFO* excat;
		CLIP_INFO* exclip = GetClipByHash(pinfo->hash, &excat);
		if (!exclip) break;
		if (excat == cat && !strcmp(exclip->title, pinfo->title)) {	
			fprintf(fpdup, "<li>%06d %s [ <a href='/vodstream?file=%s'>%s</a> ] [ <a href='/vodstream?file=%s'>%s</a> ]</li>\n",
				pinfo->hash, cat->name, exclip->filename, exclip->title, pinfo->filename, pinfo->title);	
		} else {
			//printf("HASH FAULT @ %06d Exist: %s - %s New: %s - %s\n", pinfo->hash, cat->name, exclip->title, cat->name, pinfo->title);	
		}
		pinfo->hash++;
	}

	// add clip to category
	if (!cat->clips) {
		cat->clips = pinfo;
	} else {
		CLIP_INFO *ptr = cat->clips;
		CLIP_INFO *prev = 0;
		for (;;) {
			int n = strcmp(ptr->title, pinfo->title);
			if (n > 0) {
				if (!prev) {
					pinfo->next = cat->clips;
					cat->clips = pinfo;
				} else {
					pinfo->next = prev->next;
					prev->next = pinfo;
				}
				break;
			}
			if (!ptr->next) {
				ptr->next = pinfo;
				break;
			}
			prev = ptr;
			ptr = ptr->next;
		}
	}
	return 0;
}

void vodInit()
{
	int i;
	int code = 0;
	int count = 0;
	if (!vodroot)
		return;
	memset(&vodctx, 0, sizeof(vodctx));
	memset(&charsinfo, 0, sizeof(charsinfo));
	hashmap = calloc(1000000 / 32, sizeof(long));
	//sprintf(vodroot,"%s",hp->pchWebPath);
	cats.name = "";
	cats.hash = GetCategoryHash(&cats);
	cats.next = 0;
	prefixlen = strlen(vodroot) + 1;
 	EnumDir(vodroot);

	fpdup = fopen("htdocs/dup.htm", "w");
	fprintf(fpdup, "<html><body>");
	for (i = 0; i < filecount; i++) {
		if (!AddClip(filelist[i])) count++;
	}
	fprintf(fpdup, "</body></html>");
	fclose(fpdup);

	printf("\n\nCount: %d\n", count);
}

int ehVod(MW_EVENT event, int argi, void* argp)
{
	switch (event) {
	case MW_INIT:
		if (hashmap) return 0; 	// already inited
		vodInit();
		break;
	case MW_UNINIT:
		//un-initialization
		if (hashmap) {
			free(hashmap);
			hashmap = 0;
			if (vodloop) {
				free(vodloop);
				vodloop = 0;
			}
		}
		break;
	case MW_PARSE_ARGS: {
		int i = 0;
		char** argv = (char**)argp;
		for (i = 0; i < argi; i++) {
			if (!strcmp(argv[i], "--vodroot")) {
				vodroot = argv[++i];
			} else if (!strcmp(argv[i], "--vodloop")) {
				i++;
				if (vodhost) {
					vodloop = (char*)malloc(strlen(vodhost) + strlen(argv[i]) + 1);
					sprintf(vodloop, "%s%s", vodhost, argv[i]);
				} else {
					vodloop = strdup(argv[i]);
				}
			} else if (!strcmp(argv[i], "--vodhost")) {
				vodhost = argv[++i];
			}
		}
		}
	}
	return 0;
}

#define VOD_LOOP_COUNT 4

#ifdef WIN32
#define OPEN_FLAG O_RDONLY|0x8000
#else
#define OPEN_FLAG O_RDONLY
#endif

int uhVodStream(UrlHandlerParam* param)
{
	static int code = 0;
	static int prevtime = 0;
	int id;
	char* file;
	mwParseQueryString(param);
	file = mwGetVarValue(param->pxVars, "file", 0);
	id = mwGetVarValueInt(param->pxVars, "id" , -1);
	if (!file) {
		CLIP_INFO* clip = GetClipByHash(id, 0);
		if (clip) {
			snprintf(param->pucBuffer, param->dataBytes, "~%s/%s", vodroot, clip->filename);
			return FLAG_DATA_FILE;
		}
	} else {
		snprintf(param->pucBuffer, param->dataBytes, "~%s/%s", vodroot, file);
	}
	return 0;
}

int uhStream(UrlHandlerParam* param)
{
	char* file;
	mwParseQueryString(param);
	file = mwGetVarValue(param->pxVars, "file", 0);
	param->hs->fd = _open(file, _O_BINARY|_O_RDONLY);
	return FLAG_DATA_FD;
}

static void OutputItemInfo(char** pbuf, int* pbufsize, char* id)
{
	char buf[256];
	CATEGORY_INFO *cat;
	CLIP_INFO* clip;
	int hash = atoi(id);
	if (hash > 0)
		clip = GetClipByHash(hash, &cat);
	else
		clip = GetClipByName(0, id, &cat);
	if (!clip) return;
	mwWriteXmlString(pbuf, pbufsize, 2, "<item>");
	snprintf(buf, sizeof(buf), "<id><![CDATA[%s]]></id>", id);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	snprintf(buf, sizeof(buf), "<file><![CDATA[%s]]></file>", clip->filename);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	snprintf(buf, sizeof(buf), "<title><![CDATA[%s]]></title>", clip->title);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	snprintf(buf, sizeof(buf), "<category><![CDATA[%s]]></category>", cat->name);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	mwWriteXmlString(pbuf, pbufsize, 2, "</item>");
}

int uhLib(UrlHandlerParam* param)
{
	char *pbuf;
	int bufsize;
	char buf[256];
	char *id;
	char *p;
	int from, count;
	
	p = strstr(param->pucRequest, "back=");
	if (p) *p = 0;
	mwParseQueryString(param);

	id = mwGetVarValue(param->pxVars, "id", 0);
	from = mwGetVarValueInt(param->pxVars, "from", 0);
	count = mwGetVarValueInt(param->pxVars, "count", -1);

	if (count <= 0)
		bufsize = 1024 * 1024;
	else
		bufsize = count * 256;

	pbuf = (char*)malloc(bufsize);
	param->pucBuffer = pbuf;

	mwWriteXmlHeader(&pbuf, &bufsize, 10, "gb2312", mwGetVarValue(param->pxVars, "xsl", 0));
	mwWriteXmlString(&pbuf, &bufsize, 0, "<response>");

	if (!strcmp(param->pucRequest, "/category")) {
		int mincount = mwGetVarValueInt(param->pxVars, "min", 2);
		int catid = id ? atoi(id) : -1;
		int hash = mwGetVarValueInt(param->pxVars, "hash", -1);
		char* name = mwGetVarValue(param->pxVars, "name", 0);
		CATEGORY_INFO* cat;
		int i = 0;
		int idx = 0;
		for (cat = &cats; cat; cat = cat->next, i++) {
			if ((hash >= 0 && hash != cat->hash) || (name && strcmp(name, cat->name)) || (catid >= 0 && catid != i) || cat->count < mincount)
				continue;

			if (idx >= from) { 
				if ((count--) == 0) break;

				snprintf(buf, sizeof(buf), "<category id=\"%d\" hash=\"%02d\" index=\"%03d\">", i, cat->hash, idx);
				mwWriteXmlString(&pbuf, &bufsize, 1, buf);

				snprintf(buf, sizeof(buf), "<name><![CDATA[%s]]></name>", cat->name);
				mwWriteXmlString(&pbuf, &bufsize, 1, buf);

				snprintf(buf, sizeof(buf), "<clips>%d</clips>", cat->count);
				mwWriteXmlString(&pbuf, &bufsize, 1, buf);

				mwWriteXmlString(&pbuf, &bufsize, 1, "</category>");
			}
			idx++;
		}
	} else if (!strcmp(param->pucRequest, "/title")) {
		int hash = id ? atoi(id) : -1;
		int chars = mwGetVarValueInt(param->pxVars, "chars", 0);
		char* catname = mwGetVarValue(param->pxVars, "category", 0);
		int catid = mwGetVarValueInt(param->pxVars, "catid", -1);
		int i = 0;
		int idx = 0;
		BOOL matched = 0;
		CLIP_INFO* info;
		CATEGORY_INFO* cat;
		int pos = 1;
		for (cat = &cats; cat; cat = cat->next, i++) {
			if ((catid >= 0 && catid != i) || (catname && strcmp(catname, cat->name))) continue;
			for (info = cat->clips; info; info = info->next) {
				if ((hash >= 0 && hash != info->hash) || (chars && info->chars != chars)) continue;
				if (idx >= from) { 
					if (count == 0) break;
					count--;
					if (!matched) {
						snprintf(buf, sizeof(buf), "<category name=\"%s\">", cat->name);
						mwWriteXmlString(&pbuf, &bufsize, 1, buf);
						matched = 1;
					}
					snprintf(buf, sizeof(buf), "<item id=\"%06d\" pos=\"%d\">", info->hash, pos++);
					mwWriteXmlString(&pbuf, &bufsize, 2, buf);

					snprintf(buf, sizeof(buf), "<name><![CDATA[%s]]></name>", info->title);
					mwWriteXmlString(&pbuf, &bufsize, 2, buf);

					snprintf(buf, sizeof(buf), "<chars>%d</chars>", info->chars);
					mwWriteXmlString(&pbuf, &bufsize, 2, buf);

					mwWriteXmlString(&pbuf, &bufsize, 2, "</item>");
				}
				idx++;
			}
			if (matched) mwWriteXmlString(&pbuf, &bufsize, 1, "</category>");
			if (count == 0) break;
			matched = FALSE;
		}
	} else if (!strcmp(param->pucRequest, "/chars")) {
		int i;
		for (i = 1; i <= MAX_CHARS; i++) {
			if (i > from) { 
				if ((count--) == 0) break;
				snprintf(buf, sizeof(buf), "<category chars=\"%d\" count=\"%d\"/>", i, charsinfo[i]);
				mwWriteXmlString(&pbuf, &bufsize, 2, buf);
			}
		}
	} else if (!strcmp(param->pucRequest, "/query")) {
		char buf[256];
		CATEGORY_INFO *cat;
		CLIP_INFO* clip;
		int idx = 0;
		int hash = id ? atoi(id) : -1;
		if (hash > 0)
			clip = GetClipByHash(hash, &cat);
		else
			clip = GetClipByName(0, id, &cat);
		if (!clip) clip = GetClipByFile(id, &cat);
		if (clip) {
			mwWriteXmlString(&pbuf, &bufsize, 2, "<item>");
			snprintf(buf, sizeof(buf), "<id><![CDATA[%d]]></id>", clip->hash);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			snprintf(buf, sizeof(buf), "<file><![CDATA[%s]]></file>", clip->filename);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			snprintf(buf, sizeof(buf), "<title><![CDATA[%s]]></title>", clip->title);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			snprintf(buf, sizeof(buf), "<category><![CDATA[%s]]></category>", cat->name);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			mwWriteXmlString(&pbuf, &bufsize, 2, "</item>");
		}
	}

	mwWriteXmlString(&pbuf, &bufsize, 0, "</response>");

	param->dataBytes=(int)(pbuf-param->pucBuffer);
	param->fileType=HTTPFILETYPE_XML;
	return FLAG_DATA_RAW | FLAG_TO_FREE;
}

static VOD_CTX* GetVodContext(DWORD ip)
{
	VOD_CTX* ctx = &vodctx;
	if (!ip) return NULL;
	for (;;) {
		if (ctx->ip == 0) break;
		if (ctx->ip == ip) return ctx;
		if (!ctx->next) {
			ctx->next = (VOD_CTX*)calloc(1, sizeof(VOD_CTX));
			ctx = ctx->next;
			ctx->next = 0;
			ctx->playlist = 0;
			break;
		}
		ctx = ctx->next;
	}
	ctx->ip = ip;
	return ctx;
}

int uhVod(UrlHandlerParam* param)
{
	HTTP_XML_NODE node;
	char *req=param->pucRequest;
	char *pbuf = param->pucBuffer;
	int bufsize = param->dataBytes;
	char *action;
	PL_ENTRY *ptr;
	VOD_CTX* ctx;
	int i;

	if (*req && *req != '?') return 0;
	node.indent = 1;
	node.name = "state";
	node.fmt = "%s";
	node.flags = 0;
	mwWriteXmlHeader(&pbuf, &bufsize, 10, "gb2312", mwGetVarValue(param->pxVars, "xsl", 0));
	mwWriteXmlString(&pbuf, &bufsize, 0, "<response>");

	mwParseQueryString(param);

	ctx = GetVodContext(param->hs->ipAddr.laddr);

	action = mwGetVarValue(param->pxVars, "action", 0);

	if (!action) {
		int count = mwGetVarValueInt(param->pxVars, "count", -1);
		ptr = ctx->playlist;
		mwWriteXmlString(&pbuf, &bufsize, 1, "<playlist>");
		for (i=0; ptr && (unsigned int)i < (unsigned int)count; ptr = ptr->next, i++) {
			char buf[32];
			snprintf(buf, sizeof(buf), "<item index=\"%03d\">", i);
			mwWriteXmlString(&pbuf, &bufsize, 2, buf);

			node.indent = 3;
			node.name = "stream";
			node.fmt = "%s";
			node.value = ptr->data;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);

			node.flags = XN_CDATA;
			node.name = "title";
			node.value = (char*)ptr->data + strlen(ptr->data) + 1;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);

			mwWriteXmlString(&pbuf, &bufsize, 2, "</item>");
		}
		mwWriteXmlString(&pbuf, &bufsize, 1, "</playlist>");
	} else if (!strcmp(action, "add")) {
		char *filename = mwGetVarValue(param->pxVars, "stream", 0);
		char *title = mwGetVarValue(param->pxVars, "title", 0);
		if (!title) title = "";
		if (!filename) {
			node.value = "error";
		} else {
			int fnlen;
			int titlelen;
			char *entrydata;
			mwDecodeString(filename);
			fnlen = strlen(filename);
			titlelen = strlen(title);
			entrydata = (char*)malloc(fnlen + titlelen + 3);
			strcpy(entrydata, filename);
			strcpy(entrydata + fnlen + 1, title);
			node.value = plAddEntry(&ctx->playlist, entrydata, fnlen + titlelen + 2) ? "OK" : "error";
		}
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
	} else if (!strcmp(action, "pin")) {
		int index = mwGetVarValueInt(param->pxVars, "arg", 0);
		node.value = plPinEntryByIndex(&ctx->playlist, index) ? "OK" : "error";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
	} else if (!strcmp(action, "del")) {
		int index = mwGetVarValueInt(param->pxVars, "arg", 0);
		void* data = plDelEntryByIndex(&ctx->playlist, index) ? "OK" : "error";
		if (data) {
			free(data);
			node.value = "OK";
		} else {
			node.value = "error";
		}
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
	} else if (!strcmp(action, "play")) {
		char* stream = (char*)plGetEntry(&ctx->playlist);
		if (stream) {
			node.name = "stream";
		}
		node.name = "stream";
		node.value = stream ? stream : vodloop;
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		node.name = "control";
		node.fmt = "%s/vodplay?action=control";
		node.value = vodhost;
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		if (stream) free(stream);
		ctx->nextaction = 0;
	} else if (!strcmp(action, "control")) {
		int arg = mwGetVarValueInt(param->pxVars, "arg", 0);
		if (arg)
			ctx->nextaction = arg;
		else if (ctx->nextaction) {
			node.name = "action";
			node.fmt = "%d";
			node.value = (void*)ctx->nextaction;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
			ctx->nextaction = 0;
		}
	}

#if 0
	arg = mwGetVarValue(param->pxVars, "arg", 0);
	id = mwGetVarValue(param->pxVars, "id", 0);
	switch (GETDWORD(param->pucRequest + 1)) {
	case DEFDWORD('n','o','p',0):
		strcpy(pbuf,"state=OK");
		break;
	case DEFDWORD('c','m','d',0):
		//action=ACT_SKIP;
		strcpy(pbuf,"Play next");
		param->dataBytes=9;
		return FLAG_DATA_RAW;
	case DEFDWORD('l','i','s','t'): {
		PL_ENTRY *ptr = plhdr[session];
		int i;
		mwWriteXmlString(&pbuf, &bufsize, 1, "<playlist>");
		for (i=0; ptr; ptr = ptr->next, i++) {
			OutputItemInfo(&pbuf, &bufsize, ptr->data);
		}
		mwWriteXmlString(&pbuf, &bufsize, 1, "</playlist>");
		} break;
	case DEFDWORD('a','d','d',0): {
		node.name = "state";
		if (plFindEntry(plhdr[session],(void*)id, strlen(id))) {
			OutputItemInfo(&pbuf, &bufsize, id);
			node.value = "ordered";
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
			break;
		}
		if (plAddEntry(&plhdr[session], strdup(id), strlen(id) + 1)) {
			node.value =  "OK";
			OutputItemInfo(&pbuf, &bufsize, id);
		} else {
			node.value =  "error";
		}
		node.name = "state";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		} break;
	case DEFDWORD('d','e','l',0):
		OutputItemInfo(&pbuf, &bufsize, id);
		node.name = "state";
		node.value = plDelEntry(&plhdr[session],(void*)id) ? "OK" : "error";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		break;
	case DEFDWORD('p','i','n',0):
		OutputItemInfo(&pbuf, &bufsize, id);
		node.name = "state";
		node.value = plPinEntry(&plhdr[session],(void*)id) ? "OK" : "error";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		break;
	case DEFDWORD('p','l','a','y'): {
		void* data = plGetEntry(&plhdr[session]);
		node.name = "state";
		if (data) {
			OutputItemInfo(&pbuf, &bufsize, data);
			node.value = "OK";
			free(data);
		} else {
			node.value = "error";
		}
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		} break;
	default:
		strcpy(pbuf,"Invalid request");
	}
#endif	
	mwWriteXmlString(&pbuf, &bufsize, 0, "</response>");
	param->dataBytes=(int)(pbuf-param->pucBuffer);
	param->fileType=HTTPFILETYPE_XML;
	return FLAG_DATA_RAW;
}

//////////////////////////////////////////////////////////////////
// Playlist implementation
//////////////////////////////////////////////////////////////////

PL_ENTRY* plAddEntry(PL_ENTRY **hdr, void* data, int datalen)
{
	PL_ENTRY *ptr=*hdr;
	if (!ptr) {
		// allocate header
		*hdr=(PL_ENTRY*)malloc(sizeof(PL_ENTRY));
		ptr=*hdr;
	} else {
		// travel to the end of list
		while (ptr->next) ptr=ptr->next;
		ptr->next=(PL_ENTRY*)malloc(sizeof(PL_ENTRY));
	 	ptr=ptr->next;
	}
	ptr->data=data;
	ptr->datalen = datalen;
	ptr->next=NULL;
	listcount++;
	return ptr;
}

void* plGetEntry(PL_ENTRY **hdr)
{
	PL_ENTRY *ptr=*hdr;
	void* data;
	if (!ptr) return NULL;
	data=ptr->data;
	*hdr=ptr->next;
	free(ptr);
	listcount--;
	return data;
}

PL_ENTRY* plFindEntry(PL_ENTRY *hdr, void* data, int datalen)
{
	PL_ENTRY *ptr;
	for (ptr=hdr; ptr; ptr=ptr->next) {
		if (!memcmp(ptr->data, data, min(datalen, ptr->datalen))) return ptr;
	}
	return NULL;
}

void* plDelEntry(PL_ENTRY **hdr, void* data)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	void* deleted;
	if (!ptr) return NULL;
	if (ptr->data==data) {
		deleted=ptr->data;
		*hdr=ptr->next;
		free(ptr);
		listcount--;
		return deleted;
	}
	for (prev=ptr, ptr=ptr->next; ptr; prev=ptr, ptr=ptr->next) {
		if (ptr->data==data) { 
			deleted=ptr->data;
			prev->next=ptr->next;
			free(ptr);
			listcount--;
			return deleted;
		}
	}
	return NULL;
}

void* plDelEntryByIndex(PL_ENTRY **hdr, int index)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	void* deleted;
	int i;
	if (!ptr) return NULL;
	if (index == 0) {
		deleted = ptr->data;
		*hdr = ptr->next;
		free(ptr);
		listcount--;
		return deleted;
	}
	for (i = 0; i < index && ptr; i++, prev = ptr, ptr = ptr->next);
	if (ptr) {
		deleted = ptr->data;
		prev->next = ptr->next;
		free(ptr);
		listcount--;
		return deleted;
	}
	return NULL;
}

PL_ENTRY* plPinEntry(PL_ENTRY **hdr, void* data)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	while (ptr) {
		if (ptr->data==data) {
			if (!prev) return ptr;
			prev->next=ptr->next;
			ptr->next=*hdr;
			*hdr=ptr;
			return ptr;
		}
		prev=ptr;
		ptr=ptr->next;
	}
	return NULL;
}

PL_ENTRY* plPinEntryByIndex(PL_ENTRY **hdr, int index)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	int i;
	if (index == 0 || !ptr) return ptr;
	for (i = 0; i < index && ptr; i++, prev = ptr, ptr = ptr->next);
	if (ptr) {
		prev->next = ptr->next;
		ptr->next = *hdr;
		*hdr = ptr;
	}
	return ptr;
}

int plEnumEntries(PL_ENTRY *hdr, PL_ENUM_CALLBACK pfnEnumCallback, 
						   void *arg, int from, int count)
{
	PL_ENTRY *ptr=hdr;
	int hits;
	if (!pfnEnumCallback) return -1;
	for (hits=0;ptr;ptr=ptr->next,hits++) {
		if (count && hits>=from+count) return hits;
		if (hits>=from)
			if ((*pfnEnumCallback)(ptr, arg)) break;
	}
	return hits;
}
