#include "functions.h"

#include "../redisclient.h"

void test_sets(redis::client & c)
{
  test("sadd");
  {
    c.sadd("set1", "sval1");
    c.sadd("set1", "sval2");
    ASSERT_EQUAL(c.exists("set1"), true);
    ASSERT_EQUAL(c.type("set1"), redis::client::datatype_set);
    ASSERT_EQUAL(c.sismember("set1", "sval1"), true);
  }
  
  test("srem");
  {
    c.srem("set1", "sval1");
    ASSERT_EQUAL(c.exists("set1"), true);
    ASSERT_EQUAL(c.type("set1"), redis::client::datatype_set);
    ASSERT_EQUAL(c.sismember("set1", "sval1"), false);
  }
  
  test("smove");
  {
    c.sadd("set1", "hi");
    // set1 = { hi }
    ASSERT_EQUAL(c.exists("set2"), false);
    c.smove("set1", "set2", "hi");
    ASSERT_EQUAL(c.sismember("set1", "hi"), false);
    ASSERT_EQUAL(c.sismember("set2", "hi"), true);
  }
  
  test("scard");
  {
    c.srem("set1", "sval2");
    ASSERT_EQUAL(c.scard("set1"), 0L);
    ASSERT_EQUAL(c.scard("set2"), 1L);
  }
  
  test("sismember");
  {
    // see above
  }
  
  test("smembers");
  {
    c.sadd("set2", "bye");
    redis::client::string_set members;
    ASSERT_EQUAL(c.smembers("set2", members), 2L);
    ASSERT_EQUAL(members.size(), (size_t) 2);
    ASSERT_NOT_EQUAL(members.find("hi"),  members.end());
    ASSERT_NOT_EQUAL(members.find("bye"), members.end());
  }
  
  test("sinter");
  {
    c.sadd("set3", "bye");
    c.sadd("set3", "bye2");
    redis::client::string_vector keys;
    keys.push_back("set2");
    keys.push_back("set3");
    redis::client::string_set intersection;
    ASSERT_EQUAL(c.sinter(keys, intersection), 1L);
    ASSERT_EQUAL(intersection.size(), (size_t) 1);
    ASSERT_NOT_EQUAL(intersection.find("bye"), intersection.end());
  }
  
  test("sinterstore");
  {
    c.sadd("seta", "1");
    c.sadd("seta", "2");
    c.sadd("seta", "3");
    
    c.sadd("setb", "2");
    c.sadd("setb", "3");
    c.sadd("setb", "4");
    
    redis::client::string_vector keys;
    keys.push_back("seta");
    keys.push_back("setb");
    
    ASSERT_EQUAL(c.sinterstore("setc", keys), 2L);
    
    redis::client::string_set members;
    ASSERT_EQUAL(c.smembers("setc", members), 2L);
    ASSERT_EQUAL(members.size(), (size_t) 2);
    ASSERT_NOT_EQUAL(members.find("2"), members.end());
    ASSERT_NOT_EQUAL(members.find("3"), members.end());
  }
  
  test("sunion");
  {
    c.sadd("setd", "1");
    c.sadd("sete", "2");
    redis::client::string_vector keys;
    keys.push_back("setd");
    keys.push_back("sete");
    redis::client::string_set a_union;
    ASSERT_EQUAL(c.sunion(keys, a_union), 2L);
    ASSERT_EQUAL(a_union.size(), (size_t) 2);
    ASSERT_NOT_EQUAL(a_union.find("1"), a_union.end());
    ASSERT_NOT_EQUAL(a_union.find("2"), a_union.end());
  }
  
  test("sunionstore");
  {
    c.sadd("setf", "1");
    c.sadd("setg", "2");
    
    redis::client::string_vector keys;
    keys.push_back("setf");
    keys.push_back("setg");
    
    ASSERT_EQUAL(c.sunionstore("seth", keys), 2L);
    
    redis::client::string_set members;
    ASSERT_EQUAL(c.smembers("seth", members), 2L);
    ASSERT_EQUAL(members.size(), (size_t) 2);
    ASSERT_NOT_EQUAL(members.find("1"), members.end());
    ASSERT_NOT_EQUAL(members.find("2"), members.end());
  }
}