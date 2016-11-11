#include <stdio.h>
#include "httpxml.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

int mwWriteXmlHeader(char** pbuf, int* pbufsize, int ver, char* charset, char* xsl)
{
	int len2 = 0;
	int len1 = snprintf(*pbuf, *pbufsize, "<?xml version=\"%d.%d\" encoding=\"%s\"?>\n",
		ver / 10, ver % 10, charset ? charset : "utf-8");
	*pbuf += len1;
	*pbufsize -= len1;
	if (xsl){
		len2 = snprintf(*pbuf, *pbufsize, "<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", xsl);
		*pbuf += len2;
		*pbufsize -= len2;
	}
	return len1 + len2;
}

void mwWriteXmlString(char** pbuf, int* pbufsize, int indent, char* str)
{
	int i;
	int len;
	int bufsize = *pbufsize;
	if (bufsize < indent * 2) return;
	for (i = 0; i < indent; i++) {
		*((*pbuf) ++) = ' ';
		*((*pbuf) ++) = ' ';
	}
	bufsize -= indent * 2;
	len = snprintf(*pbuf, bufsize, "%s%s", str, indent ? "\n" : "");
	*pbuf += len;
	bufsize -= len;
	*pbufsize = bufsize;
}

int mwWriteXmlLine(char** pbuf, int* pbufsize, HTTP_XML_NODE *node, char *attr)
{
	int bufsize = *pbufsize;
	int len;
	int i;

	if (bufsize < node->indent * 2) return -1;
	for (i = 0; i < node->indent; i++) {
		*((*pbuf)++) = ' ';
		*((*pbuf)++) = ' ';
	}
	bufsize -= node->indent * 2;
	len = snprintf(*pbuf, bufsize, "<%s", node->name);
	*pbuf += len;
	bufsize -= len;
	if (attr) {
		len = snprintf(*pbuf, bufsize, " %s", attr);
		*pbuf += len;
		bufsize -= len;
	}
	*((*pbuf)++) = '>';
	bufsize--;

	if (node->flags & XN_CDATA) {
		len = snprintf(*pbuf, bufsize, "%s", "<![CDATA[");
		*pbuf += len;
		bufsize -= len;
	}

	len = snprintf(*pbuf, bufsize, node->fmt, node->value);
	*pbuf += len;
	bufsize -= len;

	if (node->flags & XN_CDATA) {
		len = snprintf(*pbuf, bufsize, "%s", "]]>");
		*pbuf += len;
		bufsize -= len;
	}

	len = snprintf(*pbuf, bufsize, "</%s>\n", node->name);
	*pbuf += len;
	bufsize -= len;

	*pbufsize = bufsize;
	return 0;
}
