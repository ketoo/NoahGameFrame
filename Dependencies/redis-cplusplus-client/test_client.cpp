#include "redisclient.h"

#include <iostream>
#include <boost/date_time.hpp>

#include "tests/functions.h"

boost::shared_ptr<redis::client> init_non_cluster_client();
boost::shared_ptr<redis::client> init_cluster_client();

// Low level API
void test_lists(redis::client & c);
void test_sets(redis::client & c);
void test_zsets(redis::client & c);
void test_hashes(redis::client & c);
void test_generic(redis::client & c);

// High level API
void test_distributed_strings(redis::client & c);
void test_distributed_ints(redis::client & c);
//void test_distributed_mutexes(redis::client & c);

void test_cluster();
void benchmark(redis::client & c, int TEST_SIZE);

int main()
{
  try 
  {
    bool CLUSTER_MODE = false;
    boost::shared_ptr<redis::client> shared_c;
    
    if(CLUSTER_MODE)
      shared_c = init_cluster_client();
    else
      shared_c = init_non_cluster_client();
    
    redis::client & c = *shared_c;
    
	//c.set("a", 1);

	string s = c.get("a");
	
	return 0;
    // Test on high number databases

    c.select(14);
    c.flushdb();
    ASSERT_EQUAL(c.dbsize(), (redis::client::int_type) 0);

    c.select(15);
    c.flushdb();
    ASSERT_EQUAL(c.dbsize(), (redis::client::int_type) 0);

    string foo("foo"), bar("bar"), baz("baz"), buz("buz"), goo("goo");

    test("auth");
    {
      // TODO ... needs a conf for redis-server
    }

    test("binary save values");
    {
      int repeations = 3;
      string bin;
      for(int i=0; i < repeations; i++)
      {
        for(int i1=0; i1 <= 255; i1++)
          bin += (char) i1;
      }
      c.set("binary", bin);
      string response = c.get("binary");
      ASSERT_EQUAL(response.size(), (size_t)repeations*256);
      ASSERT_EQUAL(response, bin);

      c.append("binary", bin);
      ASSERT_EQUAL(c.get("binary"), bin+bin);

      string second_half = c.substr("binary", bin.size(), -1);
      ASSERT_EQUAL(second_half, bin);
    }
    
    test("binary save keys");
    {
      string bin1 = "bin_";
      for(int i1=0; i1 <= 127; i1++)
        bin1 += (char) i1;
      
      ASSERT_EQUAL(c.exists(bin1), false);
      c.set(bin1, "hello world");
      ASSERT_EQUAL(c.exists(bin1), true);
      ASSERT_EQUAL(c.get(bin1), string("hello world"));

      string bin2 = "bin_";
      for(int i1=128; i1 <= 255; i1++)
        bin2 += (char) i1;
      
      ASSERT_EQUAL(c.exists(bin2), false);
      c.set(bin2, "hello world");
      ASSERT_EQUAL(c.exists(bin2), true);
      ASSERT_EQUAL(c.get(bin2), string("hello world"));

      redis::client::string_vector keys;
      redis::client::int_type count = c.keys("bin_*", keys);
      ASSERT_EQUAL(count, (redis::client::int_type) 2);
      ASSERT_EQUAL(keys.size(), (size_t) 2);
      if( keys[0] == bin1 )
        ASSERT_EQUAL(keys[1], bin2);
      else if( keys[0] == bin2 )
        ASSERT_EQUAL(keys[1], bin1);
      else
        // keys[0] must be bin1 or bin2 so we must fail here
        ASSERT_EQUAL(true, false);
    }
    
    redis::server_info info;
    
    test("info");
    {
      // doesn't throw? then, has valid numbers and known info-keys.
      c.info(info);
    }

    test("set, get");
    {
      c.set(foo, bar);
      ASSERT_EQUAL(c.get(foo), bar);
    }

    test("getset");
    {
      ASSERT_EQUAL(c.getset(foo, baz), bar);
      ASSERT_EQUAL(c.get(foo), baz);
    }

    test("mget");
    {
      string x_val("hello"), y_val("world");
      c.set("x", x_val);
      c.set("y", y_val);
      redis::client::string_vector keys;
      keys.push_back("x");
      keys.push_back("y");
      redis::client::string_vector vals;
      c.mget(keys, vals);
      ASSERT_EQUAL(vals.size(), size_t(2));
      ASSERT_EQUAL(vals[0], x_val);
      ASSERT_EQUAL(vals[1], y_val);
    }

    test("setnx");
    {
      ASSERT_EQUAL(c.setnx(foo, bar), false);
      ASSERT_EQUAL(c.setnx(buz, baz), true);
      ASSERT_EQUAL(c.get(buz), baz);
    }

    test("incr");
    {
      ASSERT_EQUAL(c.incr("goo"), 1L);test("nonexistent (0) -> 1");
      ASSERT_EQUAL(c.incr("goo"), 2L);test("1->2");
    }

    test("decr");
    {
      ASSERT_EQUAL(c.decr("goo"), 1L);test("2->1");
      ASSERT_EQUAL(c.decr("goo"), 0L);test("1->0");
    }

    test("incrby");
    {
      ASSERT_EQUAL(c.incrby("goo", 3L), 3L);test("0->3");
      ASSERT_EQUAL(c.incrby("goo", 2L), 5L);test("3->5");
    }

    test("exists");
    {
      ASSERT_EQUAL(c.exists("goo"), true);
    }

    test("del");
    {
      c.del("goo");
      ASSERT_EQUAL(c.exists("goo"), false);
    }

    test("type (basic)");
    {
      ASSERT_EQUAL(c.type(goo), redis::client::datatype_none);test("we deleted it");
      c.set(goo, "redis");
      ASSERT_EQUAL(c.type(goo), redis::client::datatype_string);
    }

    test("keys");
    {
      redis::client::string_vector keys;
//       ASSERT_EQUAL(c.keys("*oo", keys), 2L);
//       ASSERT_EQUAL(keys.size(), (size_t) 2);
//       ASSERT_EQUAL(keys[0], foo);
//       ASSERT_EQUAL(keys[1], goo);
    }

    test("randomkey");
    {
      ASSERT_GT(c.randomkey().size(), (size_t) 0);
    }

    test("rename");
    {
      ASSERT_EQUAL(c.exists("foo"), true);
      ASSERT_EQUAL(c.exists("doo"), false);
      c.rename("foo", "doo");
      ASSERT_EQUAL(c.exists("foo"), false);
      ASSERT_EQUAL(c.exists("doo"), true);
    }

    test("renamenx");
    {
      ASSERT_EQUAL(c.exists("doo"), true);
      ASSERT_EQUAL(c.exists("foo"), false);
      ASSERT_EQUAL(c.renamenx("doo", "foo"), true);
      ASSERT_EQUAL(c.exists("doo"), false);
      ASSERT_EQUAL(c.exists("foo"), true);
      ASSERT_EQUAL(c.renamenx("goo", "foo"), false);
      ASSERT_EQUAL(c.exists("foo"), true);
      ASSERT_EQUAL(c.exists("goo"), true);
    }

    test("dbsize");
    {
      ASSERT_GT(c.dbsize(), 0L);
    }

    test("expire");
    {
      c.expire("goo", 1);
#ifndef NDEBUG
      cerr << "please wait a few seconds.." << endl;
#endif
#ifdef _WIN32
      Sleep(2000);
#else
      sleep(2);
#endif
      ASSERT_EQUAL(c.exists("goo"), false);
    }
    
    test("move");
    {
      c.select(14);
      ASSERT_EQUAL(c.exists("ttt"), false);
      c.select(15);
      c.set("ttt", "uuu");
      c.move("ttt", 14);
      c.select(14);
      ASSERT_EQUAL(c.exists("ttt"), true);
      c.select(15);
      ASSERT_EQUAL(c.exists("ttt"), false);
    }
    
    test("move should fail since key exists already");
    {
      c.select(14);
      c.set("ttt", "xxx");
      c.select(15);
      c.set("ttt", "uuu");
      
      bool threw = false;
      
      try
      {
        c.move("ttt", 14);
      }
      catch (redis::protocol_error & e)
      {
        threw = true;
      }
      
      ASSERT_EQUAL(threw, true);
      
      c.select(14);
      ASSERT_EQUAL(c.exists("ttt"), true);
      c.select(15);
      ASSERT_EQUAL(c.exists("ttt"), true);
    }
    
    test("sort ascending");
    {
      c.sadd("sort1", "3");
      c.sadd("sort1", "2");
      c.sadd("sort1", "1");
      
      redis::client::string_vector sorted;
      ASSERT_EQUAL(c.sort("sort1", sorted), 3L);
      ASSERT_EQUAL(sorted.size(), (size_t) 3);
      ASSERT_EQUAL(sorted[0], string("1"));
      ASSERT_EQUAL(sorted[1], string("2"));
      ASSERT_EQUAL(sorted[2], string("3"));
    }
    
    test("sort descending");
    {
      redis::client::string_vector sorted;
      ASSERT_EQUAL(c.sort("sort1", sorted, redis::client::sort_order_descending), 3L);
      ASSERT_EQUAL(sorted.size(), (size_t) 3);
      ASSERT_EQUAL(sorted[0], string("3"));
      ASSERT_EQUAL(sorted[1], string("2"));
      ASSERT_EQUAL(sorted[2], string("1"));
    }
    
    test("sort with limit");
    {
      // TODO
    }
    
    test("sort lexicographically");
    {
      // TODO
    }
    
    test("sort with pattern and weights");
    {
      // TODO
    }
    
    test_lists(c);
    test_sets(c);
    test_zsets(c);
    test_hashes(c);

    test_distributed_strings(c);
    test_distributed_ints(c);
    //test_distributed_mutexes(c);
    
    test_generic(c);
    
    benchmark(c, 10000);

    test("save");
    {
      c.save();
    }

    test("bgsave");
    {
      c.bgsave();
    }

    test("lastsave");
    {
#ifdef _WIN32
      ASSERT_GT(c.lastsave(), (time_t)0L);
#else
      ASSERT_GT(c.lastsave(), 0L);
#endif
    }

    test("shutdown");
    {
// You can test this if you really want to ...
//      c.shutdown();
    }
  } 
  catch (redis::redis_error & e) 
  {
    cerr << "got exception: " << e.what() << endl << "FAIL" << endl;
    return 1;
  }

  cout << endl << "testing completed successfully" << endl;
  return 0;
}
