#include "../redisclient.h"

#include "functions.h"

void test_distributed_ints(redis::client & c)
{
  test("distributed_int");
  {
    redis::distributed_int sh_int1("sh_int1", 123, c);
    
    int int1 = sh_int1;
    ASSERT_EQUAL(int1, 123);
    ASSERT_EQUAL(sh_int1.to_int(), 123);
    
    ASSERT_EQUAL(sh_int1++, 123);
    ASSERT_EQUAL(++sh_int1, 125);
    
    ASSERT_EQUAL(sh_int1 == 125, true);
    ASSERT_EQUAL(sh_int1 != 125, false);
    
    sh_int1 -= sh_int1;
    ASSERT_EQUAL(sh_int1.to_int(), 0);
    sh_int1 += 314;
    ASSERT_EQUAL(sh_int1.to_int(), 314);

    sh_int1 -= 314*2;
    ASSERT_EQUAL(sh_int1.to_int(), -314);
    sh_int1 += 314*2;
    ASSERT_EQUAL(sh_int1.to_int(), 314);
    
    redis::distributed_int si("si", 0, c);
    redis::client::int_type i, i1 = 0;
    while( (i = si++) < 10 )
      ASSERT_EQUAL(i, i1++);

    // should not compile in couse of static assert
    //redis::distributed_base_int<double> foo(c, "sh_int2");
    //redis::distributed_base_int<std::string> foo(c, "sh_int2");
      
    boost::int32_t initialVal = std::numeric_limits<boost::int32_t>::max();
    redis::distributed_int64 sh_int64("sh_int64", initialVal, c);

    boost::int64_t newVal = initialVal;
    newVal++;
    sh_int64++;
    
    ASSERT_EQUAL(sh_int64.to_int(), newVal);

    boost::uint32_t initialVal2 = std::numeric_limits<boost::uint32_t>::max();
    sh_int64 = initialVal2;
    
    boost::int64_t newVal2 = initialVal2;
    newVal2++;
    sh_int64++;
    
    ASSERT_EQUAL(sh_int64.to_int(), newVal2);

    sh_int64 = sh_int64*2;
    ASSERT_EQUAL(sh_int64.to_int(), newVal2*2);
  }
}