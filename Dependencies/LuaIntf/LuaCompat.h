//
// https://github.com/SteveKChiu/lua-intf
//
// Copyright 2014, Steve K. Chiu <steve.k.chiu@gmail.com>
//
// The MIT License (http://www.opensource.org/licenses/mit-license.php)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef LUACOMPAT_H
#define LUACOMPAT_H

//---------------------------------------------------------------------------

/**
 * Set LUAINTF_HEADERS_ONLY to 1 if you want this to be headers only library;
 * otherwise you need to compile some of the .cpp file separately.
 */
#ifndef LUAINTF_HEADERS_ONLY
    #define LUAINTF_HEADERS_ONLY 1
#endif

/**
 * Set LUAINTF_LINK_LUA_COMPILED_IN_CXX to 1 if you compile Lua library under C++
 * (it is *Lua* library, not LuaIntf); it is highly recommended to build Lua library
 * under C++, so it is safer and easier to work with Lua error handling.
 *
 * If Lua is compiled under C, it will use longjmp for error handling, longjmp will ignore
 * any destructor of object on stack, and may cause memory leak and other problem.
 *
 * If Lua is compiled under C++, it will throw C++ exception for error handling, and
 * destructor of object on stack will be called as expected.
 */
#ifndef LUAINTF_LINK_LUA_COMPILED_IN_CXX
    #define LUAINTF_LINK_LUA_COMPILED_IN_CXX 1
#endif

/**
 * Set LUAINTF_UNSAFE_INT64 to 1 if you want to include partial int64_t support.
 *
 * This option applys to lua 5.2 or eailier version only, or with 32-bit configuration.
 * This option has no effect to lua 5.3 or later version, which has native 64 bit int support.
 */
#ifndef LUAINTF_UNSAFE_INT64
    #define LUAINTF_UNSAFE_INT64 1
#endif

/**
 * Set LUAINTF_UNSAFE_INT64_CHECK to 1 if you want to make sure every pushed int64_t is safe,
 * that is, it can be converted from/to lua_Number without loss.
 *
 * This check will throw Lua runtime error if the conversion is not safe.
 * This check has no effect to lua 5.3 or later version, which has native 64 bit int support.
 */
#ifndef LUAINTF_UNSAFE_INT64_CHECK
    #define LUAINTF_UNSAFE_INT64_CHECK 0
#endif

/**
 * Set LUAINTF_STD_FUNCTION_WRAPPER to 1 if you want to automatically create std::function
 * wrapper for the Lua function.
 */
#ifndef LUAINTF_STD_FUNCTION_WRAPPER
    #define LUAINTF_STD_FUNCTION_WRAPPER 1
#endif

/**
 * Set LUAINTF_STD_WIDE_STRING to 1 if you want to include support for std wide string conversion.
 */
#ifndef LUAINTF_STD_WIDE_STRING
    #define LUAINTF_STD_WIDE_STRING 0
#endif

/**
 * Set LUAINTF_EXTRA_LUA_FIELDS to 1 if you want to include support for adding extra lua fields
 * for the exported C++ objects. Otherwise setting missing field will raise lua error.
 *
 * Note the extra fields is attached to Lua object, different Lua objects will have different
 * set of extra fields even they refer to the same C++ object (in the pointer or reference case).
 */
#ifndef LUAINTF_EXTRA_LUA_FIELDS
    #define LUAINTF_EXTRA_LUA_FIELDS 0
#endif

/**
 * Set LUAINTF_AUTO_DOWNCAST to 1 if you want to include support for auto downcast.
 * This will allow Lua object to access the correct class even the C++ function returns the base class.
 */
#ifndef LUAINTF_AUTO_DOWNCAST
    #define LUAINTF_AUTO_DOWNCAST 1
#endif

//---------------------------------------------------------------------------

#if LUAINTF_HEADERS_ONLY
    #define LUA_INLINE inline
#else
    #define LUA_INLINE
#endif

#if !LUAINTF_LINK_LUA_COMPILED_IN_CXX
extern "C"
{
#endif

#include "lualib.h"
#include "lauxlib.h"

#if !LUAINTF_LINK_LUA_COMPILED_IN_CXX
}
#endif

//---------------------------------------------------------------------------

#if LUA_VERSION_NUM == 501

#include <stdint.h>

#if !LUAINTF_LINK_LUA_COMPILED_IN_CXX
extern "C"
{
#endif

typedef uint32_t lua_Unsigned;

#define LUA_OK 0

#define LUA_OPEQ 0
#define LUA_OPLT 1
#define LUA_OPLE 2

#define lua_rawlen(L, i) \
    lua_objlen(L, i)

#define lua_pushglobaltable(L) \
    lua_pushvalue(L, LUA_GLOBALSINDEX)

#define lua_tounsigned(L, i) \
    lua_tounsignedx(L, i, NULL)

const lua_Number* lua_version(lua_State* L);

int lua_absindex(lua_State* L, int i);
void lua_copy(lua_State* L, int from, int to);

void lua_pushunsigned(lua_State* L, lua_Unsigned n);
lua_Number lua_tonumberx(lua_State* L, int i, int* isnum);
lua_Integer lua_tointegerx(lua_State* L, int i, int* isnum);
lua_Unsigned lua_tounsignedx(lua_State* L, int i, int* isnum);

int lua_compare(lua_State* L, int index1, int index2, int op);

void lua_len(lua_State* L, int i);
void lua_rawgetp(lua_State* L, int i, const void* p);
void lua_rawsetp(lua_State* L, int i, const void* p);

void lua_getuservalue(lua_State* L, int i);
void lua_setuservalue(lua_State* L, int i);

#define luaL_newlib(L, l) \
    (lua_newtable(L), luaL_setfuncs(L, l, 0))

void luaL_checkversion(lua_State* L);

lua_Unsigned luaL_checkunsigned(lua_State* L, int i);
lua_Unsigned luaL_optunsigned(lua_State* L, int i, lua_Unsigned def);
const char* luaL_tolstring(lua_State* L, int idx, size_t* len);

void* luaL_testudata(lua_State* L, int i, const char* tname);
void luaL_setfuncs(lua_State* L, const luaL_Reg* l, int nup);

int luaL_len(lua_State* L, int i);
void luaL_setmetatable(lua_State* L, const char* tname);
int luaL_getsubtable(lua_State* L, int i, const char* name);

void luaL_traceback(lua_State* L, lua_State* L1, const char* msg, int level);
int luaL_fileresult(lua_State* L, int stat, const char* fname);

#if !LUAINTF_LINK_LUA_COMPILED_IN_CXX
}
#endif

//---------------------------------------------------------------------------

#elif LUA_VERSION_NUM >= 503

    #ifndef lua_pushunsigned
        #define lua_pushunsigned(L, n) \
            lua_pushinteger(L, static_cast<lua_Integer>(n))

        #define lua_tounsignedx(L, i, is) \
            static_cast<lua_Unsigned>(lua_tointegerx(L, i, is))

        #define lua_tounsigned(L, i) \
            lua_tounsignedx(L, (i), nullptr)
    #endif

    #ifndef luaL_checkunsigned
        #define luaL_checkunsigned(L, a) \
            static_cast<lua_Unsigned>(luaL_checkinteger(L, a))

        #define luaL_optunsigned(L, a, d) \
            static_cast<lua_Unsigned>(luaL_optinteger(L, a, static_cast<lua_Integer>(d)))
    #endif

#endif

//---------------------------------------------------------------------------

#if LUAINTF_HEADERS_ONLY
#include "src/LuaCompat.cpp"
#endif

//---------------------------------------------------------------------------

#endif
