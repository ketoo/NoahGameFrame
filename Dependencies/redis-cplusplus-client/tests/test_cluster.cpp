#include "functions.h"

#include "../redisclient.h"

void test_cluster_dbsize(redis::client & c)
{
  redis::client::int_type count;
  for(size_t i=0; i < c.connections().size(); i++)
  {
    redis::client::int_type curSize = c.dbsize( c.connections()[i] );
    cerr << "DB#" << i << " contains " << curSize << " keys" << endl;
    count += curSize;
  }
  ASSERT_EQUAL( count, c.dbsize() );
}

boost::shared_ptr<redis::client> init_non_cluster_client()
{
  const char* c_host = getenv("REDIS_HOST");
  string host = "localhost";
  if(c_host)
    host = c_host;
  return boost::shared_ptr<redis::client>( new redis::client(host) );
}

boost::shared_ptr<redis::client> init_cluster_client()
{
  vector<redis::connection_data> redis_server;

  const char* c_host = getenv("REDIS_HOST");
  string host = "localhost";
  if(c_host)
    host = c_host;
  
  {
    /// DB0
    redis::connection_data con;
    con.host = host;
    con.port = 6379;
    con.dbindex = 14;
    redis_server.push_back(con);
  }
  {
    /// DB1
    redis::connection_data con;
    con.host = host;
    con.port = 6380;
    con.dbindex = 14;
    redis_server.push_back(con);
  }
  {
    /// DB2
    redis::connection_data con;
    con.host = host;
    con.port = 6381;
    con.dbindex = 14;
    redis_server.push_back(con);
  }
  {
    /// DB3
    redis::connection_data con;
    con.host = host;
    con.port = 6382;
    con.dbindex = 14;
    redis_server.push_back(con);
  }

  boost::shared_ptr<redis::client> cluster(
    new redis::client(redis_server.begin(), redis_server.end() )
  );
  return cluster;
}
