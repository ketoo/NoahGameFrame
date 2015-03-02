// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include "test.hpp"
#include <luabind/luabind.hpp>

#define LUABIND_TEST_BUILTIN(type, value) \
    default_converter<type>().to(L, value); \
    default_converter<type> BOOST_PP_CAT(cv, __LINE__); \
    TEST_CHECK(BOOST_PP_CAT(cv, __LINE__).compute_score(L, -1) >= 0); \
    TEST_CHECK(BOOST_PP_CAT(cv, __LINE__).from(L, -1) == value); \
    lua_pop(L, 1)

void test_main(lua_State* L)
{
    using namespace luabind;

    LUABIND_TEST_BUILTIN(int, 1);
    LUABIND_TEST_BUILTIN(int, -1);
    LUABIND_TEST_BUILTIN(unsigned int, 1);
    LUABIND_TEST_BUILTIN(unsigned int, 2);

    LUABIND_TEST_BUILTIN(short, 1);
    LUABIND_TEST_BUILTIN(short, -1);
    LUABIND_TEST_BUILTIN(unsigned short, 1);
    LUABIND_TEST_BUILTIN(unsigned short, 2);

    LUABIND_TEST_BUILTIN(long, 1);
    LUABIND_TEST_BUILTIN(long, -1);
    LUABIND_TEST_BUILTIN(unsigned long, 1);
    LUABIND_TEST_BUILTIN(unsigned long, 2);

    LUABIND_TEST_BUILTIN(char, 1);
    LUABIND_TEST_BUILTIN(char, 2);
    LUABIND_TEST_BUILTIN(unsigned char, 1);
    LUABIND_TEST_BUILTIN(unsigned char, 2);
    LUABIND_TEST_BUILTIN(signed char, -1);
    LUABIND_TEST_BUILTIN(signed char, 1);

    LUABIND_TEST_BUILTIN(float, 1.5);
    LUABIND_TEST_BUILTIN(float, -1.5);

    LUABIND_TEST_BUILTIN(double, 1.5);
    LUABIND_TEST_BUILTIN(double, -1.5);

    LUABIND_TEST_BUILTIN(bool, true);
    LUABIND_TEST_BUILTIN(bool, false);
}
