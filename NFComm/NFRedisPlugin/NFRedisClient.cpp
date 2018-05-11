#include "NFRedisClient.h"
#include <functional>
#include <fstream>
#include <thread>
#include <memory>
#include <fstream>

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <WinSock2.h>	//timeval
#endif

#pragma warning(disable:4996)


NFCRedisClient::NFCRedisClient(const char *host, unsigned short port, int second) 
	:_connect(nullptr) 
{
	RedisConnectPtr connectRedis(new NFCRedisConnect(this,host,port,second));
	_listClient.push_front(connectRedis);
	_connect = connectRedis;
}

NFCRedisClient::~NFCRedisClient()
{

}

void NFCRedisClient::freeReply(void *reply)
{
	if (reply)
	{
		freeReplyObject(reply);
		reply = nullptr;
	}
}

bool  NFCRedisClient::reConnect()
{
	if (_bDestroy)
	{
		return true;
	}

	while( _connect == nullptr||!_connect->IsConnect())
	{
		if( _bDestroy )
		{
			return true;
		}

		auto ptrConnect = _connect == nullptr?nullptr: _connect->Reconnect();
		if(ptrConnect == nullptr)
		{
			if(OnSearchMaster( _connect == nullptr ? nullptr : _connect.get() ))
			{
				break;
			}
		}
		else
		{
			if( _connect->GetRole() == master)
			{
				break;
			}
			else
			{
				_connect = nullptr;
				if( OnSearchMaster( _connect == nullptr ? nullptr : _connect.get() ) )
				{
					break;
				}

			}
		}
	}
	return true;
}

bool NFCRedisClient::IsConnect()
{
	if(_connect == nullptr)
		return false;

	if(_connect->GetConnect() == nullptr)
		return false;

	if(_connect->GetConnect()->err != 0)
		return false;

	return true;
}

bool NFCRedisClient::ping()
{
// 	if (nullptr == _connect->GetConnect())
// 	{
// 		ERRORS("redis connect err!");
// 		return false;
// 	}

	if (!_connect->IsConnect())
	{
		reConnect();
	}

	RedisReplyPtr _reply = exeCmd("PING");
	if (nullptr == _reply)
	{
		//ERRORS("redis PING err!");
		return false;
	}

	if (_reply && _reply->type == REDIS_REPLY_STATUS && 0 == strcmp(_reply->str, "PONG"))
	{
		return true;
	}
	else
	{
		//ERRORS("Redis Server PING: %s\n", _reply->str);
	}

	return false;
}

void NFCRedisClient::OnMasterRedis(char* ip , unsigned short wPort )
{
	for(auto& ref : _listClient)
	{
		if(strcmp(ref->GetAddress().ip, ip) == 0 && wPort == ref->GetAddress().wPort)
			return;
	}

	RedisConnectPtr connectRedis( new NFCRedisConnect( this , ip , wPort , 5 ) );
	_listClient.push_front( connectRedis );
}

bool NFCRedisClient::OnSearchMaster(NFCRedisConnect* pOldConnect )	// 用于调整老服务器连接位置
{
	// 失效的连接是主连接
	if(pOldConnect == _connect.get())
	{
		RedisConnectPtr _tmpMaster = nullptr;
		for(auto it = _listClient.begin();it != _listClient.end();++it)
		{
			int role = ( *it )->GetServerRole();
			if(role == master)
			{
				_connect = *it;
				return true;
			}
		}
	}
	return false;
}

bool NFCRedisClient::runCmd(const char *format, ...) throw()
{
	try
	{
		if (!IsConnect())
		{
			reConnect();
		}
// 
		RedisReplyPtr _reply = nullptr;
		while (_reply == nullptr||_reply.get() == nullptr)
		{
			_reply = std::shared_ptr<redisReply>( (redisReply*)redisCommand( _connect->GetConnect() , format ) , []( redisReply * r )
			{
				if( r )freeReplyObject( r );
			} );

			if( _reply == nullptr )
			{
				//ERRORS( "redisCommand error!" );
				if( _connect->GetConnect()->err != 0 )
				{
					if(reConnect())
					{
						continue;
					}

					throw CONNECT_FAILURE;
				}

				throw REIDS_COMMAND_ERROR;
			}

			if( _reply->type == REDIS_REPLY_ERROR )
			{
				if (chkReplyError(_reply->str))
				{
					if (reConnect())
						continue;

					throw CONNECT_FAILURE;
				}

				//ERRORS("REDIS_REPLY_ERROR %s\n", _reply->str);
			}
		}


		if( _reply->type == REDIS_REPLY_STATUS && ( strcmp( _reply->str , "OK" ) == 0 || strcmp( _reply->str , "ok" ) == 0 || strcmp(_reply->str, "PONG") == 0) )
		{
			return true;
		}
	}
	catch (int err)
	{
		//ERRORS("redis err[%d]",err);
	}
	catch (char* strError)
	{
		//ERRORS("redis 查询错误[%s]", strError);
	}

	return false;
}

