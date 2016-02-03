#include "functions.h"

#include "../redisclient.h"

void test_hashes(redis::client & c)
{
  test("hset");
  {
    c.hset("hash1", "key1", "hval1");
    c.hset("hash1", "key2", "hval2");
    c.hset("hash1", "key3", "hval3");
    
    ASSERT_EQUAL(c.exists("hash1"), true);
    ASSERT_EQUAL(c.type("hash1"), redis::client::datatype_hash);
    ASSERT_EQUAL(c.hlen("hash1"), 3L);
  }
  
  test("hget");
  {
    ASSERT_EQUAL(c.hget("hash1", "key1"), string("hval1"));
    ASSERT_EQUAL(c.hget("hash1", "key2"), string("hval2"));
    ASSERT_EQUAL(c.hget("hash1", "key3"), string("hval3"));
  }
  
  test("hsetnx");
  {
    ASSERT_EQUAL(c.hsetnx("hash1", "key1", "hvalINVALID"), false);
    ASSERT_EQUAL(c.hget("hash1", "key1"), string("hval1"));
    ASSERT_EQUAL(c.hsetnx("hash1", "key4", "hval4"), true);
    ASSERT_EQUAL(c.hget("hash1", "key4"), string("hval4"));
  }
  
  redis::client::string_pair_vector content;
  test("hmset");
  {
    content.push_back( make_pair("key1", "hval1") );
    content.push_back( make_pair("key2", "hval2") );
    content.push_back( make_pair("key3", "hval3") );
    
    c.hmset("hash2", content);
  }
  
  test("hmget");
  {
    redis::client::string_vector fields;
    fields.push_back("key1");
    fields.push_back("key2");
    fields.push_back("key3");
    fields.push_back("key4");
    
    redis::client::string_vector content2;
    c.hmget("hash2", fields, content2);
    ASSERT_EQUAL(content2.size(), (size_t) 4);
    
    for(size_t i=0; i<content2.size(); i++)
    {
      if(content.size() > i)
        ASSERT_EQUAL( content2[i], content[i].second );
      else
        ASSERT_EQUAL( content2[i], redis::client::missing_value() );
    }
  }
  
  test("hincrby");
  {
    c.hset("hash3", "key1", "1");
    long l;
    l = c.hincrby("hash3", "key1", 1);
    ASSERT_EQUAL( l, 2L );
    l = c.hincrby("hash3", "key2", 1);
    ASSERT_EQUAL( l, 1L );
    l = c.hincrby("hash3", "key3", -3);
    ASSERT_EQUAL( l, -3L );

    string s;
    s = c.hget("hash3", "key1");
    ASSERT_EQUAL( s, string("2") );
    s = c.hget("hash3", "key2" );
    ASSERT_EQUAL( s, string("1") );
    s = c.hget("hash3", "key3" );
    ASSERT_EQUAL( s, string("-3") );
  }
  
  test("hexists");
  {
    ASSERT_EQUAL( c.hexists("hash3", "key1"), true );
    ASSERT_EQUAL( c.hexists("hash3", "key4"), false );
  }
  
  test("hdel");
  {
    ASSERT_EQUAL( c.hdel("hash3", "key1"), true );
    ASSERT_EQUAL( c.hexists("hash3", "key1"), false );
    
    ASSERT_EQUAL( c.hdel("hash3", "key4"), false );
    ASSERT_EQUAL( c.hexists("hash3", "key4"), false );
    
    c.hset("hash3", "key1", "hval1");
  }
  
  test("hlen");
  {
    ASSERT_EQUAL( c.hlen("hash3"), 3L );
    ASSERT_EQUAL( c.hlen("hash4"), 0L );
  }
  
  test("hkeys");
  {
    redis::client::string_vector fields;
    c.hkeys("hash3", fields);
    ASSERT_EQUAL( fields.size(), (size_t) 3 );
    std::sort(fields.begin(), fields.end());
    ASSERT_EQUAL( fields[0], string("key1") );
    ASSERT_EQUAL( fields[1], string("key2") );
    ASSERT_EQUAL( fields[2], string("key3") );
  }
  
  test("hvals");
  {
    redis::client::string_vector vals;
    c.hvals("hash1", vals);
    ASSERT_EQUAL( vals.size(), (size_t) 4 );
    std::sort(vals.begin(), vals.end());
    ASSERT_EQUAL( vals[0], string("hval1") );
    ASSERT_EQUAL( vals[1], string("hval2") );
    ASSERT_EQUAL( vals[2], string("hval3") );
    ASSERT_EQUAL( vals[3], string("hval4") );
  }
  
  test("hgetall");
  {
    redis::client::string_pair_vector entries;
    c.hgetall("hash1", entries);
    ASSERT_EQUAL( entries.size(), (size_t) 4 );
    std::sort(entries.begin(), entries.end());

    ASSERT_EQUAL( entries[0].first, string("key1") );
    ASSERT_EQUAL( entries[0].second, string("hval1") );

    ASSERT_EQUAL( entries[1].first, string("key2") );
    ASSERT_EQUAL( entries[1].second, string("hval2") );

    ASSERT_EQUAL( entries[2].first, string("key3") );
    ASSERT_EQUAL( entries[2].second, string("hval3") );
    
    ASSERT_EQUAL( entries[3].first, string("key4") );
    ASSERT_EQUAL( entries[3].second, string("hval4") );
  }
}
