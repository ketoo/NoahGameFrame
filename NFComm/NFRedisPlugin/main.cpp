#include <iostream>
#include <assert.h>
#include "NFRedisMgr.h"
using namespace std;

#pragma comment(lib, "NFRedisPlugin_d.lib") 
#pragma comment(lib, "hiredis_d.lib") 
#pragma comment(lib, "Win32_Interop_d.lib") 


#define _IP_ "10.5.193.153"
#define _PORT_ 6379

void test_set_get()
{
	struct A
	{
		int a;
		short b;
	};
	A aa{ 100, 200};
	NFCRedisMgr::getInstance().GetImpl()->SetValueByBinary("test1", (char*)&aa, sizeof(aa));

	A bb;
	int real_len;
	NFCRedisMgr::getInstance().GetImpl()->GetValue("test1", (char*)&bb, sizeof(bb), real_len);
	assert(bb.a == 100 && bb.b == 200);
	cout << "real_len: " << real_len << endl;

	std::string set_str1 = "abcd";
	NFCRedisMgr::getInstance().GetImpl()->SetValueByString("test2", set_str1);
	char buf[64] = { 0 };
	NFCRedisMgr::getInstance().GetImpl()->GetValue("test2", buf, sizeof(buf), real_len);
	cout << "real_len: " << real_len << endl;
	assert(std::string(buf) == "abcd");
}

int main(void)
{
	NFCRedisMgr::getInstance().Init(_IP_, _PORT_);

	test_set_get();

	NFCRedisMgr::getInstance().Final();

	std::getchar();
	return 0;
}












