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
//---------------------------------------------------------------------------
//
// Part of this file is derived from hishamhm's lua-compat-5.2
// https://github.com/hishamhm/lua-compat-5.2
//

#ifndef LUACOMPAT_H
    #include "LuaIntf/LuaCompat.h"
#endif

#if LUA_VERSION_NUM == 501

#include <errno.h>
#include <string.h>
#include <limits.h>

#define LUA_PACKAGE_KEY "_LUAINTF_COMPAT"

#ifndef lua_number2unsigned
    #if defined(LUA_NUMBER_DOUBLE) || defined(LUA_NUMBER_FLOAT)
        #include <math.h>

        #define LUA_SUPUNSIGNED	\
            ((lua_Number)(~(lua_Unsigned)0) + 1)

        #define lua_number2unsigned(i, n) \
            ((i) = (lua_Unsigned)((n) - floor((n) / LUA_SUPUNSIGNED) * LUA_SUPUNSIGNED))
    #else
        #define lua_number2unsigned(i,n) \
            ((i) = (lua_Unsigned)(n))
    #endif
#endif

#ifndef lua_unsigned2number
    #define lua_unsigned2number(u) \
        (((u) <= (lua_Unsigned)INT_MAX) ? (lua_Number)(int)(u) : (lua_Number)(u))
#endif

//---------------------------------------------------------------------------

LUA_INLINE const lua_Number* lua_version(lua_State*)
{
    static const lua_Number version = LUA_VERSION_NUM;
    return &version;
}

LUA_INLINE int lua_absindex(lua_State* L, int i)
{
    if (i < 0 && i > LUA_REGISTRYINDEX) {
        i += lua_gettop(L) + 1;
    }
    return i;
}

LUA_INLINE void lua_copy(lua_State* L, int from, int to)
{
    int abs_to = lua_absindex(L, to);
    luaL_checkstack(L, 1, "not enough stack slots");
    lua_pushvalue(L, from);
    lua_replace(L, abs_to);
}

LUA_INLINE void lua_rawgetp(lua_State* L, int i, const void* p)
{
    int abs_i = lua_absindex(L, i);
    lua_pushlightuserdata(L, (void*)p);
    lua_rawget(L, abs_i);
}

LUA_INLINE void lua_rawsetp(lua_State* L, int i, const void* p)
{
    int abs_i = lua_absindex(L, i);
    luaL_checkstack(L, 1, "not enough stack slots");
    lua_pushlightuserdata(L, (void*)p);
    lua_insert(L, -2);
    lua_rawset(L, abs_i);
}

static LUA_INLINE void _lua_pushpackagetable(lua_State* L)
{
    lua_pushliteral(L, LUA_PACKAGE_KEY);
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        /* try to get package table from globals */
        lua_pushliteral(L, "package");
        lua_rawget(L, LUA_GLOBALSINDEX);
        if (lua_istable(L, -1)) {
            lua_pushliteral(L, LUA_PACKAGE_KEY);
            lua_pushvalue(L, -2);
            lua_rawset(L, LUA_REGISTRYINDEX);
        }
    }
}

LUA_INLINE void lua_getuservalue(lua_State* L, int i)
{
    luaL_checktype(L, i, LUA_TUSERDATA);
    luaL_checkstack(L, 2, "not enough stack slots");
    lua_getfenv(L, i);
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    if (lua_rawequal(L, -1, -2)) {
        lua_pop(L, 1);
        lua_pushnil(L);
        lua_replace(L, -2);
    } else {
        lua_pop(L, 1);
        _lua_pushpackagetable(L);
        if (lua_rawequal(L, -1, -2)) {
            lua_pop(L, 1);
            lua_pushnil(L);
            lua_replace(L, -2);
        } else {
            lua_pop(L, 1);
        }
    }
}

LUA_INLINE void lua_setuservalue(lua_State* L, int i)
{
    luaL_checktype(L, i, LUA_TUSERDATA);
    if (lua_isnil(L, -1)) {
        luaL_checkstack(L, 1, "not enough stack slots");
        lua_pushvalue(L, LUA_GLOBALSINDEX);
        lua_replace(L, -2);
    }
    lua_setfenv(L, i);
}


LUA_INLINE void lua_pushunsigned(lua_State* L, lua_Unsigned n)
{
    lua_pushnumber(L, lua_unsigned2number(n));
}

LUA_INLINE lua_Unsigned lua_tounsignedx(lua_State* L, int i, int* is_num)
{
    lua_Unsigned result;
    lua_Number n = lua_tonumberx(L, i, is_num);
    lua_number2unsigned(result, n);
    return result;
}

LUA_INLINE lua_Number lua_tonumberx(lua_State* L, int i, int* is_num)
{
    lua_Number n = lua_tonumber(L, i);
    if (is_num != NULL) {
        *is_num = (n != 0 || lua_isnumber(L, i));
    }
    return n;
}