RedisReplyPtr NFCRedisClient::exeCmd(const char *format, ...) throw()
{
	try
	{	
		if(!IsConnect())
		{
			reConnect();
		}

		RedisReplyPtr _reply = nullptr;
		while( _reply == nullptr || _reply.get() == nullptr )
		{
			va_list ap;
			va_start( ap , format );
			_reply = std::shared_ptr<redisReply>((redisReply*)redisvCommand( _connect->GetConnect() , format, ap), [](redisReply * r){ if (r)freeReplyObject(r); });
			va_end( ap );

			if( _reply == nullptr )
			{
				if( _connect->GetConnect()->err != 0 )
				{
					if(reConnect())
					{
						_reply = nullptr;
						continue;
					}

					throw CONNECT_FAILURE;
				}

				throw REIDS_COMMAND_ERROR;
			}

			if( REDIS_REPLY_ERROR == _reply->type ) // && strstr(_reply->str, "MOVED"))
			{
				if (chkReplyError(_reply->str))
				{
					if (reConnect())
					{
						_reply = nullptr;
						continue;
					}

					throw CONNECT_FAILURE;
				}
			}
		}

		if (_reply->type == REDIS_REPLY_STATUS && (strcmp(_reply->str, "OK") == 0 || strcmp(_reply->str, "ok") == 0 || strcmp(_reply->str, "PONG") == 0))
		{
			return _reply;
		}
		return _reply;
	}
	catch (int nError)
	{
		//ERRORS("redis err[%d]", nError);
	}
	catch (char* pError)
	{
		//ERRORS("redis 查询错误[%s][%d]", pError, __LINE__);
	}
	return nullptr;
}

RedisReplyPtr NFCRedisClient::exeCmd(int argc, const char **argv, const size_t *argvlen) throw()
{
	try
	{
		if(!IsConnect())
		{
			reConnect();
		}

		RedisReplyPtr _reply = nullptr;
		while( _reply == nullptr|| _reply.get() == nullptr )
		{
			_reply = std::shared_ptr<redisReply>( (redisReply*)redisCommandArgv( _connect->GetConnect() , argc , argv , argvlen ) , []( redisReply * r )
			{
				if( r )
				{
					freeReplyObject( r );
				}
			} );

			if( _reply == nullptr )
			{
				//ERRORS( "redisvCommand error! _reply == nullptr" );
				if( _connect->GetConnect()->err != 0 )
				{
					if( reConnect() )
						continue;

					throw CONNECT_FAILURE;
				}

				throw REIDS_COMMAND_ERROR;
			}


			if (REDIS_REPLY_ERROR == _reply->type) // && strstr(_reply->str, "MOVED"))
			{
				if (chkReplyError(_reply->str))
				{
					if (reConnect())
						continue;

					throw CONNECT_FAILURE;
				}

				//ERRORS("REDIS_REPLY_ERROR %s\n", _reply->str);
				throw REIDS_REPLY_ERROR;
			}
		}

		if (_reply->type == REDIS_REPLY_STATUS && (strcmp(_reply->str, "OK") == 0 || strcmp(_reply->str, "ok") == 0 || strcmp(_reply->str, "PONG") == 0))
		{
			return _reply;
		}

		return _reply;
	}
	catch (int nError)
	{
		//ERRORS("redis err[%d]", nError);
	}
	catch (char* pError)
	{
		//ERRORS("redis 查询错误[%s][%d]", pError, __LINE__);
	}

	return nullptr;
}

bool NFCRedisClient::exeCmdInPipe(const std::string &cmd)
{
	try
	{
		if(!IsConnect())
		{
			reConnect();
		}
		
		int rtn = 1;
		rtn = redisAppendCommand( _connect->GetConnect() , cmd.c_str());
		if (REDIS_OK != rtn)
		{
			//ERRORS("redisvAppendCommand error!");
			throw REIDS_COMMAND_ERROR;
		}
		return true;
	}
	catch (int nError)
	{
		//ERRORS("redis err[%d]", nError);
	}
	catch (char* pError)
	{
		//ERRORS("redis 查询错误[%s][%d]", pError, __LINE__);
	}
	return false;
}


bool  NFCRedisClient::exeCmdInPipe(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	bool ret = exeCmdInPipeV(format, ap);
	va_end(ap);
	return ret;
}

