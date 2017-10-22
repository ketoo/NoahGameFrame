#include "functions.h"

#include "../redisclient.h"

void type_tests(redis::client & c, int mode)
{
  using namespace redis;
  
  vector<command> commands;
  
  // 0. Status reply
  commands.push_back( makecmd("SET") << key("test_string") << "value" );
  // 1. Error reply
  commands.push_back( makecmd("LPOP") << key("test_string") ); // -ERR Operation against a key holding the wrong kind of value
  // 2. Int reply
  commands.push_back( makecmd("APPEND") << key("test_string") << "_test" );
  // 3. Bulk reply
  commands.push_back( makecmd("GET") << key("test_string") );
  // 4. Multi bulk reply
  commands.push_back( makecmd("SMEMBERS") << key("seth") );

  switch(mode)
  {
    // single exec
    case 0: 
    {
      for(size_t i=0; i < commands.size(); i++)
      {
        c.exec( commands[i] );
      }
      break;
    }
    // pipelining
    case 1:
    {
      c.exec(commands);
      break;
    }
    // pipelining + transaction
    case 2:
    {
      c.exec_transaction(commands);
      break;
    }
  }
  
  // 0. Status reply
  ASSERT_EQUAL( commands[0].reply_type(), status_code_reply );
  ASSERT_EQUAL( commands[0].get_status_code_reply(), string("OK") );
  // 1. Error reply
  ASSERT_EQUAL( commands[1].reply_type(), error_reply );
  ASSERT_NOT_EQUAL( commands[1].get_error_reply(), string() );
  // 2. Int reply
  ASSERT_EQUAL( commands[2].reply_type(), int_reply );
  ASSERT_EQUAL( commands[2].get_int_reply(), (int) (strlen("value")+strlen("_test")) );
  // 3. Bulk reply
  ASSERT_EQUAL( commands[3].reply_type(), bulk_reply );
  ASSERT_EQUAL( commands[3].get_bulk_reply(), string("value_test") );
  // 4. Multi bulk reply
  ASSERT_EQUAL( commands[4].reply_type(), multi_bulk_reply );
  vector<string> bulk = commands[4].get_multi_bulk_reply();
  if( bulk[0] == "1" )
    ASSERT_EQUAL( bulk[1], string("2") );
  else if( bulk[0] == "2" )
    ASSERT_EQUAL( bulk[1], string("1") );
  else
    ASSERT_EQUAL( true, false );
}

void bench_incr(redis::client & c, size_t count)
{
  for(size_t i=0; i < count; i++)
  {
    ostringstream ss;
    ss << "incr_bench" << i;
    c.incr( ss.str() );
  }
}

void bench_incr_pipelined(redis::client & c, size_t count)
{
  using namespace redis;
  vector<command> commands;
  
  for(size_t i=0; i < count; i++)
  {
    ostringstream ss;
    ss << "incr_bench" << i;
    commands.push_back( makecmd("INCR") << key(ss.str()) );
  }

  c.exec(commands);
}

void test_generic(redis::client & c)
{
  using namespace redis;

  test("generic response types (single)");
  {
    type_tests(c, 0);
  }
  
  test("generic response types (pipeling)");
  {
    type_tests(c, 1);
  }
  
  test("generic response types (pipeling + transaction)");
  {
    type_tests(c, 2);
  }

  test("generic incr (pipeling)");
  {
    vector<command> commands;

    for(size_t i=0; i<25; i++)
    {
      stringstream ss;
      ss << "incr_test" << i;
      commands.push_back( redis::makecmd("INCR") << key(ss.str()) );
    }

    c.exec( commands );

    for(size_t i=0; i < commands.size(); i++)
    {
      ASSERT_EQUAL( commands[i].reply_type(), int_reply );
      ASSERT_EQUAL( commands[i].get_int_reply(), 1 );
    }
    
    c.exec( commands );
    
    for(size_t i=0; i < commands.size(); i++)
    {
      ASSERT_EQUAL( commands[i].reply_type(), int_reply );
      ASSERT_EQUAL( commands[i].get_int_reply(), 2 );
    }
  }

  int recurrences = 10000;
  int var_count = 8;
  
  {
    block_duration b("Incrementing keys (normal)", recurrences*var_count);
    
    for(int i=0; i < recurrences; i++)
    {
      bench_incr(c, var_count);
    }
  }
  
  {
    ostringstream ss;
    ss << "Incrementing keys (pipelined - " << var_count << " at once)";
    block_duration b(ss.str(), recurrences*var_count);
    
    for(int i=0; i < recurrences; i++)
    {
      bench_incr_pipelined(c, var_count);
    }
  }
}