LUA_INLINE lua_Integer lua_tointegerx(lua_State* L, int i, int* is_num)
{
    lua_Integer n = lua_tointeger(L, i);
    if (is_num) {
        *is_num = (n != 0 || lua_isnumber(L, i));
    }
    return n;
}

LUA_INLINE void lua_len(lua_State* L, int i)
{
    switch (lua_type(L, i)) {
    case LUA_TSTRING:
    case LUA_TTABLE:
        lua_pushnumber(L, (int)lua_objlen(L, i));
        break;
    case LUA_TUSERDATA:
        if (luaL_callmeta(L, i, "__len")) {
            break;
        }
        // maybe fall through
    default:
        luaL_error(L, "attempt to get length of a %s value", lua_typename(L, lua_type(L, i)));
        break;
    }
}

LUA_INLINE int lua_compare(lua_State* L, int index1, int index2, int op)
{
    if (op == LUA_OPEQ) {
        return lua_equal(L, index1, index2);
    } else if (op == LUA_OPLT) {
        return lua_lessthan(L, index1, index2);
    } else if (op == LUA_OPLE) {
        return lua_lessthan(L, index1, index2) || lua_equal(L, index1, index2);
    } else {
        return 0;
    }
}

//---------------------------------------------------------------------------

LUA_INLINE void luaL_checkversion(lua_State*)
{
    // do nothing
}

LUA_INLINE lua_Unsigned luaL_checkunsigned(lua_State* L, int i)
{
    lua_Unsigned result;
    lua_Number n = lua_tonumber(L, i);
    if (n == 0 && !lua_isnumber(L, i)) {
        luaL_checktype(L, i, LUA_TNUMBER);
    }
    lua_number2unsigned(result, n);
    return result;
}

LUA_INLINE lua_Unsigned luaL_optunsigned(lua_State* L, int i, lua_Unsigned def)
{
    return luaL_opt(L, luaL_checkunsigned, i, def);
}

LUA_INLINE const char* luaL_tolstring(lua_State* L, int idx, size_t* len)
{
    if (!luaL_callmeta(L, idx, "__tostring")) {
        int t = lua_type(L, idx);
        switch (t) {
        case LUA_TNIL:
            lua_pushliteral(L, "nil");
            break;
        case LUA_TSTRING:
        case LUA_TNUMBER:
            lua_pushvalue(L, idx);
            break;
        case LUA_TBOOLEAN:
            if (lua_toboolean(L, idx)) {
                lua_pushliteral(L, "true");
            } else {
                lua_pushliteral(L, "false");
            }
            break;
        default:
            lua_pushfstring(L, "%s: %p", lua_typename(L, t), lua_topointer(L, idx));
            break;
        }
    }
    return lua_tolstring(L, -1, len);
}

LUA_INLINE int luaL_len(lua_State* L, int i)
{
    luaL_checkstack(L, 1, "not enough stack slots");
    lua_len(L, i);
    int is_num = 0;
    int res = int(lua_tointegerx(L, -1, &is_num));
    lua_pop(L, 1);
    if (!is_num) {
        luaL_error(L, "object length is not a number");
    }
    return res;
}

LUA_INLINE void* luaL_testudata(lua_State* L, int i, const char* tname)
{
    void *p = lua_touserdata(L, i);
    luaL_checkstack(L, 2, "not enough stack slots");
    if (p == NULL || !lua_getmetatable(L, i)) {
        return NULL;
    } else {
        int res = 0;
        luaL_getmetatable(L, tname);
        res = lua_rawequal(L, -1, -2);
        lua_pop(L, 2);
        if (!res) {
            p = NULL;
        }
    }
    return p;
}

LUA_INLINE void luaL_setfuncs(lua_State* L, const luaL_Reg* l, int nup)
{
    luaL_checkstack(L, nup + 1, "too many upvalues");
    for (; l->name != NULL; l++) {  /* fill the table with given functions */
        int i;
        lua_pushstring(L, l->name);
        for (i = 0; i < nup; i++) { /* copy upvalues to the top */
            lua_pushvalue(L, -(nup + 1));
        }
        lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
        lua_settable(L, -(nup + 3)); /* table must be below the upvalues, the name and the closure */
    }
    lua_pop(L, nup);  /* remove upvalues */
}

LUA_INLINE void luaL_setmetatable(lua_State* L, const char* tname)
{
    luaL_checkstack(L, 1, "not enough stack slots");
    luaL_getmetatable(L, tname);
    lua_setmetatable(L, -2);
}

LUA_INLINE int luaL_getsubtable(lua_State* L, int i, const char* name)
{
    int abs_i = lua_absindex(L, i);
    luaL_checkstack(L, 3, "not enough stack slots");
    lua_pushstring(L, name);
    lua_gettable(L, abs_i);
    if (lua_istable(L, -1)) {
        return 1;
    }
    lua_pop(L, 1);
    lua_newtable(L);
    lua_pushstring(L, name);
    lua_pushvalue(L, -2);
    lua_settable(L, abs_i);
    return 0;
}

