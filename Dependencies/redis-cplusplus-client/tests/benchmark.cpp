#include "functions.h"

#include "../redisclient.h"

void benchmark_mset(redis::client & c, int TEST_SIZE, boost::optional<const string &> opt_val = boost::none)
{
  block_duration b("Writing keys with MSET", TEST_SIZE);
  redis::client::string_pair_vector keyValuePairs;
  for(int i=0; i < TEST_SIZE; i++)
  {
    stringstream ss;
    ss << "key_" << i;
    string val = boost::lexical_cast<string>(i);
    if(opt_val)
      val = *opt_val;
    keyValuePairs.push_back( make_pair( ss.str(), val ) );

    if( keyValuePairs.size()*val.size() > (1024*1024*16) )
    {
      c.mset( keyValuePairs );
      keyValuePairs.clear();
    }
  }
  c.mset( keyValuePairs );
  keyValuePairs.clear();
}

void benchmark_get(redis::client & c, int TEST_SIZE)
{
  block_duration b("Reading keys with GET", TEST_SIZE);
  redis::client::string_vector keys;
  for(int i=0; i < TEST_SIZE; i++)
  {
    stringstream ss;
    ss << "key_" << i;
    string val = c.get( ss.str() );
  }
}

void benchmark_set(redis::client & c, int TEST_SIZE, boost::optional<const string &> opt_val = boost::none)
{
  block_duration b("Writing keys with SET", TEST_SIZE);
  redis::client::string_vector keys;
  for(int i=0; i < TEST_SIZE; i++)
  {
    stringstream ss;
    ss << "key_" << i;
    string val = boost::lexical_cast<string>(i);
    if(opt_val)
      val = *opt_val;
    
    c.set( ss.str(), val );
  }
}

void benchmark_mget(redis::client & c, int TEST_SIZE)
{
  block_duration b("Reading keys with MGET", TEST_SIZE);
  redis::client::string_vector keys;
  for(int i=0; i < TEST_SIZE; i++)
  {
    stringstream ss;
    ss << "key_" << i;
    keys.push_back( ss.str() );
    
    if( keys.size() == 250 )
    {
      redis::client::string_vector out;
      c.mget( keys, out );
      for(int i1=0; i1 < (int) keys.size(); i1++)
      {
        //assert( boost::lexical_cast<int>( keys[i1].substr(4) ) == boost::lexical_cast<int>( out[i1] ) );
        //ASSERT_EQUAL( boost::lexical_cast<int>( keys[i1].substr(4) ), boost::lexical_cast<int>( out[i1] ) );
      }
      keys.clear();
    }
  }
  redis::client::string_vector out;
  c.mget( keys, out );
  for(int i1=0; i1 < (int) keys.size(); i1++)
  {
    //assert( boost::lexical_cast<int>( keys[i1].substr(4) ) == boost::lexical_cast<int>( out[i1] ) );
    //ASSERT_EQUAL( boost::lexical_cast<int>( keys[i1].substr(4) ), boost::lexical_cast<int>( out[i1] ) );
  }
  keys.clear();
}

void benchmark_incr(redis::client & c, int TEST_SIZE)
{
  block_duration dur("Incrementing with shared_int", TEST_SIZE);
  
  // Increment
  for(int i=0; i < TEST_SIZE; i++)
  {
    stringstream ss;
    ss << "key_" << i;
    redis::distributed_int sh_int(ss.str(), c);
    
    ASSERT_EQUAL( i+1, ++sh_int );
  }
}

void benchmark(redis::client & c, int TEST_SIZE)
{
  c.flushdb();
  benchmark_set (c, TEST_SIZE);
  c.flushdb();
  benchmark_mset(c, TEST_SIZE);
  benchmark_get (c, TEST_SIZE);
  benchmark_mget(c, TEST_SIZE);
  benchmark_incr(c, TEST_SIZE);

  ///
  /// Large values
  ///
  
  string val;

  for(int i=0; i < 256; i++)
  {
    val += (char) i;
  }

  do {
    cout << endl;
    cout << "====== Value size: " << val.size() <<  " ======" << endl;
    cout << endl;

    TEST_SIZE = TEST_SIZE/1.35;
    
    c.flushdb();
    benchmark_set (c, TEST_SIZE, val);
    c.flushdb();
    benchmark_mset(c, TEST_SIZE, val);
    benchmark_get (c, TEST_SIZE);
    benchmark_mget(c, TEST_SIZE);

    val.append(val);
  } while( val.size() <= (1024*1024)*2 );
}