bool NFCRedisClient::exeCmdInPipeV(const char *format, va_list ap)
{
	try
	{
		if (!IsConnect())
			reConnect();


		//RedisReplyPtr _reply((redisReply*)redisvCommand(_connect.get(), format, ap), std::bind(&NFCRedisClient::freeReply, this, std::placeholders::_1));
		int rtn = redisAppendCommand( _connect->GetConnect() , format, ap);
		if (REDIS_OK != rtn)
		{
			//ERRORS("redisvAppendCommand error!");
			throw REIDS_COMMAND_ERROR;
		}
		return true;
	}
	catch (int nError)
	{
		//ERRORS_LOG("redis err[%d]", nError);
	}
	catch (char* pError)
	{
		//ERRORS_LOG("redis 查询错误[%s][%d]", pError, __LINE__);
	}
	return false;
}

void NFCRedisClient::getReplyPipe(std::vector<std::string> &vecKeyValue, std::vector<std::pair<std::string , std::string>> &out_KeyValue )
{
	if(!IsConnect())
		reConnect();

	if(vecKeyValue.empty())
		return;

	int num = (int)vecKeyValue.size();
	redisReply **replies = (redisReply **)malloc(sizeof(redisReply*)*num);
	if (replies == nullptr)
	{
		return;
	}
	
	for (int i = 0; i < num; i++)
	{
		if( _connect == nullptr || _connect.get() == nullptr )
		{
			return;
		}

		if (redisGetReply(_connect->GetConnect(), (void**)&replies[i]) == REDIS_OK)
		{
			if (replies[i] != NULL && replies[i]->type == REDIS_REPLY_STRING)
			{
				out_KeyValue.push_back(make_pair(vecKeyValue[i], std::string(replies[i]->str, replies[i]->len)));
			}
			else
			{
				//ERRORS_LOG("getReplyPipe i=%d key=%s null!", i, vecKeyValue[i].empty() ? "" : vecKeyValue[i].c_str()) ;
			}

			if (replies[i] != NULL)
			{
				freeReplyObject(replies[i]);
			}
		}
	}

	free(replies);
}


bool NFCRedisClient::loadAllScript(const char* dir)
{
	//exeCmd("script flush");

	//std::experimental::filesystem::path path(dir);
	//if (!std::experimental::filesystem::exists(path))
	//{
	//	//ERRORS("路径不存在 %s", CodeChange::WChar_To_String(path.c_str()).c_str());
	//	return false;
	//}

	////boost::filesystem::current_path(path);
	////boost::filesystem::path cur_path(".");  //= boost::filesystem::current_path();

	//std::experimental::filesystem::directory_iterator end_iter;
	//for (std::experimental::filesystem::directory_iterator iter(path); iter != end_iter; ++iter)
	//{
	//	if (std::experimental::filesystem::is_regular_file(iter->status()))
	//	{
	//		//boost::filesystem::path path2 = iter->path().filename();
	//		std::string filename = iter->path().string();
	//		loadScript(filename.c_str());
	//	}

	//	//if (boost::filesystem::is_directory(iter->status()))
	//	//{
	//	//	
	//	//}
	//}

	return true;
}

bool NFCRedisClient::loadScript(const char*  filename)
{
	//std::ifstream ifs(filename, std::ifstream::in); //only read  
	//if (!ifs.is_open())
	//{
	//	//ERRORS("打开文件失败 file=%s",filename);
	//	return false;
	//}
	//char c = 0;
	//std::string strScript;
	//while (!ifs.eof())
	//{
	//	ifs.read(&c, 1);
	//	strScript += c;
	//}

	//ifs.close();

	//RedisReplyPtr pReply = exeCmd("script load %s", strScript.c_str());
	//if (pReply == nullptr)
	//{
	//	return false;
	//}

	//std::experimental::filesystem::path path(filename);
	//_scripts[path.filename().string()] = pReply->str;

	return true;
}

bool NFCRedisClient::runScript(const char* filename, const std::vector<std::string>& keys, const std::vector<std::string>& params)
{
	if (0 == _scripts.count(filename))
	{
		return false;
	}

	RedisReplyPtr pReply = exeCmd("script exists %s", _scripts[filename].c_str());
	if (pReply == nullptr)
	{
		return false;
	}

	size_t keysNum = keys.size();

	std::string format;
	for (const auto& v : keys)
		format += (" "+ v);

	for (const auto& v : params)
		format += (" " + v);

	pReply = exeCmd("evalsha %s %s %s", _scripts[filename].c_str(), std::to_string(keysNum).c_str(), format.c_str());
	if (pReply == nullptr)
	{
		return false;
	}

	return true;
}

bool NFCRedisClient::chkReplyError(const char* replyStr)
{
	std::string strInfo(replyStr);
	std::string::size_type pos = strInfo.find("CLUSTERDOWN");
	if (pos != std::string::npos)
	{
		if (_connect->GetConnect()->err != 0)
		{
			return true;
		}
		// 网络是连接的,但是集群已经宕了
		else 
		{
			return true;
		}
	}

	pos = strInfo.find("MOVED");
	if (pos != std::string::npos)
	{
		getMovedInfo(replyStr);
		return true;
	}

	return false;
}