static LUA_INLINE int _luaL_countlevels(lua_State* L)
{
    lua_Debug ar;
    int li = 1, le = 1;
    /* find an upper bound */
    while (lua_getstack(L, le, &ar)) {
        li = le;
        le *= 2;
    }
    /* do a binary search */
    while (li < le) {
        int m = (li + le) / 2;
        if (lua_getstack(L, m, &ar)) {
            li = m + 1;
        } else {
            le = m;
        }
    }
    return le - 1;
}

static LUA_INLINE int _luaL_findfield(lua_State* L, int objidx, int level)
{
    if (level == 0 || !lua_istable(L, -1)) {
        return 0;  /* not found */
    }
    lua_pushnil(L);  /* start 'next' loop */
    while (lua_next(L, -2)) {  /* for each pair in table */
        if (lua_type(L, -2) == LUA_TSTRING) {  /* ignore non-string keys */
            if (lua_rawequal(L, objidx, -1)) {  /* found object? */
                lua_pop(L, 1);  /* remove value (but keep name) */
                return 1;
            } else if (_luaL_findfield(L, objidx, level - 1)) {  /* try recursively */
                lua_remove(L, -2);  /* remove table (but keep name) */
                lua_pushliteral(L, ".");
                lua_insert(L, -2);  /* place '.' between the two names */
                lua_concat(L, 3);
                return 1;
            }
        }
        lua_pop(L, 1);  /* remove value */
    }
    return 0;  /* not found */
}

static LUA_INLINE int _luaL_pushglobalfuncname(lua_State* L, lua_Debug* ar)
{
    int top = lua_gettop(L);
    lua_getinfo(L, "f", ar);  /* push function */
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    if (_luaL_findfield(L, top + 1, 2)) {
        lua_copy(L, -1, top + 1);  /* move name to proper place */
        lua_pop(L, 2);  /* remove pushed values */
        return 1;
    } else {
        lua_settop(L, top);  /* remove function and global table */
        return 0;
    }
}

static LUA_INLINE void _luaL_pushfuncname(lua_State* L, lua_Debug* ar)
{
    if (*ar->namewhat != '\0') { /* is there a name? */
        lua_pushfstring(L, "function " LUA_QS, ar->name);
    } else if (*ar->what == 'm') { /* main? */
        lua_pushliteral(L, "main chunk");
    } else if (*ar->what == 'C') {
        if (_luaL_pushglobalfuncname(L, ar)) {
            lua_pushfstring(L, "function " LUA_QS, lua_tostring(L, -1));
            lua_remove(L, -2);  /* remove name */
        } else {
            lua_pushliteral(L, "?");
        }
    } else {
        lua_pushfstring(L, "function <%s:%d>", ar->short_src, ar->linedefined);
    }
}

LUA_INLINE void luaL_traceback(lua_State* L, lua_State* L1, const char* msg, int level)
{
    const int LEVELS1 = 12;  /* size of the first part of the stack */
    const int LEVELS2 = 10;  /* size of the second part of the stack */

    lua_Debug ar;
    int top = lua_gettop(L);
    int numlevels = _luaL_countlevels(L1);
    int mark = (numlevels > LEVELS1 + LEVELS2) ? LEVELS1 : 0;
    if (msg) lua_pushfstring(L, "%s\n", msg);
    lua_pushliteral(L, "stack traceback:");
    while (lua_getstack(L1, level++, &ar)) {
        if (level == mark) {  /* too many levels? */
            lua_pushliteral(L, "\n\t...");  /* add a '...' */
            level = numlevels - LEVELS2;  /* and skip to last ones */
        } else {
            lua_getinfo(L1, "Slnt", &ar);
            lua_pushfstring(L, "\n\t%s:", ar.short_src);
            if (ar.currentline > 0) {
                lua_pushfstring(L, "%d:", ar.currentline);
            }
            lua_pushliteral(L, " in ");
            _luaL_pushfuncname(L, &ar);
            lua_concat(L, lua_gettop(L) - top);
        }
    }
    lua_concat(L, lua_gettop(L) - top);
}

LUA_INLINE int luaL_fileresult(lua_State* L, int stat, const char* fname)
{
    int err = errno;  // calls to Lua API may change this value
    if (stat) {
        lua_pushboolean(L, 1);
        return 1;
    } else {
        lua_pushnil(L);
        if (fname) {
            lua_pushfstring(L, "%s: %s", fname, strerror(err));
        } else {
            lua_pushstring(L, strerror(err));
        }
        lua_pushnumber(L, (lua_Number)err);
        return 3;
    }
}

//---------------------------------------------------------------------------

#endif
