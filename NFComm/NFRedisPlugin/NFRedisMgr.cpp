#include "NFRedisMgr.h"


NFCRedisMgr::NFCRedisMgr() 
	: _port(0)
{
}

NFCRedisMgr::~NFCRedisMgr()
{
	Final();
}

NFCRedisMgr& NFCRedisMgr::getInstance()
{
	static NFCRedisMgr ins;
	return ins;
}

int NFCRedisMgr::Init(const char* pIp, unsigned short port)
{	
	std::thread::id thareadId = std::this_thread::get_id();
	RedisImplPtr pImpl(new NFCRedisImpl(thareadId));
	if (!pImpl->init(pIp, port))
	{
		return -1;
	}

	_mapConnect[thareadId] = pImpl;

	_szIP = pIp;
	_port = port;

	return 0;
}

int NFCRedisMgr::DeleteRedisInThread()
{
	std::thread::id thareadId = std::this_thread::get_id();
	auto itFind = _mapConnect.find(thareadId);
	if (itFind != _mapConnect.end())
	{
		itFind->second->finit();
		_mapConnect.erase(itFind);
		return 0;
	}

	return -1;
}

RedisImplPtr NFCRedisMgr::GetImpl()
{
	std::thread::id id = std::this_thread::get_id();
	auto itFind = _mapConnect.find(id);
	if (itFind != _mapConnect.end())
	{
		return itFind->second;
	}

	Init(_szIP.c_str(), _port);

	itFind = _mapConnect.find(id);
	if (itFind != _mapConnect.end())
	{
		return itFind->second;
	}

	return nullptr;
}

int NFCRedisMgr::Final()
{
	for (auto& ref : _mapConnect)
	{
		ref.second->finit();
	}

	_mapConnect.clear();

	return 0;
}