//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#include "xRedisClient.h"
//
//#define CACHE_TYPE_1 1
//xRedisClient xClient;
//
//// AP Hash Function
//unsigned int APHash(const char *str)
//{
//	unsigned int hash = 0;
//	int i;
//
//	for (i = 0; *str; i++) {
//		if ((i & 1) == 0) {
//			hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
//		}
//		else {
//			hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
//		}
//	}
//
//	return (hash & 0x7FFFFFFF);
//}
//
//
//void test_zadd(const char *charkey, const std::string& strValue)
//{
//	std::string strkey = charkey;
//	VALUES vVal;
//	int64_t retVal = 0;
//	int64_t scores = 168;
//	vVal.push_back(toString(scores));
//	vVal.push_back(strValue);
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(charkey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		if (xClient.zadd(dbi, strkey, vVal, retVal)) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_set(const char *strkey, const char *strValue)
//{
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(strkey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		if (xClient.set(dbi, strkey, strValue)) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_append()
//{
//	test_set("test", "hello");
//	char szKey[256] = { 0 };
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		if (xClient.append(dbi, szKey, " xsky")) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_decr()
//{
//	test_set("test", "100");
//	char szKey[256] = { 0 };
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t res = 0;
//		if (xClient.decr(dbi, szKey, res)) {
//			if (res == 99) {
//				printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
//			}
//			else {
//				printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//			}
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_decrby()
//{
//	test_set("test", "100");
//	char szKey[256] = { 0 };
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t res = 0;
//		if (xClient.decrby(dbi, szKey, 11, res)) {
//			if (res == 89) {
//				printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
//			}
//			else {
//				printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//			}
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_incr()
//{
//	test_set("test", "100");
//	char szKey[256] = { 0 };
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t res = 0;
//		if (xClient.incr(dbi, szKey, res)) {
//			if (res == 101) {
//				printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
//			}
//			else {
//				printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//			}
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_incrby()
//{
//	test_set("test", "100");
//	char szKey[256] = { 0 };
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t res = 0;
//		if (xClient.incrby(dbi, szKey, 11, res)) {
//			if (res == 111) {
//				printf("%s success %ld \r\n", __PRETTY_FUNCTION__, res);
//			}
//			else {
//				printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//			}
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_get()
//{
//
//	char szKey[256] = { 0 };
//	{
//		strcpy(szKey, "test");
//		RedisDBIdx dbi(&xClient);
//		bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//		if (bRet) {
//			string strData;
//			if (xClient.get(dbi, szKey, strData)) {
//				printf("%s success data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
//			}
//			else {
//				printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//			}
//		}
//	}
//
//
//	{
//		sprintf(szKey, "test_%u", (unsigned int)time(NULL));
//		RedisDBIdx dbi(&xClient);
//		bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//		if (bRet) {
//			string strData;
//			if (xClient.get(dbi, szKey, strData)) {
//				printf("%s error data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
//			}
//			else {
//				printf("%s success [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//			}
//		}
//	}
//}
//
//void test_type()
//{
//	char szKey[256] = { 0 };
//
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//
//
//	xClient.set(dbi, szKey, "wwwwwwwwwwwwwwwwwwwwwwwww");
//	string strData;
//	if (xClient.type(dbi, szKey, strData)) {
//		printf("%s success data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
//	}
//	else {
//		printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//	}
//
//
//
//
//
//}
//
//void test_getrange()
//{
//	test_set("test", "01234567890123456789");
//	char szKey[256] = { 0 };
//
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		string strData;
//		if (xClient.getrange(dbi, szKey, 2, 6, strData)) {
//			printf("%s success data:%s \r\n", __PRETTY_FUNCTION__, strData.c_str());
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_exists()
//{
//	char szKey[256] = { 0 };
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		if (xClient.exists(dbi, szKey)) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_del()
//{
//	char szKey[256] = { 0 };
//	strcpy(szKey, "test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		if (xClient.del(dbi, szKey)) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_mset()
//{
//	char szKey[256] = { 0 };
//	DBIArray vdbi;
//	VDATA    kvData;
//
//	for (int i = 0; i < 10; i++) {
//		RedisDBIdx dbi(&xClient);
//		sprintf(szKey, "mset_key_%d", i);
//		dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//		vdbi.push_back(dbi);
//		kvData.push_back(szKey);
//		sprintf(szKey, "mset_value_%d", i);
//		kvData.push_back(szKey);
//	}
//
//	if (xClient.mset(vdbi, kvData)) {
//		printf("%s success \r\n", __PRETTY_FUNCTION__);
//	}
//	else {
//		printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, "mset error");
//	}
//}
//
//void test_mget()
//{
//	char szKey[256] = { 0 };
//	DBIArray vdbi;
//	KEYS     kData;
//	ReplyData Reply;
//	for (int i = 0; i < 15; i++) {
//		RedisDBIdx dbi(&xClient);
//		sprintf(szKey, "mset_key_%d", i);
//		dbi.CreateDBIndex(szKey, APHash, CACHE_TYPE_1);
//		vdbi.push_back(dbi);
//		kData.push_back(szKey);
//	}
//
//	if (xClient.mget(vdbi, kData, Reply)) {
//		printf("%s success %zu \r\n", __PRETTY_FUNCTION__, Reply.size());
//		ReplyData::iterator iter = Reply.begin();
//		for (; iter != Reply.end(); iter++) {
//			printf("%d\t%s\r\n", (*iter).type, (*iter).str.c_str());
//		}
//	}
//	else {
//		printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, "mset error");
//	}
//}
//
//void test_hset()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "hashtest");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t count = 0;
//		if (xClient.hset(dbi, szHKey, "filed1", "filed1_values", count)) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//
//void test_lpush()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "list_test");
//	RedisDBIdx dbi(&xClient);
//
//	VALUES vVal;
//	vVal.push_back(toString(time(NULL)));
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t count = 0;
//		if (xClient.lpush(dbi, szHKey, vVal, count)) {
//			printf("%s success %ld \r\n", __PRETTY_FUNCTION__, count);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_llen()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "list_test");
//	RedisDBIdx dbi(&xClient);
//
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t count = 0;
//		if (xClient.llen(dbi, szHKey, count)) {
//			printf("%s success len: %ld \r\n", __PRETTY_FUNCTION__, count);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_lrange()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "list_test");
//	RedisDBIdx dbi(&xClient);
//
//	VALUES vVal;
//
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		ArrayReply Reply;
//		if (xClient.lrange(dbi, szHKey, 0, -1, Reply)) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//			ReplyData::iterator iter = Reply.begin();
//			for (; iter != Reply.end(); iter++) {
//				printf("%d\t%s\r\n", (*iter).type, (*iter).str.c_str());
//			}
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_lpop()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "list_test");
//	RedisDBIdx dbi(&xClient);
//
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		string strVal;
//		if (xClient.lpop(dbi, szHKey, strVal)) {
//			printf("%s success val: %s \r\n", __PRETTY_FUNCTION__, strVal.c_str());
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_rpop()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "list_test");
//	RedisDBIdx dbi(&xClient);
//
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		string strVal;
//		if (xClient.rpop(dbi, szHKey, strVal)) {
//			printf("%s success val: %s \r\n", __PRETTY_FUNCTION__, strVal.c_str());
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//
//void test_publish()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "pubsub_test");
//	RedisDBIdx dbi(&xClient);
//
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (bRet) {
//		int64_t count;
//		if (xClient.publish(dbi, szHKey, "test message", count)) {
//			printf("%s success \r\n", __PRETTY_FUNCTION__);
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//		}
//	}
//}
//
//void test_subscribe()
//{
//	char szHKey[256] = { 0 };
//	strcpy(szHKey, "pubsub_test");
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(szHKey, APHash, CACHE_TYPE_1);
//	if (!bRet) {
//		return;
//	}
//
//	VDATA channels;
//	channels.push_back(szHKey);
//	xRedisContext ctx;
//	if (xClient.subscribe(dbi, channels, ctx)) {
//		printf("%s success \r\n", __PRETTY_FUNCTION__);
//		ReplyData vReply;
//		while (0 == xRedisClient::GetReply(&ctx, vReply)) {
//			ReplyData::iterator iter = vReply.begin();
//			for (; iter != vReply.end(); iter++) {
//				printf("%d\t%s\r\n", (*iter).type, (*iter).str.c_str());
//			}
//		}
//
//	}
//	else {
//		printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//	}
//	xClient.unsubscribe(dbi, channels, ctx);
//	xClient.FreexRedisContext(&ctx);
//}
//
//void test_scan()
//{
//	char* pattern = "a*";
//	RedisDBIdx dbi(&xClient);
//	bool bRet = dbi.CreateDBIndex(0, CACHE_TYPE_1);
//	if (!bRet) {
//		return;
//	}
//
//	ArrayReply arrayReply;
//	int64_t cursor = 0;
//	xRedisContext ctx;
//	xClient.GetxRedisContext(dbi, &ctx);
//
//	do
//	{
//		arrayReply.clear();
//		if (xClient.scan(dbi, cursor, pattern, 0, arrayReply, ctx)) {
//			printf("%lld\t\r\n", cursor);
//			ReplyData::iterator iter = arrayReply.begin();
//			for (; iter != arrayReply.end(); iter++) {
//				printf("\t\t%s\r\n", (*iter).str.c_str());
//			}
//		}
//		else {
//			printf("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//			break;
//		}
//	} while (cursor != 0);
//
//	xClient.FreexRedisContext(&ctx);
//}
//
//int main(int argc, char **argv)
//{
//	printf("%d %s\r\n", argc, argv[0]);
//
//	xClient.Init(3);
//
//	RedisNode RedisList1[3] = {
//		{ 0, "127.0.0.1", 6379, "", 2, 5, 0 },
//	{ 1, "127.0.0.1", 6379, "", 2, 5, 0 },
//	{ 2, "127.0.0.1", 6379, "", 2, 5, 0 }
//	};
//
//	xClient.ConnectRedisCache(RedisList1, sizeof(RedisList1) / sizeof(RedisNode), 3, CACHE_TYPE_1);
//
//	test_zadd("test:sorted:key", "sorted value hello xredis");
//	test_set("test", "wwww");
//	test_get();
//	test_getrange();
//	test_exists();
//	test_del();
//	test_hset();
//	test_mset();
//	test_mget();
//	test_append();
//
//	test_decr();
//	test_decrby();
//	test_incr();
//	test_incrby();
//
//	test_lpush();
//	test_llen();
//	test_lrange();
//	test_lpop();
//	test_rpop();
//
//	test_type();
//	test_scan();
//	//int n = 10;
//	//while (n--) {
//	//    xClient.KeepAlive();
//	//    usleep(1000*1000*10);
//	//}
//
//	xClient.Release();
//
//	return 0;
//}


