#ifndef NFC_REDIS_MGR_H
#define NFC_REDIS_MGR_H



#include "NFRedisImpl.h"


static const int MAX_REIDS_CONNECT_COUNT = 100;

using RedisImplPtr = std::shared_ptr<NFCRedisImpl>;

class NFCRedisMgr 
{
public:
	NFCRedisMgr();
	~NFCRedisMgr();

	static NFCRedisMgr& getInstance();

	// 获的一个redis连接的实例
	RedisImplPtr GetImpl();

	// 在线程中创建连接
	int Init(const char* pIp, unsigned short port);

	// 删除线程创建连接
	int DeleteRedisInThread();
	
	int Final();

private:

	std::map<std::thread::id, RedisImplPtr> _mapConnect;

	std::string _szIP;
	unsigned short _port;
};


#endif

