#include "functions.h"

#include "../redisclient.h"

void test_distributed_strings(redis::client & c)
{
  redis::distributed_string sh_str1("sh_str1", c);
    
  test("not existing distributed_string");
  {
    // Check uninitialized/missing string
    ASSERT_EQUAL(sh_str1.exists(), false);
    ASSERT_EQUAL(sh_str1 == redis::client::missing_value(), true);
    ASSERT_EQUAL(sh_str1 != "asdf", true);
  }
    
  test("empty distributed_string");
  {
    // Check initialized empty string
    sh_str1 = "";
    ASSERT_EQUAL(sh_str1.exists(), true);
    ASSERT_EQUAL(sh_str1 == "", true);
    ASSERT_EQUAL(sh_str1 != "asdf", true);
  }
    
  test("initialized distributed_string");
  {
    // Check initialized string
    sh_str1 = "asdf";
    ASSERT_EQUAL(sh_str1.exists(), true);
    ASSERT_EQUAL(sh_str1 != redis::client::missing_value(), true);
    ASSERT_EQUAL(sh_str1 != "", true);
    ASSERT_EQUAL(sh_str1 == "asdf", true);
  }
    
  test("append to distributed_string");
  {
      sh_str1.append("123");
      ASSERT_EQUAL(sh_str1 == "asdf123", true);
      
      sh_str1 += "456";
      ASSERT_EQUAL(sh_str1 == "asdf123456", true);
  }
  
  
  test("substr on distributed_string");
  {
    ASSERT_EQUAL(sh_str1.substr(0, 3), string("asdf"));
    ASSERT_EQUAL(sh_str1.substr(4, 7), string("1234"));
  }
    
    string str1 = sh_str1;
    ASSERT_EQUAL(str1, sh_str1.str());
    
    sh_str1.del();
    ASSERT_EQUAL(sh_str1.exists(), false);
    ASSERT_EQUAL(sh_str1.setnx("asdf"), true);
    ASSERT_EQUAL(sh_str1.setnx("asdf123"), false);
    
    cerr << "Time to life of 'sh_str1': " << sh_str1.ttl() << endl;
    ASSERT_EQUAL(sh_str1.ttl(), -1);
    sh_str1.expire(100);
    int ttl = sh_str1.ttl();
    ASSERT_EQUAL(ttl > 0, true);
#ifdef _WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
    ASSERT_EQUAL(ttl > sh_str1.ttl(), true);
    
    ASSERT_EQUAL(sh_str1.type(), redis::client::datatype_string);
    
    string oldVal = sh_str1.getset("asdf123");
    ASSERT_EQUAL(oldVal, string("asdf"));
    ASSERT_EQUAL(sh_str1.str(), string("asdf123"));
    
    redis::distributed_string sh_str2("sh_str2", "asdf123", c);
    ASSERT_EQUAL(sh_str1, sh_str2);
    sh_str2 = "asdf123456";
    ASSERT_NOT_EQUAL(sh_str1, sh_str2);
    sh_str1 = sh_str2;
    ASSERT_EQUAL(sh_str1, sh_str2);  
}