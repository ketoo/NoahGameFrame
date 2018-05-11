#ifndef NFC_REDIS_CLIENT_H
#define NFC_REDIS_CLIENT_H

#include <memory>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <list>

#include <hiredis/hiredis.h>

#include "NFComm/NFPluginModule/NFPlatform.h"




#define CONNECT_FAILURE 1
#define CLIENT_CONNECT_ERROR 2
#define REIDS_COMMAND_ERROR 3
#define REIDS_REPLY_ERROR 4


struct timeval;
struct redisContext;
struct redisReply;
class NFCRedisConnect;
class NFCRedisClient;

using ScriptHashMap = std::unordered_map<std::string, std::string>;
using RedisContextPtr = std::shared_ptr<redisContext>;
using RedisReplyPtr = std::shared_ptr<redisReply>;
using RedisConnectPtr = std::shared_ptr<NFCRedisConnect>;
using RedisClientPtr = std::shared_ptr<NFCRedisClient>;


enum eRedisRole
{
	master = 1,
	slave = 2,
};

struct NFRedisSlot
{
	size_t		beg = 0;
	size_t		end = 0;
};


struct NFRedisAddr
{
	char						ip[16];
	unsigned short				wPort;

	int							second;
	NFRedisAddr()
	{
		memset(this, 0, sizeof(NFRedisAddr));
	}
};

struct NFRedisNode
{
	NFRedisNode()
	{
		memset(&this->ip, 0, sizeof(this->ip));
	}

	char						ip[16];
	unsigned short				port = 0;
	int							second = 0;
	std::map<size_t , NFRedisSlot>	slot;
};

class NFCRedisConnect
{
public:
	NFCRedisConnect( class NFCRedisClient* pClient, const char *host, unsigned short port, int second = 5 );
	~NFCRedisConnect();
	RedisContextPtr connect( const char *host , unsigned short port , timeval &timeout );

	redisContext* GetConnect()
	{
		return _connect.get();
	}

	NFRedisAddr& GetAddress()
	{
		return _address;
	}

	int GetServerRole();

	int GetRole() { return _role; }

	bool IsConnect()
	{
		if(_connect == nullptr)
			return false;

		if(_connect.get() == nullptr)
			return false;

		if(_connect->err != 0)
			return false;

		return true;
	}
	RedisContextPtr Reconnect();

	void setAddress(const char *host, unsigned short port, int second = 5)
	{
		strncpy(_address.ip, host, (std::min)(sizeof(_address.ip) - 1, strlen(host)));
		_address.wPort = port;
		_address.second = second;
	}

private:
	RedisContextPtr			_connect;
	int						_role;
	NFRedisAddr				_address;
	NFCRedisClient*			_pClient;
};

class NFCRedisClient
{
	friend class NFCRedisCluster;
public:
	NFCRedisClient(const char *host, unsigned short port, int second = 5);
	virtual ~NFCRedisClient();
	NFCRedisClient(){}
public:

	void freeReply(void *reply);
	bool chkReplyError(const char *replyStr);
	bool reConnect();
	bool IsConnect();
	RedisConnectPtr getConnectMaster() { return _connect; }
	bool ping();
	void OnMasterRedis(char* ip,unsigned short wPort);
	bool OnSearchMaster(NFCRedisConnect* pOldConnect);
	inline bool runCmd(const char *format, ...) throw();
	RedisReplyPtr exeCmd(const char *format, ...) throw();
	RedisReplyPtr exeCmd(int argc, const char **argv, const size_t *argvlen) throw();

	bool exeCmdInPipe(const std::string &cmd);
	bool exeCmdInPipe(const char *format, ...);
	bool exeCmdInPipeV(const char *format, va_list ap);
	void getReplyPipe(std::vector<std::string> &vecKeyValue, std::vector<std::pair<std::string , std::string>> &out_KeyValue);

	bool loadAllScript(const char* dir);
	bool loadScript(const char*  filename);
	bool runScript(const char* filename, const std::vector<std::string>& keys = {}, const std::vector<std::string>& params = {});

	void getMovedInfo(const char *info);
private:

	ScriptHashMap _scripts;
	RedisConnectPtr _connect;
	bool _bDestroy = false;
	std::list<RedisConnectPtr> _listClient;
	std::map<size_t , NFRedisSlot> slot;
};



#endif