void NFCRedisClient::getMovedInfo(const char *info)
{
	std::string strAddr(info);
	std::string::size_type pos1 = strAddr.rfind(' ');
	std::string::size_type pos2 = strAddr.find(':', pos1);
	if (pos2 > pos1 &&  pos2 < strAddr.length())
	{
		std::string ip = strAddr.substr(pos1, pos2 - pos1);
		std::string port = strAddr.substr(pos2 + 1);
		for (auto& ptrConnect : _listClient)
		{
			if (strcmp(ptrConnect->GetAddress().ip, ip.c_str()) == 0 &&
				atoi(port.c_str()) == ptrConnect->GetAddress().wPort)
			{
				_connect = ptrConnect;
				return;
			}
		}

		// 需要重新开连接替换
		RedisConnectPtr ptrConnect(new NFCRedisConnect(this, ip.c_str(), atoi(port.c_str())));
		_listClient.push_back(ptrConnect);
		_connect = ptrConnect;
	}
}

NFCRedisConnect::NFCRedisConnect( class NFCRedisClient* pClient , const char *host , unsigned short port , int second )
{
	strncpy( _address.ip , host , min( sizeof( _address.ip ) - 1 , strlen( host ) ) );
	_address.wPort = port;
	_address.second = second;
	_pClient = pClient;

	timeval sec;
	sec.tv_sec = second;
	sec.tv_usec = 0;

	connect( _address.ip , port , sec );
}

NFCRedisConnect::~NFCRedisConnect()
{

}

RedisContextPtr NFCRedisConnect::connect( const char *host , unsigned short port , timeval &timeout )
{
	auto lambda_connect = [&]()
	{
		_connect.reset( redisConnectWithTimeout( host , port , timeout ) , []( redisContext* c )
		{
			if( c )redisFree( c );
		} );
		if( nullptr == _connect )
		{
			return false;
		}

		if( _connect->err )
		{
			return false;
		}

		return true;
	};

	if( !lambda_connect() )
	{
		return nullptr;
	}

	// 确定服务器角色信息
	RedisReplyPtr _reply( (redisReply*)redisCommand( _connect.get() ,"ROLE" ) , []( redisReply * r )
	{
		if( r )freeReplyObject( r );
	} );

	if( _reply == nullptr )
		return false;

	if( _reply->type == REDIS_REPLY_ARRAY )
	{
		// 确定服务器是主服务器则搜索从服务器回调连接
		if( strcmp( _reply->element[0]->str , "master" ) == 0 )
		{
			_role = master;
			for(size_t k = 0; k < _reply->element[2]->elements; k++ )
			{
				redisReply* p = _reply->element[2]->element[k];
				_pClient->OnMasterRedis(p->element[0]->str, atoi( p->element[1]->str));
			}
		}
		else
		{
			_role = slave;
		}
	}

	return  _connect;
}

int NFCRedisConnect::GetServerRole()
{
	if(_connect == nullptr)
	{
		timeval sec;
		sec.tv_sec = _address.second;
		sec.tv_usec = 0;

		_connect = connect(_address.ip,_address.wPort, sec );
		if(_connect == nullptr)
			return 0;

		return _role;
	}

	if(_connect->err)
	{
		_connect = nullptr;
		timeval sec;
		sec.tv_sec = _address.second;
		sec.tv_usec = 0;

		_connect = connect( _address.ip , _address.wPort , sec );
		if( _connect == nullptr )
			return 0;

		return _role;
	}

	// 确定服务器角色信息
	RedisReplyPtr _reply( (redisReply*)redisCommand( _connect.get() , "ROLE" ) , []( redisReply * r )
	{
		if( r )freeReplyObject( r );
	} );

	if( _reply == nullptr )
		return false;


	if( _reply->type == REDIS_REPLY_ARRAY )
	{
		// 确定服务器是主服务器则搜索从服务器回调连接
		if( strcmp( _reply->element[0]->str , "master" ) == 0 )
		{
			_role = master;
			for(size_t k = 0; k < _reply->element[2]->elements; k++ )
			{
				_pClient->OnMasterRedis(_reply->element[2]->element[k]->element[0]->str, atoi(_reply->element[2]->element[k]->element[1]->str));
			}
		}
		else
		{
			_role = slave;
		}
	}

	return _role;
}

RedisContextPtr NFCRedisConnect::Reconnect()
{
	timeval sec;
	sec.tv_sec = 1;
	sec.tv_usec = 0;

	return connect( _address.ip , _address.wPort , sec );
}
