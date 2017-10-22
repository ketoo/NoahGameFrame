#include "functions.h"

#include "../redisclient.h"

void test_lists(redis::client & c)
{
  test("rpush");
  {
    ASSERT_EQUAL(c.exists("list1"), false);
    c.rpush("list1", "val1");
    ASSERT_EQUAL(c.llen("list1"), 1L);
    ASSERT_EQUAL(c.type("list1"), redis::client::datatype_list);
    c.rpush("list1", "val2");
    ASSERT_EQUAL(c.llen("list1"), 2L);
    ASSERT_EQUAL(c.lindex("list1", 0), string("val1"));
    ASSERT_EQUAL(c.lindex("list1", 1), string("val2"));
  }
  
  test("lpush");
  {
    c.del("list1");
    ASSERT_EQUAL(c.exists("list1"), false);
    c.lpush("list1", "val1");
    ASSERT_EQUAL(c.type("list1"), redis::client::datatype_list);
    ASSERT_EQUAL(c.llen("list1"), 1L);
    c.lpush("list1", "val2");
    ASSERT_EQUAL(c.llen("list1"), 2L);
    ASSERT_EQUAL(c.lindex("list1", 0), string("val2"));
    ASSERT_EQUAL(c.lindex("list1", 1), string("val1"));
  }
  
  test("llen");
  {
    c.del("list1");
    ASSERT_EQUAL(c.exists("list1"), false);
    ASSERT_EQUAL(c.llen("list1"), 0L);
    c.lpush("list1", "x");
    ASSERT_EQUAL(c.llen("list1"), 1L);
    c.lpush("list1", "y");
    ASSERT_EQUAL(c.llen("list1"), 2L);
  }
  
  test("lrange");
  {
    ASSERT_EQUAL(c.exists("list1"), true);
    ASSERT_EQUAL(c.llen("list1"), 2L);
    redis::client::string_vector vals;
    ASSERT_EQUAL(c.lrange("list1", 0, -1, vals), (redis::client::int_type) 2);
    ASSERT_EQUAL(vals.size(), (size_t) 2);
    ASSERT_EQUAL(vals[0], string("y"));
    ASSERT_EQUAL(vals[1], string("x"));
  }
  
  test("lrange with subset of full list");
  {
    ASSERT_EQUAL(c.exists("list1"), true);
    ASSERT_EQUAL(c.llen("list1"), 2L);
    redis::client::string_vector vals;
    ASSERT_EQUAL(c.lrange("list1", 0, 1, vals), 2L); // inclusive, so entire list
    ASSERT_EQUAL(vals.size(), (size_t) 2);
    ASSERT_EQUAL(vals[0], string("y"));
    ASSERT_EQUAL(vals[1], string("x"));
    
    redis::client::string_vector vals2;
    ASSERT_EQUAL(c.lrange("list1", 0, 0, vals2), 1L); // inclusive, so first item
    ASSERT_EQUAL(vals2.size(), (size_t) 1);
    ASSERT_EQUAL(vals2[0], string("y"));
    
    redis::client::string_vector vals3;
    ASSERT_EQUAL(c.lrange("list1", -1, -1, vals3), 1L); // inclusive, so first item
    ASSERT_EQUAL(vals3.size(), (size_t) 1);
    ASSERT_EQUAL(vals3[0], string("x"));
  }
  
  test("get_list");
  {
    ASSERT_EQUAL(c.exists("list1"), true);
    ASSERT_EQUAL(c.llen("list1"), 2L);
    redis::client::string_vector vals;
    ASSERT_EQUAL(c.get_list("list1", vals), 2L);
    ASSERT_EQUAL(vals.size(), (size_t) 2);
    ASSERT_EQUAL(vals[0], string("y"));
    ASSERT_EQUAL(vals[1], string("x"));
  }
  
  test("ltrim");
  {
    ASSERT_EQUAL(c.exists("list1"), true);
    ASSERT_EQUAL(c.llen("list1"), 2L);
    c.ltrim("list1", 0, 0);
    ASSERT_EQUAL(c.exists("list1"), true);
    ASSERT_EQUAL(c.llen("list1"), 1L);
    redis::client::string_vector vals;
    ASSERT_EQUAL(c.get_list("list1", vals), 1L);
    ASSERT_EQUAL(vals[0], string("y"));
  }
  
  test("lindex");
  {
    ASSERT_EQUAL(c.lindex("list1", 0), string("y"));
    c.rpush("list1", "x");
    ASSERT_EQUAL(c.llen("list1"), 2L);
    ASSERT_EQUAL(c.lindex("list1", -1), string("x"));
    ASSERT_EQUAL(c.lindex("list1", 1), string("x"));
  }
  
  test("lset");
  {
    c.lset("list1", 1, "z");
    ASSERT_EQUAL(c.lindex("list1", 1), string("z"));
    ASSERT_EQUAL(c.llen("list1"), 2L);
  }
  
  test("lrem");
  {
    c.lrem("list1", 1, "z");
    ASSERT_EQUAL(c.llen("list1"), 1L);
    ASSERT_EQUAL(c.lindex("list1", 0), string("y"));
    
    // list1 = [ y ]
    ASSERT_EQUAL(c.lrem("list1", 0, "q"), 0L);
    
    c.rpush("list1", "z");
    c.rpush("list1", "z");
    c.rpush("list1", "z");
    c.rpush("list1", "a");
    // list1 = [ y, z, z, z, a ]
    ASSERT_EQUAL(c.lrem("list1", 2, "z"), 2L);
    // list1 = [ y, z, a ]
    ASSERT_EQUAL(c.llen("list1"), 3L);
    ASSERT_EQUAL(c.lindex("list1", 0), string("y"));
    ASSERT_EQUAL(c.lindex("list1", 1), string("z"));
    ASSERT_EQUAL(c.lindex("list1", 2), string("a"));
    
    c.rpush("list1", "z");
    // list1 = [ y, z, a, z ]
    ASSERT_EQUAL(c.lrem("list1", -1, "z"), 1L);  // <0 => rm R to L
    // list1 = [ y, z, a ]
    ASSERT_EQUAL(c.llen("list1"), 3L);
    ASSERT_EQUAL(c.lindex("list1", 0), string("y"));
    ASSERT_EQUAL(c.lindex("list1", 1), string("z"));
    ASSERT_EQUAL(c.lindex("list1", 2), string("a"));
    
    // list1 = [ y, z, a ]
    // try to remove 5 'a's but there's only 1 ... no problem.
    ASSERT_EQUAL(c.lrem("list1", 5, "a"), 1L);
    // list1 = [ y, z ]
    ASSERT_EQUAL(c.llen("list1"), 2L);
    ASSERT_EQUAL(c.lindex("list1", 0), string("y"));
    ASSERT_EQUAL(c.lindex("list1", 1), string("z"));
  }
  
  test("lrem_exact");
  {
    // list1 = [ y, z ]
    
    // try to remove 5 'z's but there's only 1 ... now it's a problem.
    
    bool threw = false;
    
    try
    {
      c.lrem_exact("list1", 5, "z");
    }
    catch (redis::value_error & e)
    {
      threw = true;
    }
    
    ASSERT_EQUAL(threw, true);
    
    // This DOES remove the one 'z' though
    // list1 = [ y ]
    
    ASSERT_EQUAL(c.llen("list1"), 1L);
    ASSERT_EQUAL(c.lindex("list1", 0), string("y"));
  }
  
  test("lpop");
  {
    ASSERT_EQUAL(c.lpop("list1"), string("y"));
    // list1 = []
    ASSERT_EQUAL(c.lpop("list1"), redis::client::missing_value());
  }
  
  test("rpop");
  {
    c.rpush("list1", "hello");
    c.rpush("list1", "world");
    ASSERT_EQUAL(c.rpop("list1"), string("world"));
    ASSERT_EQUAL(c.rpop("list1"), string("hello"));
    ASSERT_EQUAL(c.lpop("list1"), redis::client::missing_value());
  }
}